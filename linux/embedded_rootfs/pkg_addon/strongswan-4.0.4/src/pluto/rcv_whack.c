/* whack communicating routines
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
 * RCSID $Id: rcv_whack.c 12119 2013-07-09 13:59:35Z dio.li $
 */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <resolv.h>
#include <arpa/nameser.h>	/* missing from <resolv.h> on old systems */
#include <sys/queue.h>
#include <fcntl.h>

#include <freeswan.h>

#include "constants.h"
#include "defs.h"
#include "id.h"
#include "ca.h"
#include "certs.h"
#include "ac.h"
#include "smartcard.h"
#include "connections.h"
#include "foodgroups.h"
#include "whack.h"	/* needs connections.h */
#include "packet.h"
#include "demux.h"	/* needs packet.h */
#include "state.h"
#include "ipsec_doi.h"	/* needs demux.h and state.h */
#include "kernel.h"
#include "rcv_whack.h"
#include "log.h"
#include "keys.h"
#include "adns.h"	/* needs <resolv.h> */
#include "dnskey.h"	/* needs keys.h and adns.h */
#include "server.h"
#include "fetch.h"
#include "ocsp.h"
#include "crl.h"

#include "kernel_alg.h"
#include "ike_alg.h"
/* helper variables and function to decode strings from whack message */

static char *next_str
    , *str_roof;

static bool
unpack_str(char **p)
{
    char *end = memchr(next_str, '\0', str_roof - next_str);

    if (end == NULL)
    {
	return FALSE;	/* fishy: no end found */
    }
    else
    {
	*p = next_str == end? NULL : next_str;
	next_str = end + 1;
	return TRUE;
    }
}

/* bits loading keys from asynchronous DNS */

enum key_add_attempt {
    ka_TXT,
#ifdef USE_KEYRR
    ka_KEY,
#endif
    ka_roof	/* largest value + 1 */
};

struct key_add_common {
    int refCount;
    char *diag[ka_roof];
    int whack_fd;
    bool success;
};

struct key_add_continuation {
    struct adns_continuation ac;	/* common prefix */
    struct key_add_common *common;	/* common data */
    enum key_add_attempt lookingfor;
};

static void
key_add_ugh(const struct id *keyid, err_t ugh)
{
    char name[BUF_LEN];	/* longer IDs will be truncated in message */

    (void)idtoa(keyid, name, sizeof(name));
    loglog(RC_NOKEY
	, "failure to fetch key for %s from DNS: %s", name, ugh);
}

/* last one out: turn out the lights */
static void
key_add_merge(struct key_add_common *oc, const struct id *keyid)
{
    if (oc->refCount == 0)
    {
	enum key_add_attempt kaa;

	/* if no success, print all diagnostics */
	if (!oc->success)
	    for (kaa = ka_TXT; kaa != ka_roof; kaa++)
		key_add_ugh(keyid, oc->diag[kaa]);

	for (kaa = ka_TXT; kaa != ka_roof; kaa++)
	    pfreeany(oc->diag[kaa]);

	close(oc->whack_fd);
	pfree(oc);
    }
}

static void
key_add_continue(struct adns_continuation *ac, err_t ugh)
{
    struct key_add_continuation *kc = (void *) ac;
    struct key_add_common *oc = kc->common;

    passert(whack_log_fd == NULL_FD);
    whack_log_fd = oc->whack_fd;

    if (ugh != NULL)
    {
	oc->diag[kc->lookingfor] = clone_str(ugh, "key add error");
    }
    else
    {
	oc->success = TRUE;
	transfer_to_public_keys(kc->ac.gateways_from_dns
#ifdef USE_KEYRR
	    , &kc->ac.keys_from_dns
#endif /* USE_KEYRR */
	    );
    }

    oc->refCount--;
    key_add_merge(oc, &ac->id);
    whack_log_fd = NULL_FD;
}

static void
key_add_request(const whack_message_t *msg)
{
    struct id keyid;
    err_t ugh = atoid(msg->keyid, &keyid, FALSE);

    if (ugh != NULL)
    {
	loglog(RC_BADID, "bad --keyid \'%s\': %s", msg->keyid, ugh);
    }
    else
    {
	if (!msg->whack_addkey)
	    delete_public_keys(&keyid, msg->pubkey_alg
		, empty_chunk, empty_chunk);

	if (msg->keyval.len == 0)
	{
	    struct key_add_common *oc
		= alloc_thing(struct key_add_common
			      , "key add common things");
	    enum key_add_attempt kaa;

	    /* initialize state shared by queries */
	    oc->refCount = 0;
	    oc->whack_fd = dup_any(whack_log_fd);
	    oc->success = FALSE;

	    for (kaa = ka_TXT; kaa != ka_roof; kaa++)
	    {
		struct key_add_continuation *kc
		    = alloc_thing(struct key_add_continuation
			, "key add continuation");

		oc->diag[kaa] = NULL;
		oc->refCount++;
		kc->common = oc;
		kc->lookingfor = kaa;
		switch (kaa)
		{
		case ka_TXT:
		    ugh = start_adns_query(&keyid
			, &keyid	/* same */
			, T_TXT
			, key_add_continue
			, &kc->ac);
		    break;
#ifdef USE_KEYRR
		case ka_KEY:
		    ugh = start_adns_query(&keyid
			, NULL
			, T_KEY
			, key_add_continue
			, &kc->ac);
		    break;
#endif /* USE_KEYRR */
		default:
		    bad_case(kaa);	/* suppress gcc warning */
		}
		if (ugh != NULL)
		{
		    oc->diag[kaa] = clone_str(ugh, "early key add failure");
		    oc->refCount--;
		}
	    }

	    /* Done launching queries.
	     * Handle total failure case.
	     */
	    key_add_merge(oc, &keyid);
	}
	else
	{
	    ugh = add_public_key(&keyid, DAL_LOCAL, msg->pubkey_alg
		, &msg->keyval, &pubkeys);
	    if (ugh != NULL)
		loglog(RC_LOG_SERIOUS, "%s", ugh);
	}
    }
}

/* Handle a kernel request. Supposedly, there's a message in
 * the kernelsock socket.
 */
void
whack_handle(int whackctlfd)
{
    whack_message_t msg;
    struct sockaddr_un whackaddr;
    int whackaddrlen = sizeof(whackaddr);
    int whackfd = accept(whackctlfd, (struct sockaddr *)&whackaddr, &whackaddrlen);
    /* Note: actual value in n should fit in int.  To print, cast to int. */
    ssize_t n;

    if (whackfd < 0)
    {
	log_errno((e, "accept() failed in whack_handle()"));
	return;
    }
    if (fcntl(whackfd, F_SETFD, FD_CLOEXEC) < 0)
    {
	log_errno((e, "failed to set CLOEXEC in whack_handle()"));
	close(whackfd);
	return;
    }

    n = read(whackfd, &msg, sizeof(msg));

    if (n == -1)
    {
	log_errno((e, "read() failed in whack_handle()"));
	close(whackfd);
	return;
    }

    whack_log_fd = whackfd;

    /* sanity check message */
    {
	err_t ugh = NULL;

	next_str = msg.string;
	str_roof = (char *)&msg + n;

	if ((size_t)n < offsetof(whack_message_t, whack_shutdown) + sizeof(msg.whack_shutdown))
	{
	    ugh = builddiag("ignoring runt message from whack: got %d bytes", (int)n);
	}
	else if (msg.magic != WHACK_MAGIC)
	{
	    if (msg.magic == WHACK_BASIC_MAGIC)
	    {
		/* Only shutdown command.  Simpler inter-version compatability. */
		if (msg.whack_shutdown)
		{
		    plog("shutting down");
		    exit_pluto(0);	/* delete lock and leave, with 0 status */
		}
		ugh = "";	/* bail early, but without complaint */
	    }
	    else
	    {
		ugh = builddiag("ignoring message from whack with bad magic %d; should be %d; probably wrong version"
		    , msg.magic, WHACK_MAGIC);
	    }
	}
	else if (next_str > str_roof)
	{
	    ugh = builddiag("ignoring truncated message from whack: got %d bytes; expected %u"
		, (int) n, (unsigned) sizeof(msg));
	}
	else if (!unpack_str(&msg.name)		/* string  1 */
	|| !unpack_str(&msg.left.id)		/* string  2 */
	|| !unpack_str(&msg.left.cert)		/* string  3 */
	|| !unpack_str(&msg.left.ca)		/* string  4 */
	|| !unpack_str(&msg.left.groups)	/* string  5 */
	|| !unpack_str(&msg.left.updown)	/* string  6 */
	|| !unpack_str(&msg.left.virt)		/* string  7 */
	|| !unpack_str(&msg.right.id)		/* string  8 */
	|| !unpack_str(&msg.right.cert)		/* string  9 */
	|| !unpack_str(&msg.right.ca)		/* string 10 */
	|| !unpack_str(&msg.right.groups)	/* string 11 */
	|| !unpack_str(&msg.right.updown)	/* string 12 */
	|| !unpack_str(&msg.right.virt)		/* string 13 */
	|| !unpack_str(&msg.keyid)		/* string 14 */
	|| !unpack_str(&msg.myid)		/* string 15 */
	|| !unpack_str(&msg.cacert)		/* string 16 */
	|| !unpack_str(&msg.ldaphost)		/* string 17 */
	|| !unpack_str(&msg.ldapbase)		/* string 18 */
	|| !unpack_str(&msg.crluri)		/* string 19 */
	|| !unpack_str(&msg.crluri2)		/* string 20 */
	|| !unpack_str(&msg.ocspuri)		/* string 21 */
	|| !unpack_str(&msg.ike)		/* string 22 */
	|| !unpack_str(&msg.auth_alg)			/* string addnew:support authenticate algorithem*/
	|| !unpack_str(&msg.esp)		/* string 23 */
	|| !unpack_str(&msg.sc_data)		/* string 24 */
	|| !unpack_str(&msg.pskey)		//20100126 trenchen : support preshare key get from whack ipc
	|| str_roof - next_str != (ptrdiff_t)msg.keyval.len)	/* check chunk */
	{
	    ugh = "message from whack contains bad string";
	}
	else
	{
	    msg.keyval.ptr = next_str;	/* grab chunk */
	}

	if (ugh != NULL)
	{
	    if (*ugh != '\0')
		loglog(RC_BADWHACKMESSAGE, "%s", ugh);
	    whack_log_fd = NULL_FD;
	    close(whackfd);
	    return;
	}
    }

    if (msg.whack_options)
    {
#ifdef DEBUG
	if (msg.name == NULL)
	{
	    /* we do a two-step so that if either old or new would
	     * cause the message to print, it will be printed.
	     */
	    cur_debugging |= msg.debugging;
	    DBG(DBG_CONTROL
		, DBG_log("base debugging = %s"
		    , bitnamesof(debug_bit_names, msg.debugging)));
	    cur_debugging = base_debugging = msg.debugging;
	}
	else if (!msg.whack_connection)
	{
	    struct connection *c = con_by_name(msg.name, TRUE);

	    if (c != NULL)
	    {
		c->extra_debugging = msg.debugging;
		DBG(DBG_CONTROL
		    , DBG_log("\'%s\' extra_debugging = %s"
			, c->name
			, bitnamesof(debug_bit_names, c->extra_debugging)));
	    }
	}
#endif
    }

    if (msg.whack_myid)
	set_myid(MYID_SPECIFIED, msg.myid);

    /* Deleting combined with adding a connection works as replace.
     * To make this more useful, in only this combination,
     * delete will silently ignore the lack of the connection.
     */
    if (msg.whack_delete)
    {
    	if (msg.whack_ca)
	    find_ca_info_by_name(msg.name, TRUE);
	else
	    delete_connections_by_name(msg.name, !msg.whack_connection);
    }

    if (msg.whack_deletestate)
    {
	struct state *st = state_with_serialno(msg.whack_deletestateno);

	if (st == NULL)
	{
	    loglog(RC_UNKNOWN_NAME, "no state #%lu to delete"
		, msg.whack_deletestateno);
	}
	else
	{
	    delete_state(st);
	}
    }

    if (msg.whack_crash)
	delete_states_by_peer(&msg.whack_crash_peer);

    if (msg.whack_connection)
	add_connection(&msg);

    if (msg.whack_ca && msg.cacert != NULL)
	add_ca_info(&msg);
	
    /* process "listen" before any operation that could require it */
    if (msg.whack_listen)
    {
	close_peerlog();    /* close any open per-peer logs */
#ifdef more_log 	
	plog("listening for IKE messages");
#endif
	listening = TRUE;
	daily_log_reset();
	reset_adns_restart_count();
	set_myFQDN();
	find_ifaces();
	load_preshared_secrets(NULL_FD);
	load_groups();
    }
    if (msg.whack_unlisten)
    {
#ifdef more_log    
	plog("no longer listening for IKE messages");
#endif
	listening = FALSE;
    }

    if (msg.whack_reread & REREAD_SECRETS)
    {
	load_preshared_secrets(whackfd);
    }

    if (msg.whack_reread & REREAD_CACERTS)
    {
	load_authcerts("CA cert", CA_CERT_PATH, AUTH_CA);
    }

    if (msg.whack_reread & REREAD_AACERTS)
    {
	load_authcerts("AA cert", AA_CERT_PATH, AUTH_AA);
    }

    if (msg.whack_reread & REREAD_OCSPCERTS)
    {
	load_authcerts("OCSP cert", OCSP_CERT_PATH, AUTH_OCSP);
    }

    if (msg.whack_reread & REREAD_ACERTS)
    {
	load_acerts();
    }

    if (msg.whack_reread & REREAD_CRLS)
    {
	load_crls();
    }

    if (msg.whack_purgeocsp)
    {
	free_ocsp_fetch();
	free_ocsp_cache();
    }

    if (msg.whack_list & LIST_ALGS)
    {
	ike_alg_list();
	kernel_alg_list();
    }
   if (msg.whack_list & LIST_PUBKEYS)
    {
	list_public_keys(msg.whack_utc);
    }

    if (msg.whack_list & LIST_CERTS)
    {
	list_certs(msg.whack_utc);
    }

    if (msg.whack_list & LIST_CACERTS)
    {
	list_authcerts("CA", AUTH_CA, msg.whack_utc);
    }

    if (msg.whack_list & LIST_AACERTS)
    {
	list_authcerts("AA", AUTH_AA, msg.whack_utc);
    }

    if (msg.whack_list & LIST_OCSPCERTS)
    {
	list_authcerts("OCSP", AUTH_OCSP, msg.whack_utc);
    }

    if (msg.whack_list & LIST_ACERTS)
    {
	list_acerts(msg.whack_utc);
    }

    if (msg.whack_list & LIST_GROUPS)
    {
	list_groups(msg.whack_utc);
    }

    if (msg.whack_list & LIST_CAINFOS)
    {
	list_ca_infos(msg.whack_utc);
    }

    if (msg.whack_list & LIST_CRLS)
    {
	list_crls(msg.whack_utc, strict_crl_policy);
	list_crl_fetch_requests(msg.whack_utc);
    }

    if (msg.whack_list & LIST_OCSP)
    {
	list_ocsp_cache(msg.whack_utc, strict_crl_policy);
	list_ocsp_fetch_requests(msg.whack_utc);
    }

    if (msg.whack_list & LIST_CARDS)
    {
	scx_list(msg.whack_utc);
    }

    if (msg.whack_key)
    {
	/* add a public key */
	key_add_request(&msg);
    }

    if (msg.whack_route)
    {
	if (!listening)
	{
	    whack_log(RC_DEAF, "need --listen before --route");
	}
	if (msg.name == NULL)
	{
	    whack_log(RC_UNKNOWN_NAME
		, "whack --route requires a connection name");
	}
	else
	{
	    struct connection *c = con_by_name(msg.name, TRUE);

	    if (c != NULL && c->ikev1)
	    {
		set_cur_connection(c);
		if (!oriented(*c))
		    whack_log(RC_ORIENT
			, "we have no ipsecN interface for either end of this connection");
		else if (c->policy & POLICY_GROUP)
		    route_group(c);
		else if (!trap_connection(c))
		    whack_log(RC_ROUTE, "could not route");
		reset_cur_connection();
	    }
	}
    }

    if (msg.whack_unroute)
    {
	if (msg.name == NULL)
	{
	    whack_log(RC_UNKNOWN_NAME
		, "whack --unroute requires a connection name");
	}
	else
	{
	    struct connection *c = con_by_name(msg.name, TRUE);

	    if (c != NULL && c->ikev1)
	    {
		struct spd_route *sr;
		int fail = 0;

		set_cur_connection(c);

		for (sr = &c->spd; sr != NULL; sr = sr->next)
		{
		    if (sr->routing >= RT_ROUTED_TUNNEL)
			fail++;
		}
		if (fail > 0)
		    whack_log(RC_RTBUSY, "cannot unroute: route busy");
		else if (c->policy & POLICY_GROUP)
		    unroute_group(c);
		else
		    unroute_connection(c);
		reset_cur_connection();
	    }
	}
    }

    if (msg.whack_initiate)
    {
	if (!listening)
	{
	    whack_log(RC_DEAF, "need --listen before --initiate");
	}
	else if (msg.name == NULL)
	{
	    whack_log(RC_UNKNOWN_NAME
		, "whack --initiate requires a connection name");
	}
	else
	{
	    initiate_connection(msg.name
		, msg.whack_async? NULL_FD : dup_any(whackfd));
	}
    }

    if (msg.whack_oppo_initiate)
    {
	if (!listening)
	    whack_log(RC_DEAF, "need --listen before opportunistic initiation");
	else
	    initiate_opportunistic(&msg.oppo_my_client, &msg.oppo_peer_client, 0
		, FALSE
		, msg.whack_async? NULL_FD : dup_any(whackfd));
    }

    if (msg.whack_terminate)
    {
	if (msg.name == NULL)
	{
	    whack_log(RC_UNKNOWN_NAME
		, "whack --terminate requires a connection name");
	}
	else
	{
	    terminate_connection(msg.name);
	}
    }

    if (msg.whack_status)
	show_status(msg.whack_statusall, msg.name);

    if (msg.whack_shutdown)
    {
	plog("shutting down");
	exit_pluto(0);	/* delete lock and leave, with 0 status */
    }

    if (msg.whack_sc_op != SC_OP_NONE)
    {
	if (pkcs11_proxy)
	    scx_op_via_whack(msg.sc_data, msg.inbase, msg.outbase
			   , msg.whack_sc_op, msg.keyid, whackfd);
	else
	   plog("pkcs11 access to smartcard not allowed (set pkcs11proxy=yes)");
    }

    whack_log_fd = NULL_FD;
    close(whackfd);
}

/*
 * Local Variables:
 * c-basic-offset:4
 * c-style: pluto
 * End:
 */
