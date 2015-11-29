/* strongSwan Pluto launcher
 * Copyright (C) 2001-2002 Mathieu Lafon - Arkoon Network Security
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
 * RCSID $Id: invokepluto.c 619 2007-06-25 11:18:20Z encounter $
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <freeswan.h>

#include "../pluto/constants.h"
#include "../pluto/defs.h"
#include "../pluto/log.h"

#include "confread.h"
#include "invokepluto.h"
#include "files.h"
#include "starterwhack.h"
#
static int _pluto_pid = 0;
static int _stop_requested;

pid_t
starter_pluto_pid(void)
{
    return _pluto_pid;
}

void
starter_pluto_sigchild(pid_t pid)
{
    if (pid == _pluto_pid)
    {
	_pluto_pid = 0;
	if (!_stop_requested)
	{
	    plog("pluto has died -- restart scheduled (%dsec)"
		, PLUTO_RESTART_DELAY);
	    alarm(PLUTO_RESTART_DELAY);   // restart in 5 sec
	}
	unlink(PLUTO_PID_FILE);
    }
}

int
starter_stop_pluto (void)
{
    pid_t pid;
    int i;

    pid = _pluto_pid;
    if (pid)
    {
	_stop_requested = 1;
	if (starter_whack_shutdown() == 0)
	{
	    for (i = 0; i < 20; i++)
	    {
		usleep(20000);
		if (_pluto_pid == 0)
		    return 0;
	    }
	}
	/* be more and more aggressive */
	for (i = 0; i < 20 && (pid = _pluto_pid) != 0; i++)
	{
	    if (i < 10)
		kill(pid, SIGTERM);
	    else
		kill(pid, SIGKILL);
	    usleep(20000);
	}
	if (_pluto_pid == 0)
	    return 0;
	plog("starter_stop_pluto(): can't stop pluto !!!");
	return -1;
    }
    else
    {
	plog("stater_stop_pluto(): pluto is not started...");
    }
    return -1;
}

#define ADD_DEBUG(v) { \
	for (l = cfg->setup.plutodebug; l && *l; l++) if (streq(*l, v)) \
		arg[argc++] = "--debug-" v; \
	}

int
starter_start_pluto (starter_config_t *cfg, bool debug)
{
    int i;
    struct stat stb;
    pid_t pid;
    char **l;
    int argc = 2;
    char *arg[] = {
	      PLUTO_CMD, "--nofork"
	    , NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
	    , NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
	    , NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
	    , NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
	};

    printf ("starter_start_pluto entered\n");

    if (debug)
    {
	arg[argc++] = "--stderrlog";
    }
    if (cfg->setup.uniqueids)
    {
	arg[argc++] = "--uniqueids";
    }
    ADD_DEBUG("none")
    ADD_DEBUG("all")
    ADD_DEBUG("raw")
    ADD_DEBUG("crypt")
    ADD_DEBUG("parsing")
    ADD_DEBUG("emitting")
    ADD_DEBUG("control")
    ADD_DEBUG("lifecycle")
    ADD_DEBUG("klips")
    ADD_DEBUG("dns")
    ADD_DEBUG("natt")
    ADD_DEBUG("oppo")
    ADD_DEBUG("controlmore")
    ADD_DEBUG("private")
    if (cfg->setup.crlcheckinterval > 0)
    {
	static char buf1[15];

	arg[argc++] = "--crlcheckinterval";
	snprintf(buf1, sizeof(buf1), "%u", cfg->setup.crlcheckinterval);
	arg[argc++] = buf1;
    }
    if (cfg->setup.cachecrls)
    {
	arg[argc++] = "--cachecrls";
    }
    if (cfg->setup.strictcrlpolicy)
    {
	arg[argc++] = "--strictcrlpolicy";
    }
    if (cfg->setup.nocrsend)
    {
	arg[argc++] = "--nocrsend";
    }
    if (cfg->setup.nat_traversal)
    {
	arg[argc++] = "--nat_traversal";
    }
    if (cfg->setup.keep_alive)
    {
	static char buf2[15];

	arg[argc++] = "--keep_alive";
	snprintf(buf2, sizeof(buf2), "%u", cfg->setup.keep_alive);
	arg[argc++] = buf2;
    }
#ifdef VIRTUAL_IP
    if (cfg->setup.virtual_private)
    {
	arg[argc++] = "--virtual_private";
	arg[argc++] = cfg->setup.virtual_private;
    }
#endif
    if (cfg->setup.pkcs11module)
    {
	arg[argc++] = "--pkcs11module";
	arg[argc++] = cfg->setup.pkcs11module;
    }
    if (cfg->setup.pkcs11keepstate)
    {
	arg[argc++] = "--pkcs11keepstate";
    }
    if (cfg->setup.pkcs11proxy)
    {
	arg[argc++] = "--pkcs11proxy";
    }

    if (_pluto_pid)
    {
	plog("starter_start_pluto(): pluto already started...");
	return -1;
    }
    else
    {
	unlink(PLUTO_CTL_FILE);
	_stop_requested = 0;

	if (cfg->setup.prepluto)
	    system(cfg->setup.prepluto);

	/* if ipsec.secrets file is missing then generate RSA default key pair */
	if (stat(SECRETS_FILE, &stb) != 0)
	{
	    mode_t oldmask;
	    FILE *f;

	    plog("no %s file, generating RSA key", SECRETS_FILE);
	    system("ipsec scepclient --out pkcs1 --out cert-self --quiet");

	    /* ipsec.secrets is root readable only */
	    oldmask = umask(0066);

	    f = fopen(SECRETS_FILE, "w");
	    if (f)
	    {
		fprintf(f, "# /etc/ipsec.secrets - strongSwan IPsec secrets file\n");
		fprintf(f, "\n");
		fprintf(f, ": RSA myKey.der\n");
		fclose(f);
	    }
	    umask(oldmask);
	}

	pid = fork();
	switch (pid)
	{
	case -1:
	    plog("can't fork(): %s", strerror(errno));
	    return -1;
	case 0:
	    /* child */
	    setsid();
	    sigprocmask(SIG_SETMASK, 0, NULL);
	    execv(arg[0], arg);
	    plog("can't execv(%s,...): %s", arg[0], strerror(errno));
	    exit(1);
	default:
	    /* father */
	    _pluto_pid = pid;
	    for (i = 0; i < 50 && _pluto_pid; i++)
	    {
		/* wait for pluto */
		usleep(20000);
		if (stat(PLUTO_CTL_FILE, &stb) == 0)
		{
		    DBG(DBG_CONTROL,
			DBG_log("pluto (%d) started", _pluto_pid)
		    )
		    if (cfg->setup.postpluto)
			system(cfg->setup.postpluto);
		    return 0;
		}
	    }
	    if (_pluto_pid)
	    {
		/* If pluto is started but with no ctl file, stop it */
		plog("pluto too long to start... - kill kill");
		for (i = 0; i < 20 && (pid = _pluto_pid) != 0; i++)
		{
		    if (i < 10)
			kill(pid, SIGTERM);
		    else
			kill(pid, SIGKILL);
		    usleep(20000);
		}
	    }
	    else
	    {
		plog("pluto refused to be started");
	    }
	    return -1;
	}
    }
    return -1;
}
