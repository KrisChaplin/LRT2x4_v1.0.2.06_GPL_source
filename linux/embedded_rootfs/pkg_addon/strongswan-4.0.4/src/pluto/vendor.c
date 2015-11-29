/* ISAKMP VendorID
 * Copyright (C) 2002-2005 Mathieu Lafon - Arkoon Network Security
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
 * RCSID $Id: vendor.c 12119 2013-07-09 13:59:35Z dio.li $
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/queue.h>
#include <freeswan.h>

#include "constants.h"
#include "defs.h"
#include "log.h"
#include "md5.h"
#include "connections.h"
#include "packet.h"
#include "demux.h"
#include "whack.h"
#include "vendor.h"
#include "kernel.h"
#include "nat_traversal.h"

/**
 * Unknown/Special VID:
 *
 * SafeNet SoftRemote 8.0.0:
 *  47bbe7c993f1fc13b4e6d0db565c68e5010201010201010310382e302e3020284275696c6420313029000000
 *  >> 382e302e3020284275696c6420313029 = '8.0.0 (Build 10)'
 *  da8e937880010000
 *
 * SafeNet SoftRemote 9.0.1
 *  47bbe7c993f1fc13b4e6d0db565c68e5010201010201010310392e302e3120284275696c6420313229000000
 *  >> 392e302e3120284275696c6420313229 = '9.0.1 (Build 12)'
 *  da8e937880010000
 *
 * Netscreen:
 *  d6b45f82f24bacb288af59a978830ab7
 *  cf49908791073fb46439790fdeb6aeed981101ab0000000500000300
 *
 * Cisco:
 *  1f07f70eaa6514d3b0fa96542a500300 (VPN 3000 version 3.0.0)
 *  1f07f70eaa6514d3b0fa96542a500301 (VPN 3000 version 3.0.1)
 *  1f07f70eaa6514d3b0fa96542a500305 (VPN 3000 version 3.0.5)
 *  1f07f70eaa6514d3b0fa96542a500407 (VPN 3000 version 4.0.7)
 *  (Can you see the pattern?)
 *  afcad71368a1f1c96b8696fc77570100 (Non-RFC Dead Peer Detection ?)
 *  c32364b3b4f447eb17c488ab2a480a57
 *  6d761ddc26aceca1b0ed11fabbb860c4
 *  5946c258f99a1a57b03eb9d1759e0f24 (From a Cisco VPN 3k)
 *  ebbc5b00141d0c895e11bd395902d690 (From a Cisco VPN 3k)
 *
 * Microsoft L2TP (???):
 *  47bbe7c993f1fc13b4e6d0db565c68e5010201010201010310382e312e3020284275696c6420313029000000
 *  >> 382e312e3020284275696c6420313029 = '8.1.0 (Build 10)'
 *  3025dbd21062b9e53dc441c6aab5293600000000
 *  da8e937880010000
 *
 * 3COM-superstack
 *    da8e937880010000
 *    404bf439522ca3f6
 *

 * If someone know what they mean, mail me.
 */

#define MAX_LOG_VID_LEN    32

#define VID_KEEP                   0x0000
#define VID_MD5HASH                0x0001
#define VID_STRING                 0x0002
#define VID_FSWAN_HASH             0x0004

#define VID_SUBSTRING_DUMPHEXA     0x0100
#define VID_SUBSTRING_DUMPASCII    0x0200
#define VID_SUBSTRING_MATCH        0x0400
#define VID_SUBSTRING  (VID_SUBSTRING_DUMPHEXA | VID_SUBSTRING_DUMPASCII | VID_SUBSTRING_MATCH)

struct vid_struct {
	enum known_vendorid id;
	unsigned short flags;
	const char *data;
	const char *descr;
	const char *vid;
	u_int vid_len;
};

#define DEC_MD5_VID_D(id,str,descr) \
	{ VID_##id, VID_MD5HASH, str, descr, NULL, 0 },
#define DEC_MD5_VID(id,str) \
	{ VID_##id, VID_MD5HASH, str, NULL, NULL, 0 },
#define DEC_FSWAN_VID(id,str,descr) \
	{ VID_##id, VID_FSWAN_HASH, str, descr, NULL, 0 },

static struct vid_struct _vid_tab[] = {

	/* Implementation names */

	{ VID_OPENPGP, VID_STRING, "OpenPGP10171", "OpenPGP", NULL, 0 },

	DEC_MD5_VID(KAME_RACOON, "KAME/racoon")

	{ VID_MS_NT5, VID_MD5HASH | VID_SUBSTRING_DUMPHEXA,
		"MS NT5 ISAKMPOAKLEY", NULL, NULL, 0 },

	DEC_MD5_VID(SSH_SENTINEL, "SSH Sentinel")
	DEC_MD5_VID(SSH_SENTINEL_1_1, "SSH Sentinel 1.1")
	DEC_MD5_VID(SSH_SENTINEL_1_2, "SSH Sentinel 1.2")
	DEC_MD5_VID(SSH_SENTINEL_1_3, "SSH Sentinel 1.3")
	DEC_MD5_VID(SSH_SENTINEL_1_4, "SSH Sentinel 1.4")
	DEC_MD5_VID(SSH_SENTINEL_1_4_1, "SSH Sentinel 1.4.1")

	/* These ones come from SSH vendors.txt */
	DEC_MD5_VID(SSH_IPSEC_1_1_0,
		"Ssh Communications Security IPSEC Express version 1.1.0")
	DEC_MD5_VID(SSH_IPSEC_1_1_1,
		"Ssh Communications Security IPSEC Express version 1.1.1")
	DEC_MD5_VID(SSH_IPSEC_1_1_2,
		"Ssh Communications Security IPSEC Express version 1.1.2")
	DEC_MD5_VID(SSH_IPSEC_1_2_1,
		"Ssh Communications Security IPSEC Express version 1.2.1")
	DEC_MD5_VID(SSH_IPSEC_1_2_2,
		"Ssh Communications Security IPSEC Express version 1.2.2")
	DEC_MD5_VID(SSH_IPSEC_2_0_0,
		"SSH Communications Security IPSEC Express version 2.0.0")
	DEC_MD5_VID(SSH_IPSEC_2_1_0,
		"SSH Communications Security IPSEC Express version 2.1.0")
	DEC_MD5_VID(SSH_IPSEC_2_1_1,
		"SSH Communications Security IPSEC Express version 2.1.1")
	DEC_MD5_VID(SSH_IPSEC_2_1_2,
		"SSH Communications Security IPSEC Express version 2.1.2")
	DEC_MD5_VID(SSH_IPSEC_3_0_0,
		"SSH Communications Security IPSEC Express version 3.0.0")
	DEC_MD5_VID(SSH_IPSEC_3_0_1,
		"SSH Communications Security IPSEC Express version 3.0.1")
	DEC_MD5_VID(SSH_IPSEC_4_0_0,
		"SSH Communications Security IPSEC Express version 4.0.0")
	DEC_MD5_VID(SSH_IPSEC_4_0_1,
		"SSH Communications Security IPSEC Express version 4.0.1")
	DEC_MD5_VID(SSH_IPSEC_4_1_0,
		"SSH Communications Security IPSEC Express version 4.1.0")
	DEC_MD5_VID(SSH_IPSEC_4_2_0,
		"SSH Communications Security IPSEC Express version 4.2.0")

	/* note: md5('CISCO-UNITY') = 12f5f28c457168a9702d9fe274cc02d4 */
	{ VID_CISCO_UNITY, VID_KEEP, NULL, "Cisco-Unity",
		"\x12\xf5\xf2\x8c\x45\x71\x68\xa9\x70\x2d\x9f\xe2\x74\xcc\x01\x00",
		16 },

	{ VID_CISCO3K, VID_KEEP | VID_SUBSTRING_MATCH,
          NULL, "Cisco VPN 3000 Series" , "\x1f\x07\xf7\x0e\xaa\x65\x14\xd3\xb0\xfa\x96\x54\x2a\x50", 14},

	/*
	 * Timestep VID seen:
	 *   - 54494d455354455020312053475720313532302033313520322e303145303133
	 *     = 'TIMESTEP 1 SGW 1520 315 2.01E013'
	 */
	{ VID_TIMESTEP, VID_STRING | VID_SUBSTRING_DUMPASCII, "TIMESTEP",
		NULL, NULL, 0 },

	/*
	 * Netscreen:
	 * 4865617274426561745f4e6f74696679386b0100  (HeartBeat_Notify + 386b0100)
	 */
	{ VID_MISC_HEARTBEAT_NOTIFY, VID_STRING | VID_SUBSTRING_DUMPHEXA,
		"HeartBeat_Notify", "HeartBeat Notify", NULL, 0 },

	/*
	 * MacOS X
	 */
	{ VID_MACOSX, VID_STRING|VID_SUBSTRING_DUMPHEXA, "Mac OSX 10.x",
	  "\x4d\xf3\x79\x28\xe9\xfc\x4f\xd1\xb3\x26\x21\x70\xd5\x15\xc6\x62", NULL, 0},

	/*
	 * Openswan
	 */
	DEC_FSWAN_VID(OPENSWAN2, "Openswan 2.2.0", "Openswan 2.2.0")
	
	/* NCP */
	{ VID_NCP_SERVER, VID_KEEP | VID_SUBSTRING_MATCH, NULL, "NCP Server",
	    "\xc6\xf5\x7a\xc3\x98\xf4\x93\x20\x81\x45\xb7\x58", 12},
	{ VID_NCP_CLIENT, VID_KEEP | VID_SUBSTRING_MATCH, NULL, "NCP Client",
	    "\xeb\x4c\x1b\x78\x8a\xfd\x4a\x9c\xb7\x73\x0a\x68", 12},
	/*
	 * strongSwan
	 */
	DEC_MD5_VID(STRONGSWAN,       "strongSwan 4.0.4")
	DEC_MD5_VID(STRONGSWAN_4_0_3, "strongSwan 4.0.3")
	DEC_MD5_VID(STRONGSWAN_4_0_2, "strongSwan 4.0.2")
	DEC_MD5_VID(STRONGSWAN_4_0_1, "strongSwan 4.0.1")
	DEC_MD5_VID(STRONGSWAN_4_0_0, "strongSwan 4.0.0")

	DEC_MD5_VID(STRONGSWAN_2_7_4, "strongSwan 2.7.4")
	DEC_MD5_VID(STRONGSWAN_2_7_3, "strongSwan 2.7.3")
	DEC_MD5_VID(STRONGSWAN_2_7_2, "strongSwan 2.7.2")
	DEC_MD5_VID(STRONGSWAN_2_7_1, "strongSwan 2.7.1")
	DEC_MD5_VID(STRONGSWAN_2_7_0, "strongSwan 2.7.0")
	DEC_MD5_VID(STRONGSWAN_2_6_4, "strongSwan 2.6.4")
	DEC_MD5_VID(STRONGSWAN_2_6_3, "strongSwan 2.6.3")
	DEC_MD5_VID(STRONGSWAN_2_6_2, "strongSwan 2.6.2")
	DEC_MD5_VID(STRONGSWAN_2_6_1, "strongSwan 2.6.1")
	DEC_MD5_VID(STRONGSWAN_2_6_0, "strongSwan 2.6.0")
	DEC_MD5_VID(STRONGSWAN_2_5_7, "strongSwan 2.5.7")
	DEC_MD5_VID(STRONGSWAN_2_5_6, "strongSwan 2.5.6")
	DEC_MD5_VID(STRONGSWAN_2_5_5, "strongSwan 2.5.5")
	DEC_MD5_VID(STRONGSWAN_2_5_4, "strongSwan 2.5.4")
	DEC_MD5_VID(STRONGSWAN_2_5_3, "strongSwan 2.5.3")
	DEC_MD5_VID(STRONGSWAN_2_5_2, "strongSwan 2.5.2")
	DEC_MD5_VID(STRONGSWAN_2_5_1, "strongSwan 2.5.1")
	DEC_MD5_VID(STRONGSWAN_2_5_0, "strongSwan 2.5.0")
	DEC_MD5_VID(STRONGSWAN_2_4_4, "strongSwan 2.4.4")
	DEC_MD5_VID(STRONGSWAN_2_4_3, "strongSwan 2.4.3")
	DEC_MD5_VID(STRONGSWAN_2_4_2, "strongSwan 2.4.2")
	DEC_MD5_VID(STRONGSWAN_2_4_1, "strongSwan 2.4.1")
	DEC_MD5_VID(STRONGSWAN_2_4_0, "strongSwan 2.4.0")
	DEC_MD5_VID(STRONGSWAN_2_3_2, "strongSwan 2.3.2")
	DEC_MD5_VID(STRONGSWAN_2_3_1, "strongSwan 2.3.1")
	DEC_MD5_VID(STRONGSWAN_2_3_0, "strongSwan 2.3.0")
	DEC_MD5_VID(STRONGSWAN_2_2_2, "strongSwan 2.2.2")
	DEC_MD5_VID(STRONGSWAN_2_2_1, "strongSwan 2.2.1")
	DEC_MD5_VID(STRONGSWAN_2_2_0, "strongSwan 2.2.0")

	/* NAT-Traversal */

	DEC_MD5_VID(NATT_STENBERG_01, "draft-stenberg-ipsec-nat-traversal-01")
	DEC_MD5_VID(NATT_STENBERG_02, "draft-stenberg-ipsec-nat-traversal-02")
	DEC_MD5_VID(NATT_HUTTUNEN, "ESPThruNAT")
	DEC_MD5_VID(NATT_HUTTUNEN_ESPINUDP, "draft-huttunen-ipsec-esp-in-udp-00.txt")
	DEC_MD5_VID(NATT_IETF_00, "draft-ietf-ipsec-nat-t-ike-00")
	DEC_MD5_VID(NATT_IETF_02, "draft-ietf-ipsec-nat-t-ike-02")
	/* hash in draft-ietf-ipsec-nat-t-ike-02 contains '\n'... Accept both */
	DEC_MD5_VID_D(NATT_IETF_02_N, "draft-ietf-ipsec-nat-t-ike-02\n", "draft-ietf-ipsec-nat-t-ike-02_n")
	DEC_MD5_VID(NATT_IETF_03, "draft-ietf-ipsec-nat-t-ike-03")
	DEC_MD5_VID(NATT_RFC, "RFC 3947")

	/* misc */
	
	{ VID_MISC_XAUTH, VID_KEEP, NULL, "XAUTH",
	    "\x09\x00\x26\x89\xdf\xd6\xb7\x12", 8 },

	{ VID_MISC_DPD, VID_KEEP, NULL, "Dead Peer Detection",
	    "\xaf\xca\xd7\x13\x68\xa1\xf1\xc9\x6b\x86\x96\xfc\x77\x57\x01\x00", 16 },

	DEC_MD5_VID(MISC_FRAGMENTATION, "FRAGMENTATION")
	
	DEC_MD5_VID(INITIAL_CONTACT, "Vid-Initial-Contact")

	/* -- */
	{ 0, 0, NULL, NULL, NULL, 0 }

};

static const char _hexdig[] = "0123456789abcdef";

static int _vid_struct_init = 0;

void
init_vendorid(void)
{
    struct vid_struct *vid;
    MD5_CTX ctx;
    int i;

    for (vid = _vid_tab; vid->id; vid++)
    {
	if (vid->flags & VID_STRING)
	{
	    /** VendorID is a string **/
	    vid->vid = strdup(vid->data);
	    vid->vid_len = strlen(vid->data);
	}
	else if (vid->flags & VID_MD5HASH)
	{
	    /** VendorID is a string to hash with MD5 **/
	    char *vidm =  malloc(MD5_DIGEST_SIZE);

	    vid->vid = vidm;
	    if (vidm)
	    {
		MD5Init(&ctx);
		MD5Update(&ctx, (const u_char *)vid->data, strlen(vid->data));
		MD5Final(vidm, &ctx);
		vid->vid_len = MD5_DIGEST_SIZE;
	    }
	}
	else if (vid->flags & VID_FSWAN_HASH)
	{
	    /** FreeS/WAN 2.00+ specific hash **/
#define FSWAN_VID_SIZE 12
	    unsigned char hash[MD5_DIGEST_SIZE];
	    char *vidm =  malloc(FSWAN_VID_SIZE);

	    vid->vid = vidm;
	    if (vidm)
	    {
		MD5Init(&ctx);
		MD5Update(&ctx, (const u_char *)vid->data, strlen(vid->data));
		MD5Final(hash, &ctx);
		vidm[0] = 'O';
		vidm[1] = 'E';
#if FSWAN_VID_SIZE - 2 <= MD5_DIGEST_SIZE
		memcpy(vidm + 2, hash, FSWAN_VID_SIZE - 2);
#else
		memcpy(vidm + 2, hash, MD5_DIGEST_SIZE);
		memset(vidm + 2 + MD5_DIGEST_SIZE, '\0',
			FSWAN_VID_SIZE - 2 - MD5_DIGEST_SIZE);
#endif
		for (i = 2; i < FSWAN_VID_SIZE; i++)
		{
		    vidm[i] &= 0x7f;
		    vidm[i] |= 0x40;
		}
		vid->vid_len = FSWAN_VID_SIZE;
	    }
	}

	if (vid->descr == NULL)
	{
	    /** Find something to display **/
	    vid->descr = vid->data;
	}
    }
    _vid_struct_init = 1;
}

static void
handle_known_vendorid (struct msg_digest *md
, const char *vidstr, size_t len, struct vid_struct *vid)
{
    char vid_dump[128];
    bool vid_useful = FALSE;
    size_t i, j;

    switch (vid->id) {
    /* Remote side supports OpenPGP certificates */
    case VID_OPENPGP:
	md->openpgp = TRUE;
	vid_useful = TRUE;
	break;

    /*
     * Use most recent supported NAT-Traversal method and ignore the
     * other ones (implementations will send all supported methods but
     * only one will be used)
     *
     * Note: most recent == higher id in vendor.h
     */
    case VID_NATT_IETF_00:
	if (!nat_traversal_support_non_ike)
	    break;
	if ((nat_traversal_enabled) && (!md->nat_traversal_vid))
	{
	    md->nat_traversal_vid = vid->id;
	    vid_useful = TRUE;
	}
	break;
    case VID_NATT_IETF_02:
    case VID_NATT_IETF_02_N:
    case VID_NATT_IETF_03:
    case VID_NATT_RFC:
	if (nat_traversal_support_port_floating
	&& md->nat_traversal_vid < vid->id)
	{
	    md->nat_traversal_vid = vid->id;
	    vid_useful = TRUE;
	}
	break;

    /* Remote side would like to do DPD with us on this connection */
    case VID_MISC_DPD:
	md->dpd = TRUE;
	vid_useful = TRUE;
	break;
    default:
	break;
    }

    if (vid->flags & VID_SUBSTRING_DUMPHEXA)
    {
	/* Dump description + Hexa */
	memset(vid_dump, 0, sizeof(vid_dump));
	snprintf(vid_dump, sizeof(vid_dump), "%s ",
		 vid->descr ? vid->descr : "");
	for (i = strlen(vid_dump), j = vid->vid_len;
	     j < len && i < sizeof(vid_dump) - 2;
	     i += 2, j++)
	{
	    vid_dump[i] = _hexdig[(vidstr[j] >> 4) & 0xF];
	    vid_dump[i+1] = _hexdig[vidstr[j] & 0xF];
	}
    }
    else if (vid->flags & VID_SUBSTRING_DUMPASCII)
    {
	/* Dump ASCII content */
	memset(vid_dump, 0, sizeof(vid_dump));
	for (i = 0; i < len && i < sizeof(vid_dump) - 1; i++)
	{
	    vid_dump[i] = (isprint(vidstr[i])) ? vidstr[i] : '.';
	}
    }
    else
    {
	/* Dump description (descr) */
	snprintf(vid_dump, sizeof(vid_dump), "%s",
		 vid->descr ? vid->descr : "");
    }

    loglog(RC_LOG_SERIOUS, "[Tunnel Negotiation Info] %s Vendor ID payload [%s]",
	    vid_useful ? "received" : "ignoring", vid_dump);
}

void
handle_vendorid (struct msg_digest *md, const char *vid, size_t len)
{
    struct vid_struct *pvid;

    if (!_vid_struct_init)
	init_vendorid();

    /*
     * Find known VendorID in _vid_tab
     */
    for (pvid = _vid_tab; pvid->id; pvid++)
    {
	if (pvid->vid && vid && pvid->vid_len && len)
	{
	    if (pvid->vid_len == len)
	    {
		if (memcmp(pvid->vid, vid, len) == 0)
		{
		    handle_known_vendorid(md, vid, len, pvid);
		    return;
		}
	    }
	    else if ((pvid->vid_len < len) && (pvid->flags & VID_SUBSTRING))
	    {
		if (memcmp(pvid->vid, vid, pvid->vid_len) == 0)
		{
		    handle_known_vendorid(md, vid, len, pvid);
		    return;
		}
	    }
	}
    }

    /*
     * Unknown VendorID. Log the beginning.
     */
    {
	char log_vid[2*MAX_LOG_VID_LEN+1];
	size_t i;

	memset(log_vid, 0, sizeof(log_vid));

	for (i = 0; i < len && i < MAX_LOG_VID_LEN; i++)
	{
	    log_vid[2*i] = _hexdig[(vid[i] >> 4) & 0xF];
	    log_vid[2*i+1] = _hexdig[vid[i] & 0xF];
	}
	loglog(RC_LOG_SERIOUS, "ignoring Vendor ID payload [%s%s]",
	       log_vid, (len>MAX_LOG_VID_LEN) ? "..." : "");
    }
}

/**
 * Add a vendor id payload to the msg
 */
bool
out_vendorid (u_int8_t np, pb_stream *outs, enum known_vendorid vid)
{
    struct vid_struct *pvid;

    if (!_vid_struct_init)
	init_vendorid();

    for (pvid = _vid_tab; pvid->id && pvid->id != vid; pvid++);

    if (pvid->id != vid)
	return STF_INTERNAL_ERROR; /* not found */
    if (!pvid->vid)
	return STF_INTERNAL_ERROR; /* not initialized */

    DBG(DBG_EMITTING,
	DBG_log("out_vendorid(): sending [%s]", pvid->descr)
    )
    return out_generic_raw(np, &isakmp_vendor_id_desc, outs,
		pvid->vid, pvid->vid_len, "V_ID");
}

