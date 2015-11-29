/* Pluto main program
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
 * RCSID $Id: plutomain.c 12119 2013-07-09 13:59:35Z dio.li $
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <getopt.h>
#include <resolv.h>
#include <arpa/nameser.h>	/* missing from <resolv.h> on old systems */
#include <sys/queue.h>

#include <freeswan.h>

#include <pfkeyv2.h>
#include <pfkey.h>

#include "constants.h"
#include "defs.h"
#include "id.h"
#include "ca.h"
#include "certs.h"
#include "ac.h"
#include "connections.h"
#include "foodgroups.h"
#include "packet.h"
#include "demux.h"	/* needs packet.h */
#include "server.h"
#include "kernel.h"
#include "log.h"
#include "keys.h"
#include "adns.h"	/* needs <resolv.h> */
#include "dnskey.h"	/* needs keys.h and adns.h */
#include "rnd.h"
#include "state.h"
#include "ipsec_doi.h"	/* needs demux.h and state.h */
#include "ocsp.h"
#include "crl.h"
#include "fetch.h"
#include "sha1.h"
#include "md5.h"
#include "crypto.h"	/* requires sha1.h and md5.h */
#include "nat_traversal.h"
#include "virtual.h"

static void
usage(const char *mess)
{
    if (mess != NULL && *mess != '\0')
	fprintf(stderr, "%s\n", mess);
    fprintf(stderr
	, "Usage: pluto"
	    " [--help]"
	    " [--version]"
	    " [--optionsfrom <filename>]"
	    " \\\n\t"
	    "[--nofork]"
	    " [--stderrlog]"
	    " [--noklips]"
	    " [--nocrsend]"
	    " \\\n\t"
	    "[--strictcrlpolicy]"
	    " [--crlcheckinterval]"
	    " [--cachecrls]"
	    " [--uniqueids]"
	    " \\\n\t"
	    "[--interface <ifname>]"
	    " [--ikeport <port-number>]"
	    " \\\n\t"
	    "[--ctlbase <path>]"
	    " \\\n\t"
	    "[--perpeerlogbase <path>] [--perpeerlog]"
	    " \\\n\t"
	    "[--secretsfile <secrets-file>]"
	    " [--policygroupsdir <policygroups-dir>]"
	    " \\\n\t"
	    "[--adns <pathname>]"
	    "[--pkcs11module <path>]"
	    "[--pkcs11keepstate"
#ifdef DEBUG
	    " \\\n\t"
	    "[--debug-none]"
	    " [--debug-all]"
	    " \\\n\t"
	    "[--debug-raw]"
	    " [--debug-crypt]"
	    " [--debug-parsing]"
	    " [--debug-emitting]"
	    " \\\n\t"
	    "[--debug-control]"
	    " [--debug-lifecycle]"
	    " [--debug-klips]"
	    " [--debug-dns]"
	    " \\\n\t"
	    "[--debug-oppo]"
	    " [--debug-controlmore]"
	    " [--debug-private]"
#endif
	    " [ --debug-natt]"
	    " \\\n\t"
	    "[--nat_traversal] [--keep_alive <delay_sec>]"
	    " \\\n\t"
	    "[--force_keepalive] [--disable_port_floating]"
	   " \\\n\t"
	   "[--virtual_private <network_list>]"
	    "\n"
	"strongSwan %s\n"
	, ipsec_version_code());
    exit_pluto(mess == NULL? 0 : 1);
}


/* lock file support
 * - provides convenient way for scripts to find Pluto's pid
 * - prevents multiple Plutos competing for the same port
 * - same basename as unix domain control socket
 * NOTE: will not take account of sharing LOCK_DIR with other systems.
 */

static char pluto_lock[sizeof(ctl_addr.sun_path)] = DEFAULT_CTLBASE LOCK_SUFFIX;
static bool pluto_lock_created = FALSE;

/* create lockfile, or die in the attempt */
static int
create_lock(void)
{
    int fd = open(pluto_lock, O_WRONLY | O_CREAT | O_EXCL | O_TRUNC
	, S_IRUSR | S_IRGRP | S_IROTH);

    if (fd < 0)
    {
	if (errno == EEXIST)
	{
	    fprintf(stderr, "pluto: lock file \'%s\' already exists\n"
		, pluto_lock);
	    exit_pluto(10);
	}
	else
	{
	    fprintf(stderr
		, "pluto: unable to create lock file \'%s\' (%d %s)\n"
		, pluto_lock, errno, strerror(errno));
	    exit_pluto(1);
	}
    }
    pluto_lock_created = TRUE;
    return fd;
}

static bool
fill_lock(int lockfd, pid_t pid)
{
    char buf[30];	/* holds "<pid>\n" */
    int len = snprintf(buf, sizeof(buf), "%u\n", (unsigned int) pid);
    bool ok = len > 0 && write(lockfd, buf, len) == len;

    close(lockfd);
    return ok;
}

static void
delete_lock(void)
{
    if (pluto_lock_created)
    {
	delete_ctl_socket();
	unlink(pluto_lock);	/* is noting failure useful? */
    }
}

/* by default pluto sends certificate requests to its peers */
bool no_cr_send = FALSE;

/* by default the CRL policy is lenient */
bool strict_crl_policy = FALSE;

/* by default CRLs are cached locally as files */
bool cache_crls = FALSE;

/* by default pluto does not check crls dynamically */
long crl_check_interval = 0;

/* path to the PKCS#11 module */
char *pkcs11_module_path = NULL;

/* by default pluto logs out after every smartcard use */
bool pkcs11_keep_state = FALSE;

/* by default pluto does not allow pkcs11 proxy access via whack */
bool pkcs11_proxy = FALSE;

/* argument string to pass to PKCS#11 module.
 * Not used for compliant modules, just for NSS softoken
 */
static const char *pkcs11_init_args = NULL;

int
main(int argc, char **argv)
{
    bool fork_desired = TRUE;
    bool log_to_stderr_desired = FALSE;
    bool nat_traversal = FALSE;
    bool nat_t_spf = TRUE;  /* support port floating */
    unsigned int keep_alive = 0;
    bool force_keepalive = FALSE;
    char *virtual_private = NULL;
    int lockfd;

    /* handle arguments */
    for (;;)
    {
#	define DBG_OFFSET 256
	static const struct option long_opts[] = {
	    /* name, has_arg, flag, val */
	    { "help", no_argument, NULL, 'h' },
	    { "version", no_argument, NULL, 'v' },
	    { "optionsfrom", required_argument, NULL, '+' },
	    { "nofork", no_argument, NULL, 'd' },
	    { "stderrlog", no_argument, NULL, 'e' },
	    { "noklips", no_argument, NULL, 'n' },
	    { "nocrsend", no_argument, NULL, 'c' },
	    { "strictcrlpolicy", no_argument, NULL, 'r' },
	    { "crlcheckinterval", required_argument, NULL, 'x'},
	    { "cachecrls", no_argument, NULL, 'C' },
	    { "uniqueids", no_argument, NULL, 'u' },
	    { "interface", required_argument, NULL, 'i' },
	    { "ikeport", required_argument, NULL, 'p' },
	    { "ctlbase", required_argument, NULL, 'b' },
	    { "secretsfile", required_argument, NULL, 's' },
	    { "foodgroupsdir", required_argument, NULL, 'f' },
	    { "perpeerlogbase", required_argument, NULL, 'P' },
	    { "perpeerlog", no_argument, NULL, 'l' },
	    { "policygroupsdir", required_argument, NULL, 'f' },
#ifdef USE_LWRES
	    { "lwdnsq", required_argument, NULL, 'a' },
#else /* !USE_LWRES */
	    { "adns", required_argument, NULL, 'a' },
#endif /* !USE_LWRES */
	    { "pkcs11module", required_argument, NULL, 'm' },
	    { "pkcs11keepstate", no_argument, NULL, 'k' },
	    { "pkcs11proxy", no_argument, NULL, 'y' },
	    { "nat_traversal", no_argument, NULL, '1' },
	    { "keep_alive", required_argument, NULL, '2' },
	    { "force_keepalive", no_argument, NULL, '3' },
	    { "disable_port_floating", no_argument, NULL, '4' },
	    { "debug-natt", no_argument, NULL, '5' },
	    { "virtual_private", required_argument, NULL, '6' },
#ifdef DEBUG
	    { "debug-none", no_argument, NULL, 'N' },
	    { "debug-all", no_argument, NULL, 'A' },
	    { "debug-raw", no_argument, NULL, DBG_RAW + DBG_OFFSET },
	    { "debug-crypt", no_argument, NULL, DBG_CRYPT + DBG_OFFSET },
	    { "debug-parsing", no_argument, NULL, DBG_PARSING + DBG_OFFSET },
	    { "debug-emitting", no_argument, NULL, DBG_EMITTING + DBG_OFFSET },
	    { "debug-control", no_argument, NULL, DBG_CONTROL + DBG_OFFSET },
	    { "debug-lifecycle", no_argument, NULL, DBG_LIFECYCLE + DBG_OFFSET },
	    { "debug-klips", no_argument, NULL, DBG_KLIPS + DBG_OFFSET },
	    { "debug-dns", no_argument, NULL, DBG_DNS + DBG_OFFSET },
	    { "debug-oppo", no_argument, NULL, DBG_OPPO + DBG_OFFSET },
	    { "debug-controlmore", no_argument, NULL, DBG_CONTROLMORE + DBG_OFFSET },
	    { "debug-private", no_argument, NULL, DBG_PRIVATE + DBG_OFFSET },

	    { "impair-delay-adns-key-answer", no_argument, NULL, IMPAIR_DELAY_ADNS_KEY_ANSWER + DBG_OFFSET },
	    { "impair-delay-adns-txt-answer", no_argument, NULL, IMPAIR_DELAY_ADNS_TXT_ANSWER + DBG_OFFSET },
	    { "impair-bust-mi2", no_argument, NULL, IMPAIR_BUST_MI2 + DBG_OFFSET },
	    { "impair-bust-mr2", no_argument, NULL, IMPAIR_BUST_MR2 + DBG_OFFSET },
#endif
	    { 0,0,0,0 }
	    };
	/* Note: we don't like the way short options get parsed
	 * by getopt_long, so we simply pass an empty string as
	 * the list.  It could be "hvdenp:l:s:" "NARXPECK".
	 */
	int c = getopt_long(argc, argv, "", long_opts, NULL);

	/* Note: "breaking" from case terminates loop */
	switch (c)
	{
	case EOF:	/* end of flags */
	    break;

	case 0: /* long option already handled */
	    continue;

	case ':':	/* diagnostic already printed by getopt_long */
	case '?':	/* diagnostic already printed by getopt_long */
	    usage("");
	    break;   /* not actually reached */

	case 'h':	/* --help */
	    usage(NULL);
	    break;	/* not actually reached */

	case 'v':	/* --version */
	    {
		const char **sp = ipsec_copyright_notice();

		printf("%s%s\n", ipsec_version_string(),
				 compile_time_interop_options);
		for (; *sp != NULL; sp++)
		    puts(*sp);
	    }
	    exit_pluto(0);
	    break;	/* not actually reached */

	case '+':	/* --optionsfrom <filename> */
	    optionsfrom(optarg, &argc, &argv, optind, stderr);
	    /* does not return on error */
	    continue;

	case 'd':	/* --nofork*/
	    fork_desired = FALSE;
	    continue;

	case 'e':	/* --stderrlog */
	    log_to_stderr_desired = TRUE;
	    continue;

	case 'n':	/* --noklips */
	    no_klips = TRUE;
	    continue;

	case 'c':	/* --nocrsend */
	    no_cr_send = TRUE;
	    continue;

	case 'r':	/* --strictcrlpolicy */
	    strict_crl_policy = TRUE;
	    continue;

	case 'x':	/* --crlcheckinterval <time>*/
            if (optarg == NULL || !isdigit(optarg[0]))
                usage("missing interval time");

            {
                char *endptr;
                long interval = strtol(optarg, &endptr, 0);

                if (*endptr != '\0' || endptr == optarg
                || interval <= 0)
                    usage("<interval-time> must be a positive number");
                crl_check_interval = interval;
            }
	    continue;

	case 'C':	/* --cachecrls */
	    cache_crls = TRUE;
	    continue;

	case 'u':	/* --uniqueids */
	    uniqueIDs = TRUE;
	    continue;

	case 'i':	/* --interface <ifname> */
	    if (!use_interface(optarg))
		usage("too many --interface specifications");
	    continue;

	case 'p':	/* --port <portnumber> */
	    if (optarg == NULL || !isdigit(optarg[0]))
		usage("missing port number");

	    {
		char *endptr;
		long port = strtol(optarg, &endptr, 0);

		if (*endptr != '\0' || endptr == optarg
		|| port <= 0 || port > 0x10000)
		    usage("<port-number> must be a number between 1 and 65535");
		pluto_port = port;
	    }
	    continue;

	case 'b':	/* --ctlbase <path> */
	    if (snprintf(ctl_addr.sun_path, sizeof(ctl_addr.sun_path)
	    , "%s%s", optarg, CTL_SUFFIX) == -1)
		usage("<path>" CTL_SUFFIX " too long for sun_path");
	    if (snprintf(info_addr.sun_path, sizeof(info_addr.sun_path)
	    , "%s%s", optarg, INFO_SUFFIX) == -1)
		usage("<path>" INFO_SUFFIX " too long for sun_path");
	    if (snprintf(pluto_lock, sizeof(pluto_lock)
	    , "%s%s", optarg, LOCK_SUFFIX) == -1)
		usage("<path>" LOCK_SUFFIX " must fit");
	    continue;

	case 's':	/* --secretsfile <secrets-file> */
	    shared_secrets_file = optarg;
	    continue;

	case 'f':	/* --policygroupsdir <policygroups-dir> */
	    policygroups_dir = optarg;
	    continue;

	case 'a':	/* --adns <pathname> */
	    pluto_adns_option = optarg;
	    continue;

	case 'm':	/* --pkcs11module <pathname> */
	    pkcs11_module_path = optarg;
	    continue;

	case 'k':	/* --pkcs11keepstate */
	    pkcs11_keep_state = TRUE;
	    continue;

	case 'y':	/* --pkcs11proxy */
	    pkcs11_proxy = TRUE;
	    continue;

#ifdef DEBUG
	case 'N':	/* --debug-none */
	    base_debugging = DBG_NONE;
	    continue;

	case 'A':	/* --debug-all */
	    base_debugging = DBG_ALL;
	    continue;
#endif

	case 'P':       /* --perpeerlogbase */
	    base_perpeer_logdir = optarg;
	    continue;

	case 'l':
	    log_to_perpeer = TRUE;
	    continue;

	case '1':	/* --nat_traversal */
	    nat_traversal = TRUE;
	    continue;
	case '2':	/* --keep_alive */
	    keep_alive = atoi(optarg);
	    continue;
	case '3':	/* --force_keepalive */
	    force_keepalive = TRUE;
	    continue;
	case '4':	/* --disable_port_floating */
	    nat_t_spf = FALSE;
	    continue;
	case '5':	/* --debug-nat_t */
	    base_debugging |= DBG_NATT;
	    continue;
	case '6':	/* --virtual_private */
	    virtual_private = optarg;
	    continue;

	default:
#ifdef DEBUG
	    if (c >= DBG_OFFSET)
	    {
		base_debugging |= c - DBG_OFFSET;
		continue;
	    }
#	undef DBG_OFFSET
#endif
	    bad_case(c);
	}
	break;
    }
    if (optind != argc)
	usage("unexpected argument");
    reset_debugging();
    lockfd = create_lock();

    /* select between logging methods */

    if (log_to_stderr_desired)
	log_to_syslog = FALSE;
    else
	log_to_stderr = FALSE;

    /* set the logging function of pfkey debugging */
#ifdef DEBUG
    pfkey_debug_func = DBG_log;
#else
    pfkey_debug_func = NULL;
#endif

	/* create control socket.
	 * We must create it before the parent process returns so that
	 * there will be no race condition in using it.  The easiest
	 * place to do this is before the daemon fork.
	 */
	{
		err_t ugh = init_ctl_socket();

		if (ugh != NULL)
		{
			fprintf(stderr, "pluto: %s", ugh);
			exit_pluto(1);
		}
	}

	/* If not suppressed, do daemon fork */

	if (fork_desired)
	{
		{
			pid_t pid = fork();

			if (pid < 0)
			{
				int e = errno;

				fprintf(stderr, "pluto: fork failed (%d %s)\n",
					errno, strerror(e));
				exit_pluto(1);
			}

			if (pid != 0)
			{
				/* parent: die, after filling PID into lock file.
				 * must not use exit_pluto: lock would be removed!
				 */
				exit(fill_lock(lockfd, pid)? 0 : 1);
			}
		}

		if (setsid() < 0)
		{
			int e = errno;

			fprintf(stderr, "setsid() failed in main(). Errno %d: %s\n",
				errno, strerror(e));
			exit_pluto(1);
		}
	}
	else
	{
		/* no daemon fork: we have to fill in lock file */
		(void) fill_lock(lockfd, getpid());
		fprintf(stdout, "Pluto initialized\n");
		fflush(stdout);
	}

	/* Redirect stdin, stdout and stderr to /dev/null
	 */
	{
		int fd;
		if ((fd = open("/dev/null", O_RDWR)) == -1)
			abort();
		if (dup2(fd, 0) != 0)
			abort();
		if (dup2(fd, 1) != 1)
			abort();
		if (!log_to_stderr && dup2(fd, 2) != 2)
			abort();
		close(fd);
	}

    init_constants();
    init_log("pluto");

    /* Note: some scripts may look for this exact message -- don't change
     * ipsec barf was one, but it no longer does.
     */
    plog("Starting Pluto (strongSwan Version %s%s)"
	, ipsec_version_code()
	, compile_time_interop_options);

    init_nat_traversal(nat_traversal, keep_alive, force_keepalive, nat_t_spf);
    init_virtual_ip(virtual_private);
    scx_init(pkcs11_module_path, pkcs11_init_args);   /* load and initialize PKCS #11 module */
    init_rnd_pool();
    init_secret();
    init_states();
    init_crypto();
    init_demux();
    init_kernel();
    init_adns();
    init_id();
    init_fetch();

    /* loading X.509 CA certificates */
    load_authcerts("CA cert", CA_CERT_PATH, AUTH_CA);
    /* loading X.509 AA certificates */
    load_authcerts("AA cert", AA_CERT_PATH, AUTH_AA);
    /* loading X.509 OCSP certificates */
    load_authcerts("OCSP cert", OCSP_CERT_PATH, AUTH_OCSP);
    /* loading X.509 CRLs */
    load_crls();
    /* loading attribute certificates (experimental) */
    load_acerts();

    daily_log_event();
    call_server();
    return -1;	/* Shouldn't ever reach this */
}

/* leave pluto, with status.
 * Once child is launched, parent must not exit this way because
 * the lock would be released.
 *
 *  0 OK
 *  1 general discomfort
 * 10 lock file exists
 */
void
exit_pluto(int status)
{
    reset_globals();	/* needed because we may be called in odd state */
    free_preshared_secrets();
    free_remembered_public_keys();
    delete_every_connection();
    free_crl_fetch();		/* free chain of crl fetch requests */
    free_ocsp_fetch();		/* free chain of ocsp fetch requests */
    free_authcerts();		/* free chain of X.509 authority certificates */
    free_crls();		/* free chain of X.509 CRLs */
    free_acerts();		/* free chain of X.509 attribute certificates */
    free_ca_infos();		/* free chain of X.509 CA information records */
    free_ocsp();		/* free ocsp cache */
    free_ifaces();
    scx_finalize();		/* finalize and unload PKCS #11 module */
    stop_adns();
    free_md_pool();
    free_id();  
    delete_lock();
#ifdef LEAK_DETECTIVE
    report_leaks();
#endif /* LEAK_DETECTIVE */
    close_log();
    exit(status);
}

/*
 * Local Variables:
 * c-basic-offset:4
 * c-style: pluto
 * End:
 */
