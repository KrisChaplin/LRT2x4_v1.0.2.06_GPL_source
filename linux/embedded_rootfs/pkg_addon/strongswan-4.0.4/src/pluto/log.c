/* error logging functions
 * Copyright (C) 1997 Angelos D. Keromytis.
 * Copyright (C) 1998-2001  D. Hugh Redelmeier.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * RCSID $Id: log.c 12119 2013-07-09 13:59:35Z dio.li $
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <syslog.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>	/* used only if MSG_NOSIGNAL not defined */
#include <sys/queue.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <freeswan.h>

#include "constants.h"
#include "defs.h"
#include "log.h"
#include "server.h"
#include "state.h"
#include "connections.h"
#include "kernel.h"
#include "whack.h"	/* needs connections.h */
#include "timer.h"

/* close one per-peer log */
static void perpeer_logclose(struct connection *c);	/* forward */


bool
    log_to_stderr = FALSE,	/* should log go to stderr? */
    log_to_syslog = FALSE,	/* should log go to syslog? */
    log_to_perpeer= FALSE;	/* should log go to per-IP file? */

bool
    logged_txt_warning = FALSE;  /* should we complain about finding KEY? */

/* should we complain when we find no local id */
bool
    logged_myid_fqdn_txt_warning = FALSE,
    logged_myid_ip_txt_warning   = FALSE,
    logged_myid_fqdn_key_warning = FALSE,
    logged_myid_ip_key_warning   = FALSE;

/* may include trailing / */
const char *base_perpeer_logdir = PERPEERLOGDIR;
static int perpeer_count = 0;

/* from sys/queue.h */
static CIRCLEQ_HEAD(,connection) perpeer_list;


/* Context for logging.
 *
 * Global variables: must be carefully adjusted at transaction boundaries!
 * If the context provides a whack file descriptor, messages
 * should be copied to it -- see whack_log()
 */
int whack_log_fd = NULL_FD;	/* only set during whack_handle() */
struct state *cur_state = NULL;	/* current state, for diagnostics */
struct connection *cur_connection = NULL;	/* current connection, for diagnostics */
const ip_address *cur_from = NULL;	/* source of current current message */
u_int16_t cur_from_port;	/* host order */

void
init_log(const char *program)
{
    if (log_to_stderr)
	setbuf(stderr, NULL);
    if (log_to_syslog)
	openlog(program, LOG_CONS | LOG_NDELAY | LOG_PID, LOG_AUTHPRIV);

    CIRCLEQ_INIT(&perpeer_list);
}

void
close_peerlog(void)
{
    /* end of circular queue is given by pointer to "HEAD"
     * BUT if the queue is not initialized, this won't be true
     * so we must guard by test perpeer_list.cqh_first != NULL
     */
    if (perpeer_list.cqh_first != NULL)
	while (perpeer_list.cqh_first != (void *)&perpeer_list)
	    perpeer_logclose(perpeer_list.cqh_first);
}

void
close_log(void)
{
    if (log_to_syslog)
	closelog();

    close_peerlog();
}

/* Sanitize character string in situ: turns dangerous characters into \OOO.
 * With a bit of work, we could use simpler reps for \\, \r, etc.,
 * but this is only to protect against something that shouldn't be used.
 * Truncate resulting string to what fits in buffer.
 */
static size_t
sanitize(char *buf, size_t size)
{
#   define UGLY_WIDTH	4	/* width for ugly character: \OOO */
    size_t len;
    size_t added = 0;
    char *p;

    passert(size >= UGLY_WIDTH);	/* need room to swing cat */

    /* find right side of string to be sanitized and count
     * number of columns to be added.  Stop on end of string
     * or lack of room for more result.
     */
    for (p = buf; *p != '\0' && &p[added] < &buf[size - UGLY_WIDTH]; )
    {
	unsigned char c = *p++;

	if (c == '\\' || !isprint(c))
	    added += UGLY_WIDTH - 1;
    }

    /* at this point, p points after last original character to be
     * included.  added is how many characters are added to sanitize.
     * so p[added] will point after last sanitized character.
     */

    p[added] = '\0';
    len = &p[added] - buf;

    /* scan backwards, copying characters to their new home
     * and inserting the expansions for ugly characters.
     * It is finished when no more shifting is required.
     * This is a predecrement loop.
     */
    while (added != 0)
    {
	char fmtd[UGLY_WIDTH + 1];
	unsigned char c;

	while ((c = *--p) != '\\' && isprint(c))
	    p[added] = c;
	added -= UGLY_WIDTH - 1;
	snprintf(fmtd, sizeof(fmtd), "\\%03o", c);
	memcpy(p + added, fmtd, UGLY_WIDTH);
    }
    return len;
#   undef UGLY_WIDTH
}

/* format a string for the log, with suitable prefixes.
 * A format starting with ~ indicates that this is a reprocessing
 * of the message, so prefixing and quoting is suppressed.
 */
static void
fmt_log(char *buf, size_t buf_len, const char *fmt, va_list ap)
{
    bool reproc = *fmt == '~';
    size_t ps;
    struct connection *c = cur_state != NULL ? cur_state->st_connection
	: cur_connection;

    buf[0] = '\0';
    if (reproc)
	fmt++;	/* ~ at start of format suppresses this prefix */
    else if (c != NULL)
    {
	/* start with name of connection */
	char *const be = buf + buf_len;
	char *bp = buf;

	snprintf(bp, be - bp, "(%s)", c->name);
	bp += strlen(bp);

	/* if it fits, put in any connection instance information */
	if (be - bp > CONN_INST_BUF)
	{
	    fmt_conn_instance(c, bp);
	    bp += strlen(bp);
	}

	if (cur_state != NULL)
	{
	    /* state number */
	    snprintf(bp, be - bp, " #%lu", cur_state->st_serialno);
	    bp += strlen(bp);
	}
	snprintf(bp, be - bp, ": ");
    }
    else if (cur_from != NULL)
    {
	/* peer's IP address */
	/* Note: must not use ip_str() because our caller might! */
	char ab[ADDRTOT_BUF];

	(void) addrtot(cur_from, 0, ab, sizeof(ab));
	snprintf(buf, buf_len, "packet from %s:%u: "
	    , ab, (unsigned)cur_from_port);
    }

    ps = strlen(buf);
    vsnprintf(buf + ps, buf_len - ps, fmt, ap);
    if (!reproc)
	(void)sanitize(buf, buf_len);
}

static void
perpeer_logclose(struct connection *c)
{
    /* only free/close things if we had used them! */
    if (c->log_file != NULL)
    {
	passert(perpeer_count > 0);

	CIRCLEQ_REMOVE(&perpeer_list, c, log_link);
	perpeer_count--;
	fclose(c->log_file);
	c->log_file=NULL;
    }
}

void
perpeer_logfree(struct connection *c)
{
    perpeer_logclose(c);
    if (c->log_file_name != NULL)
    {
	pfree(c->log_file_name);
	c->log_file_name = NULL;
	c->log_file_err = FALSE;
    }
}

/* open the per-peer log */
static void
open_peerlog(struct connection *c)
{
    syslog(LOG_INFO, "opening log file for conn %s", c->name);

    if (c->log_file_name == NULL)
    {
	char peername[ADDRTOT_BUF], dname[ADDRTOT_BUF];
	int  peernamelen, lf_len;

	addrtot(&c->spd.that.host_addr, 'Q', peername, sizeof(peername));
	peernamelen = strlen(peername);

	/* copy IP address, turning : and . into / */
	{
		char ch, *p, *q;

		p = peername;
		q = dname;
		do {
			ch = *p++;
			if (ch == '.' || ch == ':')
				ch = '/';
			*q++ = ch;
		} while (ch != '\0');
	}

	lf_len = peernamelen * 2
	    + strlen(base_perpeer_logdir)
	    + sizeof("//.log")
	    + 1;
	c->log_file_name = alloc_bytes(lf_len, "per-peer log file name");

	fprintf(stderr, "base dir |%s| dname |%s| peername |%s|"
		, base_perpeer_logdir, dname, peername);
	snprintf(c->log_file_name, lf_len, "%s/%s/%s.log"
		 , base_perpeer_logdir, dname, peername);

	syslog(LOG_DEBUG, "conn %s logfile is %s", c->name, c->log_file_name);
    }

    /* now open the file, creating directories if necessary */

    {  /* create the directory */
	char *dname;
	int   bpl_len = strlen(base_perpeer_logdir);
	char *slashloc;

	dname = clone_str(c->log_file_name, "temp copy of file name");
	dname = dirname(dname);

	if (access(dname, W_OK) != 0)
	{
	    if (errno != ENOENT)
	    {
		if (c->log_file_err)
		{
		    syslog(LOG_CRIT, "can not write to %s: %s"
			   , dname, strerror(errno));
		    c->log_file_err = TRUE;
		    pfree(dname);
		    return;
		}
	    }

	    /* directory does not exist, walk path creating dirs */
	    /* start at base_perpeer_logdir */
	    slashloc = dname + bpl_len;
	    slashloc++;    /* since, by construction there is a slash
			      right there */

	    while (*slashloc != '\0')
	    {
		char saveslash;

		/* look for next slash */
		while (*slashloc != '\0' && *slashloc != '/') slashloc++;

		saveslash = *slashloc;

		*slashloc = '\0';

		if (mkdir(dname, 0750) != 0 && errno != EEXIST)
		{
		    syslog(LOG_CRIT, "can not create dir %s: %s"
			   , dname, strerror(errno));
		    c->log_file_err = TRUE;
		    pfree(dname);
		    return;
		}
		syslog(LOG_DEBUG, "created new directory %s", dname);
		*slashloc = saveslash;
		slashloc++;
	    }
	}

	pfree(dname);
    }

    c->log_file = fopen(c->log_file_name, "a");
    if (c->log_file == NULL)
    {
	if (c->log_file_err)
	{
	    syslog(LOG_CRIT, "logging system can not open %s: %s"
		   , c->log_file_name, strerror(errno));
	    c->log_file_err = TRUE;
	}
	return;
    }

    /* look for a connection to close! */
    while (perpeer_count >= MAX_PEERLOG_COUNT)
    {
	/* can not be NULL because perpeer_count > 0 */
	passert(perpeer_list.cqh_last != (void *)&perpeer_list);

	perpeer_logclose(perpeer_list.cqh_last);
    }

    /* insert this into the list */
    CIRCLEQ_INSERT_HEAD(&perpeer_list, c, log_link);
    passert(c->log_file != NULL);
    perpeer_count++;
}

/* log a line to cur_connection's log */
static void
peerlog(const char *prefix, const char *m)
{
    if (cur_connection == NULL)
    {
	/* we can not log it in this case. Oh well. */
	return;
    }

    if (cur_connection->log_file == NULL)
    {
	open_peerlog(cur_connection);
    }

    /* despite our attempts above, we may not be able to open the file. */
    if (cur_connection->log_file != NULL)
    {
	char datebuf[32];
	time_t n;
	struct tm *t;

	time(&n);
	t = localtime(&n);

	strftime(datebuf, sizeof(datebuf), "%Y-%m-%d %T", t);
	fprintf(cur_connection->log_file, "%s %s%s\n", datebuf, prefix, m);

	/* now move it to the front of the list */
	CIRCLEQ_REMOVE(&perpeer_list, cur_connection, log_link);
	CIRCLEQ_INSERT_HEAD(&perpeer_list, cur_connection, log_link);
    }
}

void
plog(const char *message, ...)
{
    va_list args;
    char m[LOG_WIDTH];	/* longer messages will be truncated */

    if (DBGP(DBG_CONTROLMORE | DBG_CONTROL));
    else if(strstr(message, "Established]") || strstr(message, "Disconnected]"));
    else return;

    va_start(args, message);
    fmt_log(m, sizeof(m), message, args);
    va_end(args);

    if (log_to_stderr)
	fprintf(stderr, "%s\n", m);
    if (log_to_syslog)
	syslog(LOG_WARNING, "%s", m);
    if (log_to_perpeer)
	peerlog("", m);

    whack_log(RC_LOG, "~%s", m);

    NK_LOG_VPN(LOG_WARNING,"%s", m);
    closelog(); 	
}

void
loglog(int mess_no, const char *message, ...)
{
    va_list args;
    char m[LOG_WIDTH];	/* longer messages will be truncated */

    if (DBGP(DBG_CONTROLMORE | DBG_CONTROL));	
    else if (mess_no == RC_LOG_SERIOUS && strstr(message, "[Tunnel") && !strstr(message, "Info]"));
    else return;	

    va_start(args, message);
    fmt_log(m, sizeof(m), message, args);
    va_end(args);

    if (log_to_stderr)
	fprintf(stderr, "%s\n", m);
    if (log_to_syslog)
	syslog(LOG_WARNING, "%s", m);
    if (log_to_perpeer)
	peerlog("", m);

    whack_log(mess_no, "~%s", m);

    NK_LOG_VPN(LOG_WARNING,"%s", m);
    closelog(); 		
}

void
log_errno_routine(int e, const char *message, ...)
{
    va_list args;
    char m[LOG_WIDTH];	/* longer messages will be truncated */

    va_start(args, message);
    fmt_log(m, sizeof(m), message, args);
    va_end(args);

#if 0
    if (log_to_stderr)
	fprintf(stderr, "ERROR: %s. Errno %d: %s\n", m, e, strerror(e));
    if (log_to_syslog)
	syslog(LOG_ERR, "ERROR: %s. Errno %d: %s", m, e, strerror(e));
    if (log_to_perpeer)
    {
	peerlog(strerror(e), m);
    }

    whack_log(RC_LOG_SERIOUS
	, "~ERROR: %s. Errno %d: %s", m, e, strerror(e));
#else
    if (log_to_stderr)
	fprintf(stderr, "%s , cause: %s\n", m, strerror(e));
    if (log_to_syslog)
	syslog(LOG_ERR, "%s , cause: %s", m, strerror(e));
    if (log_to_perpeer)
    {
    	peerlog(strerror(e), m);
    }

    whack_log(RC_LOG_SERIOUS
	, "%s, cause: %s", m, strerror(e));
	
    NK_LOG_VPN(LOG_WARNING,"%s", m);
    closelog(); 		
#endif
}

void
exit_log(const char *message, ...)
{
    va_list args;
    char m[LOG_WIDTH];	/* longer messages will be truncated */

    va_start(args, message);
    fmt_log(m, sizeof(m), message, args);
    va_end(args);

    if (log_to_stderr)
	fprintf(stderr, "FATAL ERROR: %s\n", m);
    if (log_to_syslog)
	syslog(LOG_ERR, "FATAL ERROR: %s", m);
    if (log_to_perpeer)
	peerlog("FATAL ERROR: ", m);

    whack_log(RC_LOG_SERIOUS, "~FATAL ERROR: %s", m);

    exit_pluto(1);
}

void
exit_log_errno_routine(int e, const char *message, ...)
{
    va_list args;
    char m[LOG_WIDTH];	/* longer messages will be truncated */

    va_start(args, message);
    fmt_log(m, sizeof(m), message, args);
    va_end(args);

    if (log_to_stderr)
	fprintf(stderr, "FATAL ERROR: %s. Errno %d: %s\n", m, e, strerror(e));
    if (log_to_syslog)
	syslog(LOG_ERR, "FATAL ERROR: %s. Errno %d: %s", m, e, strerror(e));
    if (log_to_perpeer)
	peerlog(strerror(e), m);

    whack_log(RC_LOG_SERIOUS
	, "~FATAL ERROR: %s. Errno %d: %s", m, e, strerror(e));

    exit_pluto(1);
}

/* emit message to whack.
 * form is "ddd statename text" where
 * - ddd is a decimal status code (RC_*) as described in whack.h
 * - text is a human-readable annotation
 */
#ifdef DEBUG
static volatile sig_atomic_t dying_breath = FALSE;
#endif

void
whack_log(int mess_no, const char *message, ...)
{
    int wfd = whack_log_fd != NULL_FD ? whack_log_fd
	: cur_state != NULL ? cur_state->st_whack_sock
	: NULL_FD;

    if (DBGP(DBG_CONTROLMORE | DBG_CONTROL));
    else if (mess_no != RC_COMMENT) return;

    if (wfd != NULL_FD
#ifdef DEBUG
    || dying_breath
#endif
    )
    {
	va_list args;
	char m[LOG_WIDTH];	/* longer messages will be truncated */
	int prelen = snprintf(m, sizeof(m), "%03d ", mess_no);

	passert(prelen >= 0);

	va_start(args, message);
	fmt_log(m+prelen, sizeof(m)-prelen, message, args);
	va_end(args);

#if DEBUG
	if (dying_breath)
	{
	    /* status output copied to log */
	    if (log_to_stderr)
		fprintf(stderr, "%s\n", m + prelen);
	    if (log_to_syslog)
		syslog(LOG_WARNING, "%s", m + prelen);
	    if (log_to_perpeer)
		peerlog("", m);
	}
#endif

	if (wfd != NULL_FD)
	{
	    /* write to whack socket, but suppress possible SIGPIPE */
	    size_t len = strlen(m);
#ifdef MSG_NOSIGNAL	/* depends on version of glibc??? */
	    m[len] = '\n';	/* don't need NUL, do need NL */
	    (void) send(wfd, m, len + 1, MSG_NOSIGNAL);
#else /* !MSG_NOSIGNAL */
	    int r;
	    struct sigaction act
		, oldact;

	    m[len] = '\n';	/* don't need NUL, do need NL */
	    act.sa_handler = SIG_IGN;
	    sigemptyset(&act.sa_mask);
	    act.sa_flags = 0;	/* no nothing */
	    r = sigaction(SIGPIPE, &act, &oldact);
	    passert(r == 0);

	    (void) write(wfd, m, len + 1);

	    r = sigaction(SIGPIPE, &oldact, NULL);
	    passert(r == 0);
#endif /* !MSG_NOSIGNAL */
	}
    }
}

/* Build up a diagnostic in a static buffer.
 * Although this would be a generally useful function, it is very
 * hard to come up with a discipline that prevents different uses
 * from interfering.  It is intended that by limiting it to building
 * diagnostics, we will avoid this problem.
 * Juggling is performed to allow an argument to be a previous
 * result: the new string may safely depend on the old one.  This
 * restriction is not checked in any way: violators will produce
 * confusing results (without crashing!).
 */
char diag_space[sizeof(diag_space)];

err_t
builddiag(const char *fmt, ...)
{
    static char diag_space[LOG_WIDTH];	/* longer messages will be truncated */
    char t[sizeof(diag_space)];	/* build result here first */
    va_list args;

    va_start(args, fmt);
    t[0] = '\0';	/* in case nothing terminates string */
    vsnprintf(t, sizeof(t), fmt, args);
    va_end(args);
    strcpy(diag_space, t);
    return diag_space;
}

/* Debugging message support */

#ifdef DEBUG

void
switch_fail(int n, const char *file_str, unsigned long line_no)
{
    char buf[30];

    snprintf(buf, sizeof(buf), "case %d unexpected", n);
    passert_fail(buf, file_str, line_no);
}

void
passert_fail(const char *pred_str, const char *file_str, unsigned long line_no)
{
    /* we will get a possibly unplanned prefix.  Hope it works */
    loglog(RC_LOG_SERIOUS, "ASSERTION FAILED at %s:%lu: %s", file_str, line_no, pred_str);
    if (!dying_breath)
    {
	dying_breath = TRUE;
	show_status(TRUE, NULL);
    }
    abort();	/* exiting correctly doesn't always work */
}

void
pexpect_log(const char *pred_str, const char *file_str, unsigned long line_no)
{
    /* we will get a possibly unplanned prefix.  Hope it works */
    loglog(RC_LOG_SERIOUS, "EXPECTATION FAILED at %s:%lu: %s", file_str, line_no, pred_str);
}

lset_t
    base_debugging = DBG_NONE,	/* default to reporting nothing */
    cur_debugging =  DBG_NONE;

void
extra_debugging(const struct connection *c)
{
    if(c == NULL)
    {
	reset_debugging();
	return;
    }

    if (c!= NULL && c->extra_debugging != 0)
    {
	plog("enabling for connection: %s"
	    , bitnamesof(debug_bit_names, c->extra_debugging & ~cur_debugging));
	cur_debugging |= c->extra_debugging;
    }
}

/* log a debugging message (prefixed by "| ") */

void
DBG_log(const char *message, ...)
{
    va_list args;
    char m[LOG_WIDTH];	/* longer messages will be truncated */

    va_start(args, message);
    vsnprintf(m, sizeof(m), message, args);
    va_end(args);

    (void)sanitize(m, sizeof(m));

    if (log_to_stderr)
	fprintf(stderr, "| %s\n", m);
    if (log_to_syslog)
	syslog(LOG_DEBUG, "| %s", m);
    if (log_to_perpeer)
	peerlog("| ", m);

    NK_LOG_VPN(LOG_WARNING, "| %s", m);
    closelog(); 		
}

/* dump raw bytes in hex to stderr (for lack of any better destination) */

void
DBG_dump(const char *label, const void *p, size_t len)
{
#   define DUMP_LABEL_WIDTH 20	/* arbitrary modest boundary */
#   define DUMP_WIDTH	(4 * (1 + 4 * 3) + 1)
    char buf[DUMP_LABEL_WIDTH + DUMP_WIDTH];
    char *bp;
    const unsigned char *cp = p;

    bp = buf;

    if (label != NULL && label[0] != '\0')
    {
	/* Handle the label.  Care must be taken to avoid buffer overrun. */
	size_t llen = strlen(label);

	if (llen + 1 > sizeof(buf))
	{
	    DBG_log("%s", label);
	}
	else
	{
	    strcpy(buf, label);
	    if (buf[llen-1] == '\n')
	    {
		buf[llen-1] = '\0';	/* get rid of newline */
		DBG_log("%s", buf);
	    }
	    else if (llen < DUMP_LABEL_WIDTH)
	    {
		bp = buf + llen;
	    }
	    else
	    {
		DBG_log("%s", buf);
	    }
	}
    }

    do {
	int i, j;

	for (i = 0; len!=0 && i!=4; i++)
	{
	    *bp++ = ' ';
	    for (j = 0; len!=0 && j!=4; len--, j++)
	    {
		static const char hexdig[] = "0123456789abcdef";

		*bp++ = ' ';
		*bp++ = hexdig[(*cp >> 4) & 0xF];
		*bp++ = hexdig[*cp & 0xF];
		cp++;
	    }
	}
	*bp = '\0';
	DBG_log("%s", buf);
	bp = buf;
    } while (len != 0);
#   undef DUMP_LABEL_WIDTH
#   undef DUMP_WIDTH
}

#endif /* DEBUG */

void
show_status(bool all, const char *name)
{
    if (all)
    {
	show_ifaces_status();
	show_myid_status();
	show_debug_status();
	whack_log(RC_COMMENT, BLANK_FORMAT);	/* spacer */
    }
    show_connections_status(all, name);
    show_states_status(name);
#ifdef KLIPS
    show_shunt_status();
#endif
}

/* ip_str: a simple to use variant of addrtot.
 * It stores its result in a static buffer.
 * This means that newer calls overwrite the storage of older calls.
 * Note: this is not used in any of the logging functions, so their
 * callers may use it.
 */
const char *
ip_str(const ip_address *src)
{
    static char buf[ADDRTOT_BUF];

    addrtot(src, 0, buf, sizeof(buf));
    return buf;
}

/*
 * a routine that attempts to schedule itself daily.
 *
 */

void
daily_log_reset(void)
{
    /* now perform actions */
    logged_txt_warning = FALSE;

    logged_myid_fqdn_txt_warning = FALSE;
    logged_myid_ip_txt_warning   = FALSE;
    logged_myid_fqdn_key_warning = FALSE;
    logged_myid_ip_key_warning   = FALSE;
}

void
daily_log_event(void)
{
    struct tm *ltime;
    time_t n, interval;

    /* attempt to schedule oneself to midnight, local time
     * do this by getting seconds in the day, and delaying
     * by 86400 - hour*3600+minutes*60+seconds.
     */
    time(&n);
    ltime = localtime(&n);
    interval = (24 * 60 * 60)
      - (ltime->tm_sec
	 + ltime->tm_min  * 60
	 + ltime->tm_hour * 3600);

    event_schedule(EVENT_LOG_DAILY, interval, NULL);

    daily_log_reset();
}

/*
 * Local Variables:
 * c-basic-offset:4
 * c-style: pluto
 * End:
 */
