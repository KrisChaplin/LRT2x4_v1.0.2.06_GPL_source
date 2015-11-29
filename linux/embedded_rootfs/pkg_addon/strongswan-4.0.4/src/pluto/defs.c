/* misc. universal things
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
 * RCSID $Id: defs.c 12119 2013-07-09 13:59:35Z dio.li $
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <freeswan.h>

#include "constants.h"
#include "defs.h"
#include "log.h"
#include "whack.h"	/* for RC_LOG_SERIOUS */

const chunk_t empty_chunk = { NULL, 0 };

bool
all_zero(const unsigned char *m, size_t len)
{
    size_t i;

    for (i = 0; i != len; i++)
	if (m[i] != '\0')
	    return FALSE;
    return TRUE;
}

/* memory allocation
 *
 * LEAK_DETECTIVE puts a wrapper around each allocation and maintains
 * a list of live ones.  If a dead one is freed, an assertion MIGHT fail.
 * If the live list is currupted, that will often be detected.
 * In the end, report_leaks() is called, and the names of remaining
 * live allocations are printed.  At the moment, it is hoped, not that
 * the list is empty, but that there will be no surprises.
 *
 * Accepted Leaks:
 * - "struct iface" and "device name" (for "discovered" net interfaces)
 * - "struct event in event_schedule()" (events not associated with states)
 * - "Pluto lock name" (one only, needed until end -- why bother?)
 */

#ifdef LEAK_DETECTIVE

/* this magic number is 3671129837 decimal (623837458 complemented) */
#define LEAK_MAGIC 0xDAD0FEEDul

union mhdr {
    struct {
	const char *name;
	union mhdr *older, *newer;
	unsigned long magic;
    } i;    /* info */
    unsigned long junk;	/* force maximal alignment */
};

static union mhdr *allocs = NULL;

void *alloc_bytes(size_t size, const char *name)
{
    union mhdr *p = malloc(sizeof(union mhdr) + size);

    if (p == NULL)
	exit_log("unable to malloc %lu bytes for %s"
	    , (unsigned long) size, name);
    p->i.name = name;
    p->i.older = allocs;
    if (allocs != NULL)
	allocs->i.newer = p;
    allocs = p;
    p->i.newer = NULL;
    p->i.magic = LEAK_MAGIC;

    memset(p+1, '\0', size);
    return p+1;
}

void *
clone_bytes(const void *orig, size_t size, const char *name)
{
    void *p = alloc_bytes(size, name);

    memcpy(p, orig, size);
    return p;
}

void
pfree(void *ptr)
{
    union mhdr *p;

    passert(ptr != NULL);
    p = ((union mhdr *)ptr) - 1;
    passert(p->i.magic == LEAK_MAGIC);
    if (p->i.older != NULL)
    {
	passert(p->i.older->i.newer == p);
	p->i.older->i.newer = p->i.newer;
    }
    if (p->i.newer == NULL)
    {
	passert(p == allocs);
	allocs = p->i.older;
    }
    else
    {
	passert(p->i.newer->i.older == p);
	p->i.newer->i.older = p->i.older;
    }
    p->i.magic = ~LEAK_MAGIC;
    free(p);
}

void
report_leaks(void)
{
    union mhdr
	*p = allocs,
	*pprev = NULL;
    unsigned long n = 0;

    while (p != NULL)
    {
	passert(p->i.magic == LEAK_MAGIC);
	passert(pprev == p->i.newer);
	pprev = p;
	p = p->i.older;
	n++;
	if (p == NULL || pprev->i.name != p->i.name)
	{
	    if (n != 1)
		plog("leak: %lu * %s", n, pprev->i.name);
	    else
		plog("leak: %s", pprev->i.name);
	    n = 0;
	}
    }
}

#else /* !LEAK_DETECTIVE */

void *alloc_bytes(size_t size, const char *name)
{
    void *p = malloc(size);

    if (p == NULL)
	exit_log("unable to malloc %lu bytes for %s"
	    , (unsigned long) size, name);
    memset(p, '\0', size);
    return p;
}

void *clone_bytes(const void *orig, size_t size, const char *name)
{
    void *p = malloc(size);

    if (p == NULL)
	exit_log("unable to malloc %lu bytes for %s"
	    , (unsigned long) size, name);
    memcpy(p, orig, size);
    return p;
}
#endif /* !LEAK_DETECTIVE */

/*  Note that there may be as many as six IDs that are temporary at
 *  one time before unsharing the two ends of a connection. So we need
 *  at least six temporary buffers for DER_ASN1_DN IDs.
 *  We rotate them. Be careful!
 */
#define	MAX_BUF		10

char*
temporary_cyclic_buffer(void)
{
    static char buf[MAX_BUF][BUF_LEN];	/* MAX_BUF internal buffers */
    static int counter = 0;			/* cyclic counter */

    if (++counter == MAX_BUF) counter = 0;	/* next internal buffer */
    return buf[counter];			/* assign temporary buffer */
}

/* concatenates two sub paths into a string with a maximum size of BUF_LEN
 * use for temporary storage only
 */
const char*
concatenate_paths(const char *a, const char *b)
{
    char *c;

    if (*b == '/' || *b == '.')
	return b;

    c = temporary_cyclic_buffer();
    snprintf(c, BUF_LEN, "%s/%s", a, b);
    return c;
}

/*  compare two chunks, returns zero if a equals b
 *  negative/positive if a is earlier/later in the alphabet than b
 */
int
cmp_chunk(chunk_t a, chunk_t b)
{
    int cmp_len, len, cmp_value;
    
    cmp_len = a.len - b.len;
    len = (cmp_len < 0)? a.len : b.len;
    cmp_value = memcmp(a.ptr, b.ptr, len);

    return (cmp_value == 0)? cmp_len : cmp_value;
};

/* moves a chunk to a memory position, chunk is freed afterwards
 * position pointer is advanced after the insertion point
 */
void
mv_chunk(u_char **pos, chunk_t content)
{
    if (content.len > 0)
    {
	chunkcpy(*pos, content);
	freeanychunk(content);
    }
}

/*
 * write the binary contents of a chunk_t to a file
 */
bool
write_chunk(const char *filename, const char *label, chunk_t ch
, mode_t mask, bool force)
{
    mode_t oldmask;
    FILE *fd;
    size_t written;

    if (!force)
    {
	fd = fopen(filename, "r");
	if (fd)
	{
	    fclose(fd);
	    plog("  %s file '%s' already exists", label, filename);
	    return FALSE;
	}
    }

    /* set umask */
    oldmask = umask(mask);

    fd = fopen(filename, "w");

    if (fd)
    {
	written = fwrite(ch.ptr, sizeof(u_char), ch.len, fd);
	fclose(fd);
	if (written != ch.len)
	{
	    plog("  writing to %s file (%s) failed", label, filename);
	    umask(oldmask);
	    return FALSE;
	}
	plog("  written %s file '%s' (%d bytes)", label, filename, (int)ch.len);
	umask(oldmask);
	return TRUE;
    }
    else
    {
	plog("  could not open %s file '%s' for writing", label, filename);
	umask(oldmask);
	return FALSE;
    }
}

/* Names of the months */

static const char* months[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};


/*
 *  Display a date either in local or UTC time
 */
char*
timetoa(const time_t *time, bool utc)
{
    static char buf[TIMETOA_BUF];

    if (*time == UNDEFINED_TIME)
	sprintf(buf, "--- -- --:--:--%s----", (utc)?" UTC ":" ");
    else
    {
	struct tm *t = (utc)? gmtime(time) : localtime(time);

	sprintf(buf, "%s %02d %02d:%02d:%02d%s%04d",
	    months[t->tm_mon], t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec,
	    (utc)?" UTC ":" ", t->tm_year + 1900
	);
    }
    return buf;
}

/*  checks if the expiration date has been reached and
 *  warns during the warning_interval of the imminent
 *  expiry. strict=TRUE declares a fatal error,
 *  strict=FALSE issues a warning upon expiry.
 */
const char*
check_expiry(time_t expiration_date, int warning_interval, bool strict)
{
    time_t now;
    int time_left;

    if (expiration_date == UNDEFINED_TIME)
      return "ok (expires never)";

    /* determine the current time */
    time(&now);

    time_left = (expiration_date - now);
    if (time_left < 0)
	return strict? "fatal (expired)" : "warning (expired)";

    if (time_left > 86400*warning_interval)
	return "ok";
    {
	static char buf[35]; /* temporary storage */
	const char* unit = "second";

	if (time_left > 172800)
	{
	    time_left /= 86400;
	    unit = "day";
	}
	else if (time_left > 7200)
	{
	    time_left /= 3600;
	    unit = "hour";
	}
	else if (time_left > 120)
	{
	    time_left /= 60;
	    unit = "minute";
	}
	snprintf(buf, 35, "warning (expires in %d %s%s)", time_left,
		 unit, (time_left == 1)?"":"s");
	return buf;
    }
}


/*
 *  Filter eliminating the directory entries '.' and '..'
 */
int
file_select(const struct dirent *entry)
{
    return strcmp(entry->d_name, "." ) &&
	   strcmp(entry->d_name, "..");
}


