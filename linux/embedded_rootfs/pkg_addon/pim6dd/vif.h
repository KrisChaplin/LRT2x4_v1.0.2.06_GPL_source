/*	$KAME: vif.h,v 1.7 2002/06/21 13:55:04 suz Exp $	*/

/*
 * Copyright (c) 1998-2001
 * The University of Southern California/Information Sciences Institute.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/*
 * Part of this program has been derived from mrouted.
 * The mrouted program is covered by the license in the accompanying file
 * named "LICENSE.mrouted".
 *
 * The mrouted program is COPYRIGHT 1989 by The Board of Trustees of
 * Leland Stanford Junior University.
 *
 */


/*
 * Bitmap handling functions.
 * These should be fast but generic.  bytes can be slow to zero and compare,
 * words are hard to make generic.  Thus two sets of macros (yuk).
 */

/*
 * The VIFM_ functions should migrate out of <netinet/ip_mroute.h>, since
 * the kernel no longer uses vifbitmaps.
 */
#ifndef VIFM_SET

typedef	u_int32 vifbitmap_t;

#define	VIFM_SET(n, m)			((m) |=  (1 << (n)))
#define	VIFM_CLR(n, m)			((m) &= ~(1 << (n)))
#define	VIFM_ISSET(n, m)		((m) &   (1 << (n)))
#define VIFM_CLRALL(m)			((m) = 0x00000000)
#define VIFM_COPY(mfrom, mto)		((mto) = (mfrom))
#define VIFM_SAME(m1, m2)		((m1) == (m2))
#endif
/*
 * And <netinet/ip_mroute.h> was missing some required functions anyway
 */
#if !defined(__NetBSD__) && !defined(__OpenBSD__)
#define	VIFM_SETALL(m)			((m) = ~0)
#endif
#define	VIFM_ISSET_ONLY(n, m)		((m) == (1 << (n)))
#define	VIFM_ISEMPTY(m)			((m) == 0)
#define	VIFM_CLR_MASK(m, mask)		((m) &= ~(mask))
#define	VIFM_SET_MASK(m, mask)		((m) |= (mask))
#define VIFM_MERGE(m1, m2, result)      ((result) = (m1) | (m2))

/*
 * And <netinet6/ip6_mroute.h> was missing some required functions anyway
 */
extern if_set if_nullset;
#define IF_ISEMPTY(p) (memcmp((p), &if_nullset, sizeof(if_nullset)) == 0)
#define IF_CLR_MASK(p, mask) \
  {\
	int idx;\
	for (idx = 0; idx < sizeof(*(p))/sizeof(fd_mask); idx++) {\
		(p)->ifs_bits[idx] &= ~((mask)->ifs_bits[idx]);\
	}\
  }
#define IF_MERGE(p1, p2, result) \
  {\
	int idx;\
	for (idx = 0; idx < sizeof(*(p1))/sizeof(fd_mask); idx++) {\
		(result)->ifs_bits[idx] = (p1)->ifs_bits[idx]|(p2)->ifs_bits[idx]; \
	}\
  }
#define IF_SAME(p1, p2) (memcmp((p1),(p2),sizeof(*(p1))) == 0)

/* Check whether I am the forwarder on some LAN */
#define VIFM_FORWARDER(leaves, oifs)    ((leaves) & (oifs))

/*
 * Neighbor bitmaps are, for efficiency, implemented as a struct
 * containing two variables of a native machine type.  If you
 * have a native type that's bigger than a long, define it below.
 */
#define	NBRTYPE		u_long
#define NBRBITS		sizeof(NBRTYPE) * 8

typedef struct {
    NBRTYPE hi;
    NBRTYPE lo;
} nbrbitmap_t;
#define	MAXNBRS		2 * NBRBITS

#define	NBRM_SET(n, m)		(((n) < NBRBITS) ? ((m).lo |= (1 << (n))) :  \
				      ((m).hi |= (1 << (n - NBRBITS))))
#define	NBRM_CLR(n, m)		(((n) < NBRBITS) ? ((m).lo &= ~(1 << (n))) : \
				      ((m).hi &= ~(1 << (n - NBRBITS))))
#define	NBRM_ISSET(n, m)	(((n) < NBRBITS) ? ((m).lo & (1 << (n))) :   \
				      ((m).hi & (1 << ((n) - NBRBITS))))
#define	NBRM_CLRALL(m)		((m).lo = (m).hi = 0)
#define	NBRM_COPY(mfrom, mto)	((mto).lo = (mfrom).lo, (mto).hi = (mfrom).hi)
#define	NBRM_SAME(m1, m2)	(((m1).lo == (m2).lo) && ((m1).hi == (m2).hi))
#define	NBRM_ISEMPTY(m)		(((m).lo == 0) && ((m).hi == 0))
#define	NBRM_SETMASK(m, mask)	(((m).lo |= (mask).lo),((m).hi |= (mask).hi))
#define	NBRM_CLRMASK(m, mask)	(((m).lo &= ~(mask).lo),((m).hi &= ~(mask).hi))
#define	NBRM_MASK(m, mask)	(((m).lo &= (mask).lo),((m).hi &= (mask).hi))
#define	NBRM_ISSETMASK(m, mask)	(((m).lo & (mask).lo) || ((m).hi & (mask).hi))
#define	NBRM_ISSETALLMASK(m, mask)\
				((((m).lo & (mask).lo) == (mask).lo) && \
				 (((m).hi & (mask).hi) == (mask).hi))
/*
 * This macro is TRUE if all the subordinates have been pruned, or if
 * there are no subordinates on this vif.
 * The arguments is the map of subordinates, the map of neighbors on the
 * vif, and the map of received prunes.
 */
#define	SUBS_ARE_PRUNED(sub, vifmask, prunes)	\
    (((sub).lo & (vifmask).lo) == ((prunes).lo & (vifmask).lo & (sub).lo) && \
     ((sub).hi & (vifmask).hi) == ((prunes).hi & (vifmask).hi & (sub).hi))

/*
 * User level Virtual Interface structure
 *
 * A "virtual interface" is either a physical, multicast-capable interface
 * (called a "phyint"), a virtual point-to-point link (called a "tunnel")
 * or a "register vif" used by PIM. The register vif is used by the
 * Designated Router (DR) to send encapsulated data packets to the
 * Rendevous Point (RP) for a particular group. The data packets are 
 * encapsulated in PIM messages (IPPROTO_PIM = 103) and then unicast to 
 * the RP.
 * (Note: all addresses, subnet numbers and masks are kept in NETWORK order.)
 */
struct uvif {
    u_int	     uv_flags;	    /* VIFF_ flags defined below            */
    u_char	     uv_metric;     /* cost of this vif                     */
    u_char	     uv_admetric;   /* advertised cost of this vif          */
#if 0				/* unused for IPv6? */
    u_char	     uv_threshold;  /* min ttl required to forward on vif   */
#endif 
    u_int	     uv_rate_limit; /* rate limit on this vif               */
    struct sockaddr_in6 uv_lcl_addr;/* local address of this vif            */
    struct phaddr   *uv_linklocal;/* link-local address of this vif      */
#if 0
    u_int32	     uv_rmt_addr;   /* remote end-point addr (tunnels only) */
#endif 
    struct sockaddr_in6 uv_dst_addr;   /* destination for PIM messages         */
    struct sockaddr_in6 uv_prefix;  /* prefix                (phyints only) */
    struct in6_addr  uv_subnetmask; /* subnet mask           (phyints only) */
    char	     uv_name[IFNAMSIZ]; /* interface name                   */
    u_int	     uv_ifindex;    /* index of the interface */
    u_int	     uv_siteid;     /* index of the site on the interface */
    struct listaddr *uv_groups;     /* list of local groups  (phyints only) */
    struct listaddr *uv_dvmrp_neighbors; /* list of neighboring routers     */
    nbrbitmap_t	     uv_nbrmap;	    /* bitmap of active neighboring routers */
    struct listaddr *uv_querier;    /* MLD querier on vif                   */
    int		     uv_prune_lifetime; /* Prune lifetime or 0 for default  */
    struct vif_acl  *uv_acl;	    /* access control list of groups        */
    int		     uv_leaf_timer; /* time until this vif is considrd leaf */
    struct phaddr   *uv_addrs;	    /* Additional addresses on this vif     */
    struct vif_filter *uv_filter;   /* Route filters on this vif	    */
    u_int16	    uv_pim_hello_timer;/* timer for sending PIM hello msgs  */
    u_int16	    uv_gq_timer;    /* Group Query timer        	    */
    int             uv_local_pref;  /* default local preference for assert  */
    int             uv_local_metric; /* default local metric for assert     */
    struct pim_nbr_entry *uv_pim_neighbors; /* list of PIM neighbor routers */
};

/* TODO: define VIFF_KERNEL_FLAGS */
#define VIFF_KERNEL_FLAGS	(VIFF_TUNNEL | VIFF_SRCRT)
#define VIFF_DOWN		0x000100       /* kernel state of interface */
#define VIFF_DISABLED		0x000200       /* administratively disabled */
#define VIFF_QUERIER		0x000400       /* I am the subnet's querier */
#define VIFF_ONEWAY		0x000800       /* Maybe one way interface   */
#define VIFF_LEAF		0x001000       /* all neighbors are leaves  */
#define VIFF_IGMPV1		0x002000       /* Act as an IGMPv1 Router   */
#define	VIFF_REXMIT_PRUNES	0x004000       /* retransmit prunes         */
#define VIFF_PASSIVE		0x008000       /* passive tunnel	    */
#define	VIFF_ALLOW_NONPRUNERS	0x010000       /* ok to peer with nonprunrs */
#define VIFF_NOFLOOD		0x020000       /* don't flood on this vif   */
#define	VIFF_DR			0x040000       /* designated router	    */
/* TODO: VIFF_NONBRS == VIFF_ONEWAY? */
#define	VIFF_NONBRS		0x080000       /* no neighbor on vif	    */
#define VIFF_POINT_TO_POINT     0x100000       /* point-to-point link       */
#define VIFF_PIM_NBR            0x200000       /* PIM neighbor              */
#define VIFF_DVMRP_NBR          0x400000       /* DVMRP neighbor            */
#define VIFF_NOLISTENER         0x800000       /* no listener on the link   */

struct phaddr {
    struct phaddr   *pa_next;
    struct sockaddr_in6  pa_addr;       /* extra address */
    struct sockaddr_in6  pa_rmt_addr;	/* valid only in case of P2P I/F */
    struct sockaddr_in6  pa_prefix;	/* prefix of the extra address */
    struct in6_addr  pa_subnetmask;	/* netmask */
};

/* The Access Control List (list with scoped addresses) member */
struct vif_acl {
    struct vif_acl  *acl_next;	    /* next acl member         */
    u_int32	     acl_addr;	    /* Group address           */
    u_int32	     acl_mask;	    /* Group addr. mask        */
};

struct vif_filter {
    int			vf_type;
#define	VFT_ACCEPT	1
#define	VFT_DENY	2
    int			vf_flags;
#define	VFF_BIDIR	1
    struct vf_element  *vf_filter;
};

struct vf_element {
    struct vf_element  *vfe_next;
    struct sockaddr_in6	*vfe_addr;
    struct in6_addr	vfe_mask;
    int			vfe_flags;
#define	VFEF_EXACT	0x0001
};

struct listaddr {
    struct listaddr *al_next;		/* link to next addr, MUST BE FIRST */
    struct sockaddr_in6 al_addr;	/* local group or neighbor address  */
    u_long	     al_timer;		/* for timing out group or neighbor */
    time_t	     al_ctime;		/* entry creation time		    */
    union {
    	u_int32	     alu_genid;		/* generation id for neighbor       */
    	struct sockaddr_in6 alu_reporter;/* a host which reported membership */
    } al_alu;
    u_char	     al_pv;		/* router protocol version	    */
    u_char	     al_mv;		/* router mrouted version	    */
    u_char	     al_index;		/* neighbor index		    */
    u_long	     al_timerid;        /* timer for group membership	    */
    u_long	     al_query;		/* timer for repeated leave query   */
    u_int16	     al_flags;		/* flags related to this neighbor   */
};
#define	al_genid	al_alu.alu_genid
#define	al_reporter	al_alu.alu_reporter

#define	NBRF_LEAF		0x0001	/* This neighbor is a leaf 	    */
#define	NBRF_GENID		0x0100	/* I know this neighbor's genid	    */
#define	NBRF_WAITING		0x0200	/* Waiting for peering to come up   */
#define	NBRF_ONEWAY		0x0400	/* One-way peering 		    */
#define	NBRF_TOOOLD		0x0800	/* Too old (policy decision) 	    */
#define	NBRF_TOOMANYROUTES	0x1000	/* Neighbor is spouting routes 	    */
#define	NBRF_NOTPRUNING		0x2000	/* Neighbor doesn't appear to prune */

/*
 * Don't peer with neighbors with any of these flags set
 */
#define	NBRF_DONTPEER		(NBRF_WAITING|NBRF_ONEWAY|NBRF_TOOOLD| \
				 NBRF_TOOMANYROUTES|NBRF_NOTPRUNING)

#define NO_VIF		((vifi_t)MAXMIFS)  /* An invalid vif index */
  

/*
 * Used to get the RPF neighbor and IIF info
 * for a given source from the unicast routing table. 
 */
struct rpfctl {
    struct sockaddr_in6 source; /* the source for which we want iif and rpfnbr */
    struct sockaddr_in6 rpfneighbor;/* next hop towards the source */
    vifi_t iif; /* the incoming interface to reach the next hop */
};
