/* dhcrelay.c

   DHCP/BOOTP Relay Agent. */

/*
 * Copyright(c) 2004-2010 by Internet Systems Consortium, Inc.("ISC")
 * Copyright(c) 1997-2003 by Internet Software Consortium
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 *   Internet Systems Consortium, Inc.
 *   950 Charter Street
 *   Redwood City, CA 94063
 *   <info@isc.org>
 *   https://www.isc.org/
 *
 * This software has been written for Internet Systems Consortium
 * by Ted Lemon in cooperation with Vixie Enterprises and Nominum, Inc.
 * To learn more about Internet Systems Consortium, see
 * ``https://www.isc.org/''.  To learn more about Vixie Enterprises,
 * see ``http://www.vix.com''.   To learn more about Nominum, Inc., see
 * ``http://www.nominum.com''.
 */

#include <nkuserlandconf.h>
#ifdef CONFIG_NK_DHCP_SERVER_RELAY

#define  NK_db_read(a,b)	kd_doCommand(a, CMD_PRINT, ASH_DO_NOTHING, b)
#include "dhcpd.h"
#include <syslog.h>
#include <sys/time.h>
#include <string.h>

TIME default_lease_time = 43200; /* 12 hours... */
TIME max_lease_time = 86400; /* 24 hours... */
struct tree_cache *global_options[256];

struct option *requested_opts[2];

int bogus_agent_drops = 0;	/* Packets dropped because agent option
				   field was specified and we're not relaying
				   packets that already have an agent option
				   specified. */
int bogus_giaddr_drops = 0;	/* Packets sent to us to relay back to a
				   client, but with a bogus giaddr. */
int client_packets_relayed = 0;	/* Packets relayed from client to server. */
int server_packet_errors = 0;	/* Errors sending packets to servers. */
int server_packets_relayed = 0;	/* Packets relayed from server to client. */
int client_packet_errors = 0;	/* Errors sending packets to clients. */

int add_agent_options = 0;	/* If nonzero, add relay agent options. */

int agent_option_errors = 0;    /* Number of packets forwarded without
				   agent options because there was no room. */
int drop_agent_mismatches = 0;	/* If nonzero, drop server replies that
				   don't have matching circuit-id's. */
int corrupt_agent_options = 0;	/* Number of packets dropped because
				   relay agent information option was bad. */
int missing_agent_option = 0;	/* Number of packets dropped because no
				   RAI option matching our ID was found. */
int bad_circuit_id = 0;		/* Circuit ID option in matching RAI option
				   did not match any known circuit ID. */
int missing_circuit_id = 0;	/* Circuit ID option in matching RAI option
				   was missing. */
int max_hop_count = 10;		/* Maximum hop count */

#ifdef DHCPv6
	/* Force use of DHCPv6 interface-id option. */
isc_boolean_t use_if_id = ISC_FALSE;
#endif

	/* What to do about packets we're asked to relay that
	   already have a relay option: */
enum { forward_and_append,	/* Forward and append our own relay option. */
       forward_and_replace,	/* Forward, but replace theirs with ours. */
       forward_untouched,	/* Forward without changes. */
       discard } agent_relay_mode = forward_and_replace;

u_int16_t local_port_relay;
u_int16_t remote_port_relay;

/* Relay agent server list. */
struct server_list {
	struct server_list *next;
	struct sockaddr_in to;
} *servers_relay;

#ifdef DHCPv6
struct stream_list {
	struct stream_list *next;
	struct interface_info *ifp;
	struct sockaddr_in6 link;
	int id;
} *downstreams, *upstreams;

static struct stream_list *parse_downstream(char *);
static struct stream_list *parse_upstream(char *);
static void setup_streams(void);
void setup_relay_streams(void) {
	setup_streams();
}
#endif
int vlan_number=0;

static void do_relay4(struct interface_info *, struct dhcp_packet *,
	              unsigned int, unsigned int, struct iaddr,
		      struct hardware *, int );
static int add_relay_agent_options(struct interface_info *,
				   struct dhcp_packet *, unsigned,
				   struct in_addr);
static int find_interface_by_agent_option(struct dhcp_packet *,
			       struct interface_info **, u_int8_t *, int);
static int strip_relay_agent_options(struct interface_info *,
				     struct interface_info **,
				     struct dhcp_packet *, unsigned);
static ssize_t nk_send_packet(struct interface_info *,
			    struct packet *, struct dhcp_packet *, size_t, 
			    struct in_addr,
			    struct sockaddr_in *, struct hardware *);
//8021Q 
int vlan_tag= 0;

// for self test only
int test_select = 0;
int test_vlan_port = 0;
int test_vlan_id = 0;
int test_add_opt132 = 0;
int add_remote_id = 0;
#define MAX_SUBSCRIBE_ID_LEN 20
char test_subscribe_id[MAX_SUBSCRIBE_ID_LEN] = "";

static char *strncpyz(char *dest, char const *src, size_t size)
{
    if (!size--)
	return dest;
    strncpy(dest, src, size);
    dest[size] = 0; /* Make sure the string is null terminated */
    return dest;
}
static int find_special_word(char *offset, int len, int i)
{
f_again:
    if (((offset[i+len+1] == '\"')&&(offset[i+len+2] == '&'))||((offset[i+len+1] == '\"')&&(offset[i+len+2] == '\"')))
    {
	i++;
	for (i; (((offset[i+len+1] == '\"')&&(offset[i+len+2] == '&'))||((offset[i+len+1] == '\"')&&(offset[i+len+2] == '\"'))); i++);
	goto f_again;
    }
    return i;
}
static char* name_get_value(char *string, char *varname, char *retval, int buf_len, char *offset)
{
	int i=0, len=strlen(varname), break_loop=0, ValueEmpty=0;
	char searchName[257];
	char checkValueEnd[257];
	
	strcpy(searchName, varname);
	strcpy(checkValueEnd, varname);
	strcat(checkValueEnd, "=\"\"");
	strcat(searchName,"=\"");
	len+=2;
	
	if (!string)
	{
		retval[0] = '\0';
		return NULL;
	}
	offset = offset ? offset : string;
	
	char *p=NULL,*q=NULL;
	if(p=strstr(offset, checkValueEnd))
	{
		q=strstr( offset , searchName );
		if(p == q)
		ValueEmpty=1;
	}
	if (((offset=strstr(offset, searchName)) != 0) && (ValueEmpty==0))
	{
		for (i=0; ((offset+i+len)<(string+strlen(string)) && (break_loop==0)); i++)
		{
			if(((offset[i+len+1] == '\"')&&(offset[i+len+2] == '&'))||((offset[i+len+1] == '\"')&&(offset[i+len+2] == '\"')))
			{
				break_loop=1;
			}
		}
		
		i = find_special_word(offset, len, i);
		if (buf_len >= (i+1))
		{
			strncpyz(retval, (offset+len), i+1);
		}
		return (offset+len+2+(i+1));
	}
	retval[0] = '\0';
	return NULL;
}

/*
 * return 0: success
 * return -1: log_fatal("Server cannot run in both IPv4 and IPv6 ...);
 * return -2: usage();
 */
int nk_set_relay_param(int argc, char **argv, int *idx)
{
	isc_result_t status;
	struct interface_info *tmp = NULL;
	struct server_list *sp = NULL;
#ifdef DHCPv6
	struct stream_list *sl = NULL;
	static int local_family_set = 0;
#endif
	int i = *idx;
#ifdef DHCPv6
	if (strcmp(argv[i], "-4") == 0 || strcmp(argv[i], "-6") == 0) {
		// set local_family and error message in dhcpd.c
		local_family_set = 1;
	} else
#endif
		if (!strcmp(argv[i], "-Rp")) {
		*idx += 1;
		if (++i == argc)
			return -2;
		local_port_relay = validate_port(argv[i]);
		log_debug("binding to user-specified port %d",
			  ntohs(local_port_relay));
	} else if (!strcmp(argv[i], "-Rc")) {
		int hcount;
		*idx += 1;
		if (++i == argc)
			return -2;
		hcount = atoi(argv[i]);
		if (hcount <= 255)
			max_hop_count= hcount;
		else
			return -2;
 	} else if (!strcmp(argv[i], "-Ri")) {
#ifdef DHCPv6
		if (local_family_set && (local_family == AF_INET6)) {
			return -2;
		}
		local_family_set = 1;
		local_family = AF_INET;
#endif
		*idx += 1;
		if (++i == argc) {
			return -2;
		}
		
		for (tmp = interfaces; tmp; tmp = tmp -> next) {
			// ifp may belong to lower, upper, or server interface
			if (strcmp(argv[i], tmp->name) == 0)
			{
				tmp->flags |= INTERFACE_STREAMS;
				break;
			}
		}
		if (!tmp) {
			status = interface_allocate(&tmp, MDL);
			if (status != ISC_R_SUCCESS)
				log_fatal("%s: interface_allocate: %s", argv[i], isc_result_totext(status));
		
			strcpy(tmp->name, argv[i]);
			interface_snorf(tmp, INTERFACE_REQUESTED | INTERFACE_STREAMS);
			interface_dereference(&tmp, MDL);
		}
	} else if (!strcmp(argv[i], "-Riu")) {
#ifdef DHCPv6
		if (local_family_set && (local_family == AF_INET6)) {
			return -2;
		}
		local_family_set = 1;
		local_family = AF_INET;
#endif
		*idx += 1;
		if (++i == argc) {
			return -2;
		}
		
		for (tmp = interfaces; tmp; tmp = tmp -> next) {
			// ifp may belong to lower, upper, or server interface
			if (strcmp(argv[i], tmp->name) == 0)
			{
				tmp->flags |= INTERFACE_UPSTREAM;
				break;
			}
		}
		if (!tmp) {
			status = interface_allocate(&tmp, MDL);
			if (status != ISC_R_SUCCESS)
				log_fatal("%s: interface_allocate: %s", argv[i], isc_result_totext(status));
		
			strcpy(tmp->name, argv[i]);
			interface_snorf(tmp, INTERFACE_REQUESTED | INTERFACE_UPSTREAM);
			interface_dereference(&tmp, MDL);
		}
	} else if (!strcmp(argv[i], "-Ril")) {
#ifdef DHCPv6
		if (local_family_set && (local_family == AF_INET6)) {
			return -2;
		}
		local_family_set = 1;
		local_family = AF_INET;
#endif
		*idx += 1;
		if (++i == argc) {
			return -2;
		}
		
		for (tmp = interfaces; tmp; tmp = tmp -> next) {
			// ifp may belong to lower, upper, or server interface
			if (strcmp(argv[i], tmp->name) == 0)
			{
				tmp->flags |= INTERFACE_DOWNSTREAM;
				break;
			}
		}
		if (!tmp) {
			status = interface_allocate(&tmp, MDL);
			if (status != ISC_R_SUCCESS)
				log_fatal("%s: interface_allocate: %s", argv[i], isc_result_totext(status));
		
			strcpy(tmp->name, argv[i]);
			interface_snorf(tmp, INTERFACE_REQUESTED | INTERFACE_DOWNSTREAM);
			interface_dereference(&tmp, MDL);
		}
	} else if (!strcmp(argv[i], "-Ra")) {
#ifdef DHCPv6
		if (local_family_set && (local_family == AF_INET6)) {
			return -2;
		}
		local_family_set = 1;
		local_family = AF_INET;
#endif
		add_agent_options = 1;
	} else if (!strcmp(argv[i], "-RA")) {
#ifdef DHCPv6
		if (local_family_set && (local_family == AF_INET6)) {
			return -2;
		}
		local_family_set = 1;
		local_family = AF_INET;
#endif
		*idx += 1;
		if (++i == argc)
			return -2;

		dhcp_max_agent_option_packet_length = atoi(argv[i]);

		if (dhcp_max_agent_option_packet_length > DHCP_MTU_MAX)
			log_fatal("%s: packet length exceeds "
				  "longest possible MTU\n", argv[i]);
	} else if (!strcmp(argv[i], "-Rm")) {
#ifdef DHCPv6
		if (local_family_set && (local_family == AF_INET6)) {
			return -2;
		}
		local_family_set = 1;
		local_family = AF_INET;
#endif
		*idx += 1;
		if (++i == argc)
			return -2;
		if (!strcasecmp(argv[i], "append")) {
			agent_relay_mode = forward_and_append;
		} else if (!strcasecmp(argv[i], "replace")) {
			agent_relay_mode = forward_and_replace;
		} else if (!strcasecmp(argv[i], "forward")) {
			agent_relay_mode = forward_untouched;
		} else if (!strcasecmp(argv[i], "discard")) {
			agent_relay_mode = discard;
		} else
			return -2;
	} else if (!strcmp(argv[i], "-RD")) {
#ifdef DHCPv6
		if (local_family_set && (local_family == AF_INET6)) {
			return -2;
		}
		local_family_set = 1;
		local_family = AF_INET;
#endif
		drop_agent_mismatches = 1;
#ifdef DHCPv6
	} else if (!strcmp(argv[i], "-RI")) {
		if (local_family_set && (local_family == AF_INET)) {
			return -2;
		}
		local_family_set = 1;
		local_family = AF_INET6;
		use_if_id = ISC_TRUE;
	} else if (!strcmp(argv[i], "-Rl")) {
		if (local_family_set && (local_family == AF_INET)) {
			return -2;
		}
		local_family_set = 1;
		local_family = AF_INET6;
		if (downstreams != NULL)
			use_if_id = ISC_TRUE;
		*idx += 1;
		if (++i == argc)
			return -2;
		sl = parse_downstream(argv[i]);
		sl->next = downstreams;
		downstreams = sl;
	} else if (!strcmp(argv[i], "-Ru")) {
		if (local_family_set && (local_family == AF_INET)) {
			return -2;
		}
		local_family_set = 1;
		local_family = AF_INET6;
		*idx += 1;
		if (++i == argc)
			return -2;
		sl = parse_upstream(argv[i]);
		sl->next = upstreams;
		upstreams = sl;
#endif
	} else if (!strcmp(argv[i], "--version")) {
		log_info("isc-dhcrelay-%s", PACKAGE_VERSION);
		return 0;
	} else if (!strcmp(argv[i], "-Rs")) {
		struct hostent *he;
		struct in_addr ia, *iap = NULL;
#ifdef DHCPv6
		if (local_family_set && (local_family == AF_INET6)) {
			return -2;
		}
		local_family_set = 1;
		local_family = AF_INET;
#endif
		*idx += 1;
		if (++i == argc)
			return -2;
		
		if (inet_aton(argv[i], &ia)) {
			iap = &ia;
		} else {
			he = gethostbyname(argv[i]);
			if (!he) {
				log_error("%s: host unknown", argv[i]);
			} else {
				iap = ((struct in_addr *)
				       he->h_addr_list[0]);
			}
		}

		if (iap) {
			sp = ((struct server_list *)
			      dmalloc(sizeof *sp, MDL));
			if (!sp)
				log_fatal("no memory for server.\n");
			sp->next = servers_relay;
			servers_relay = sp;
			memcpy(&sp->to.sin_addr, iap, sizeof *iap);
		}
	}
	// followings are for self test only
	else if (!strcmp(argv[i], "-r")){
		test_select = 1;
	} else if (!strcmp(argv[i], "-RR")) {
		*idx += 1;
		if (++i == argc)
			return -2;
		test_select = atoi(argv[i]);
	} else if (!strcmp(argv[i], "-RVP")) {
		*idx += 1;
		if (++i == argc)
			return -2;
		test_vlan_port = atoi(argv[i]);
		add_remote_id = 1;
	} else if (!strcmp(argv[i], "-RVID")) {
		*idx += 1;
		if (++i == argc)
			return -2;
		test_vlan_id = atoi(argv[i]);
		add_remote_id = 1;
	} else if (!strcmp(argv[i], "-Rsubid")) {
		*idx += 1;
		if (++i == argc)
			return -2;
		if (strlen(argv[i]) > MAX_SUBSCRIBE_ID_LEN - 1) {
			strncpy(test_subscribe_id, argv[i], MAX_SUBSCRIBE_ID_LEN - 1);
			test_subscribe_id[MAX_SUBSCRIBE_ID_LEN-1] = '\0';
		} else
			strcpy(test_subscribe_id, argv[i]);
	} else if (!strcmp(argv[i], "-Ra132")){
		test_add_opt132 = 1;
	}
 	return 0;
}

/*
 * return -1: error, run usage();
 * return 0: no relay interface
 * return 1: have relay interface 
 */
int nk_init_relay()
{
	struct server_list *sp = NULL;
	struct servent *ent;
	char *service_local = NULL, *service_remote = NULL;
	u_int16_t port_local = 0, port_remote = 0;
	
	/* Set default port */
	if (local_family == AF_INET) {
 		service_local = "bootps";
 		service_remote = "bootpc";
		port_local = htons(67);
 		port_remote = htons(68);
	}
#ifdef DHCPv6
	else {
		service_local = "dhcpv6-server";
		service_remote = "dhcpv6-client";
		port_local = htons(547);
		port_remote = htons(546);
	}
#endif

	if (!local_port_relay) {
		ent = getservbyname(service_local, "udp");
		if (ent)
			local_port_relay = ent->s_port;
		else
			local_port_relay = port_local;
		endservent();
	}
	
	if (local_family == AF_INET) {
		remote_port_relay = htons(ntohs(local_port_relay) + 1);
	} else {
		ent = getservbyname(service_remote, "udp");
		if (ent)
			remote_port_relay = ent->s_port;
		else
			remote_port_relay = port_remote;

		endservent();
	}

	if (local_family == AF_INET) {
		/* We need at least one server */
		if (servers_relay == NULL) {
			log_info("No servers_relay specified.");
			return 0;
		}

		int has_lower_interface = 0;
		struct interface_info *ifp;
		for (ifp = interfaces; ifp; ifp = ifp->next) {
			if (ifp->flags & INTERFACE_DOWNSTREAM) {
				has_lower_interface = 1;
				break;
			}
		}
		if (has_lower_interface == 0)
		{
			for (ifp = interfaces; ifp; ifp = ifp->next) {
				if (ifp->flags & INTERFACE_UPSTREAM)
					ifp->flags ^= INTERFACE_UPSTREAM;
			}
			log_info("No lower interface specified for relay.");
			return 0;
		}

		/* Set up the server sockaddrs. */
		for (sp = servers_relay; sp; sp = sp->next) {
			sp->to.sin_port = local_port_relay;
			sp->to.sin_family = AF_INET;
#ifdef HAVE_SA_LEN
			sp->to.sin_len = sizeof sp->to;
#endif
		}
	}
#ifdef DHCPv6
	else {
		unsigned code;

		/* We need at least one upstream and one downstream interface */
		if (upstreams == NULL && downstreams == NULL)
		{
			log_info("No lower and upper interface specified.");
			return 0;
		}
		else if (upstreams == NULL || downstreams == NULL) {
			log_info("Must specify at least one lower "
				 "and one upper interface.\n");
			return -1;
		}

		/* Set up the initial dhcp option universe called in dhcpd.c*/
		/* Check requested options. */
		code = D6O_RELAY_MSG;
		if (!option_code_hash_lookup(&requested_opts[0],
					     dhcpv6_universe.code_hash,
					     &code, 0, MDL))
			log_fatal("Unable to find the RELAY_MSG "
				  "option definition.");
		code = D6O_INTERFACE_ID;
		if (!option_code_hash_lookup(&requested_opts[1],
					     dhcpv6_universe.code_hash,
					     &code, 0, MDL))
			log_fatal("Unable to find the INTERFACE_ID "
				  "option definition.");
	}
#endif
	
	return 1;
}

static ssize_t nk_send_packet (interface, packet, raw, len, from, to, hto)
	struct interface_info *interface;
	struct packet *packet;
	struct dhcp_packet *raw;
	size_t len;
	struct in_addr from;
	struct sockaddr_in *to;
	struct hardware *hto;
{
	int result;
	int sock;
	struct sockaddr_in src;
	
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		log_fatal("(nk_send_packet) Can't create socket: %m");
	}

	memset(&src, 0, sizeof(struct sockaddr_in));
	src.sin_family = AF_INET;
	src.sin_addr.s_addr = from.s_addr;
	
	if (bind(sock, (struct sockaddr *)&src,sizeof(src)) == -1) {
		log_fatal("(nk_send_packet) Can't bind socket: %m");
	}
	
	result = sendto (sock, (char *)raw, len, 0, (struct sockaddr *)to, sizeof *to);

	if (result < 0) {
		log_error ("send_packet: %m");
		if (errno == ENETUNREACH)
			log_error ("send_packet: please consult README file%s",
				   " regarding broadcast address.");
	}
	close(sock);
	return result;
}

static void
do_relay4(struct interface_info *ip, struct dhcp_packet *packet,
	  unsigned int length, unsigned int from_port, struct iaddr from,
	  struct hardware *hfrom, int idx) {
	struct server_list *sp;
	struct sockaddr_in to;
	struct interface_info *out;
	struct hardware hto, *htop;
	struct in_addr from_source;

	if (packet->hlen > sizeof packet->chaddr) {
		log_info("Discarding packet with invalid hlen.");
		return;
	}

	/* Find the interface that corresponds to the giaddr
	   in the packet. */
	if (packet->giaddr.s_addr) {
		int i=0,j=0;
		for (out = interfaces; out; out = out->next) {
			if (!(out->flags & INTERFACE_DOWNSTREAM))
				continue;

			for (i = 0 ; i < out->address_count ; i++ ) {
				if (out->addresses[i].s_addr ==packet->giaddr.s_addr)
				{
					i = -1;
					break;
				}
				else if (out->addresses[i].s_addr == db8021Q[0].lan.s_addr)
				{
					for (j=0;j<vlan_number;j++)
					{
					      if (db8021Q[j].lan.s_addr == packet->giaddr.s_addr)
					      {
						      j = -1;
						      break;				
					      }
					}
					if (j == -1)
					      break;
				}
			}

			if (i == -1 || j==-1)
				break;
		}
	} else {
		out = NULL;
	}

	/* If it's a bootreply, forward it to the client. */
	if (packet->op == BOOTREPLY) {
		if (!(ip->flags & INTERFACE_UPSTREAM))
		{
			log_debug("drop boot-reply packet from unlike upper interface");
			return;
		}

		if (!(packet->flags & htons(BOOTP_BROADCAST)) &&
			can_unicast_without_arp(out)) {
			to.sin_addr = packet->yiaddr;
			to.sin_port = remote_port_relay;

			/* and hardware address is not broadcast */
			htop = &hto;
		} else {
			to.sin_addr.s_addr = htonl(INADDR_BROADCAST);
			to.sin_port = remote_port_relay;

			/* hardware address is broadcast */
			htop = NULL;
		}
		to.sin_family = AF_INET;
#ifdef HAVE_SA_LEN
		to.sin_len = sizeof to;
#endif

		memcpy(&hto.hbuf[1], packet->chaddr, packet->hlen);
		hto.hbuf[0] = packet->htype;
		hto.hlen = packet->hlen + 1;

		/* Wipe out the agent relay options and, if possible, figure
		   out which interface to use based on the contents of the
		   option that we put on the request to which the server is
		   replying. */
		if (!(length =
		      strip_relay_agent_options(ip, &out, packet, length)))
			return;

		if (!out) {
			log_error("Packet to bogus giaddr %s.\n",
			      inet_ntoa(packet->giaddr));
			++bogus_giaddr_drops;
			return;
		}

		if(vlan_tag && idx == -1)
			from_source=packet->giaddr;
		else
			from_source=out->addresses[0];
		if (send_packet(out, NULL, packet, length, from_source,
				&to, htop) < 0) {
			++server_packet_errors;
		} else {
			log_debug("Forwarded BOOTREPLY for %s to %s",
			       print_hw_addr(packet->htype, packet->hlen,
					      packet->chaddr),
			       inet_ntoa(to.sin_addr));

			++server_packets_relayed;
		}
		return;
	}

	/* If giaddr matches one of our addresses, ignore the packet -
	   we just sent it. */
	if (out)
		return;

	/* Now it's a boot-request, forward it to the upper interfaces. */
	if (!(ip->flags & INTERFACE_DOWNSTREAM)) {
		log_debug("drop boot-request packet from unlike lower interface");
		return;
	}

	/* Add relay agent options if indicated.   If something goes wrong,
	   drop the packet. */
	if (!(length = add_relay_agent_options(ip, packet, length,
					       ip->addresses[0])))
		return;

	if (packet->hops < max_hop_count)
		packet->hops = packet->hops + 1;
	else
		return;
	
	ssize_t send_ret;
	if ( idx == -1 )
	{
		/* If giaddr is not already set, Set it so the server can
			figure out what net it's from and so that we can later
			forward the response to the correct net.    If it's already
			set, the response will be sent directly to the relay agent
			that set giaddr, so we won't see it. */
		if (!packet->giaddr.s_addr)
			packet->giaddr = ip->addresses[0];
	
		/* Otherwise, it's a BOOTREQUEST, so forward it to all the
			servers_relay. */
		for (sp = servers_relay; sp; sp = sp->next) 
		{
			if (fallback_interface)
				send_ret = nk_send_packet(fallback_interface, NULL, packet, length, ip->addresses[0], &sp->to, NULL);
			else
				send_ret = send_packet(interfaces, NULL, packet, length, ip->addresses[0], &sp->to, NULL);
			
			if (send_ret < 0) {
				++client_packet_errors;
			} else {
				log_debug("Forwarded BOOTREQUEST for %s to %s",
				      print_hw_addr(packet->htype, packet->hlen,
						      packet->chaddr),
				      inet_ntoa(sp->to.sin_addr));
				++client_packets_relayed;
			}
		}
	}
	else
	{
		if (!packet->giaddr.s_addr)
			packet->giaddr = db8021Q[idx].lan;
		
		if (fallback_interface)
			send_ret = nk_send_packet(fallback_interface, NULL, packet, length, db8021Q[idx].lan, &db8021Q[idx].to, NULL);
		else
			send_ret = send_packet(interfaces, NULL, packet, length, db8021Q[idx].lan, &db8021Q[idx].to, NULL);
		
		if (send_ret < 0) {
			++client_packet_errors;
		} else {
			log_debug("According option132 Forwarded BOOTREQUEST for %s to %s",
			      print_hw_addr(packet->htype, packet->hlen,
					      packet->chaddr),
			      inet_ntoa(db8021Q[idx].to.sin_addr));
			++client_packets_relayed;
		}
	}
	return;
}

/* Strip any Relay Agent Information options from the DHCP packet
   option buffer.   If there is a circuit ID suboption, look up the
   outgoing interface based upon it. */

static int
strip_relay_agent_options(struct interface_info *in,
			  struct interface_info **out,
			  struct dhcp_packet *packet,
			  unsigned length) {
	int is_dhcp = 0;
	u_int8_t *op, *nextop, *sp, *max;
	int good_agent_option = 0;
	int status;

	/* If we're not adding agent options to packets, we're not taking
	   them out either. */
	if (!add_agent_options)
		return (length);

	/* If there's no cookie, it's a bootp packet, so we should just
	   forward it unchanged. */
	if (memcmp(packet->options, DHCP_OPTIONS_COOKIE, 4))
		return (length);

	max = ((u_int8_t *)packet) + length;
	sp = op = &packet->options[4];

	while (op < max) {
		switch(*op) {
			/* Skip padding... */
		      case DHO_PAD:
			if (sp != op)
				*sp = *op;
			++op;
			++sp;
			continue;

			/* If we see a message type, it's a DHCP packet. */
		      case DHO_DHCP_MESSAGE_TYPE:
			is_dhcp = 1;
			goto skip;
			break;

			/* Quit immediately if we hit an End option. */
		      case DHO_END:
			if (sp != op)
				*sp++ = *op++;
			goto out;

		      case DHO_DHCP_AGENT_OPTIONS:
			/* We shouldn't see a relay agent option in a
			   packet before we've seen the DHCP packet type,
			   but if we do, we have to leave it alone. */
			if (!is_dhcp)
				goto skip;

			/* Do not process an agent option if it exceeds the
			 * buffer.  Fail this packet.
			 */
			nextop = op + op[1] + 2;
			if (nextop > max)
				return (0);

			status = find_interface_by_agent_option(packet,
								out, op + 2,
								op[1]);
			if (status == -1 && drop_agent_mismatches)
				return (0);
			if (status)
				good_agent_option = 1;
			op = nextop;
			break;

		      skip:
			/* Skip over other options. */
		      default:
			/* Fail if processing this option will exceed the
			 * buffer(op[1] is malformed).
			 */
			nextop = op + op[1] + 2;
			if (nextop > max)
				return (0);

			if (sp != op) {
				memmove(sp, op, op[1] + 2);
				sp += op[1] + 2;
				op = nextop;
			} else
				op = sp = nextop;

			break;
		}
	}
      out:

	/* If it's not a DHCP packet, we're not supposed to touch it. */
	if (!is_dhcp)
		return (length);

	/* If none of the agent options we found matched, or if we didn't
	   find any agent options, count this packet as not having any
	   matching agent options, and if we're relying on agent options
	   to determine the outgoing interface, drop the packet. */

	if (!good_agent_option) {
		++missing_agent_option;
		if (drop_agent_mismatches)
			return (0);
	}

	/* Adjust the length... */
	if (sp != op) {
		length = sp -((u_int8_t *)packet);

		/* Make sure the packet isn't short(this is unlikely,
		   but WTH) */
		if (length < BOOTP_MIN_LEN) {
			memset(sp, DHO_PAD, BOOTP_MIN_LEN - length);
			length = BOOTP_MIN_LEN;
		}
	}
	return (length);
}


/* Find an interface that matches the circuit ID specified in the
   Relay Agent Information option.   If one is found, store it through
   the pointer given; otherwise, leave the existing pointer alone.

   We actually deviate somewhat from the current specification here:
   if the option buffer is corrupt, we suggest that the caller not
   respond to this packet.  If the circuit ID doesn't match any known
   interface, we suggest that the caller to drop the packet.  Only if
   we find a circuit ID that matches an existing interface do we tell
   the caller to go ahead and process the packet. */

static int
find_interface_by_agent_option(struct dhcp_packet *packet,
			       struct interface_info **out,
			       u_int8_t *buf, int len) {
	int i = 0;
	u_int8_t *circuit_id = 0;
	unsigned circuit_id_len = 0;
	struct interface_info *ip;

	while (i < len) {
		/* If the next agent option overflows the end of the
		   packet, the agent option buffer is corrupt. */
		if (i + 1 == len ||
		    i + buf[i + 1] + 2 > len) {
			++corrupt_agent_options;
			return (-1);
		}
		switch(buf[i]) {
			/* Remember where the circuit ID is... */
		      case RAI_CIRCUIT_ID:
			circuit_id = &buf[i + 2];
			circuit_id_len = buf[i + 1];
			i += circuit_id_len + 2;
			continue;

		      default:
			i += buf[i + 1] + 2;
			break;
		}
	}

	/* If there's no circuit ID, it's not really ours, tell the caller
	   it's no good. */
	if (!circuit_id) {
		++missing_circuit_id;
		return (-1);
	}

	/* Scan the interface list looking for an interface whose
	   name matches the one specified in circuit_id. */
	for (ip = interfaces; ip; ip = ip->next) {
		if (ip->circuit_id &&
		    ip->circuit_id_len == circuit_id_len &&
		    !memcmp(ip->circuit_id, circuit_id, circuit_id_len))
			break;
	}

	/* If we got a match, use it. */
	if (ip) {
		*out = ip;
		return (1);
	}

	/* If we didn't get a match, the circuit ID was bogus. */
	++bad_circuit_id;
	return (-1);
}

/*
 * Examine a packet to see if it's a candidate to have a Relay
 * Agent Information option tacked onto its tail.   If it is, tack
 * the option on.
 */
static int
add_relay_agent_options(struct interface_info *ip, struct dhcp_packet *packet,
			unsigned length, struct in_addr giaddr) {
	int is_dhcp = 0, mms;
	unsigned optlen;
	u_int8_t *op, *nextop, *sp, *max, *end_pad = NULL;
	int has_opt132 = 0;
	int vlan_id = test_vlan_id;
	int vlan_port = test_vlan_port;

	/* If we're not adding agent options to packets, we can skip
	   this. */
	if (!add_agent_options && !test_add_opt132)
		return (length);

	/* If there's no cookie, it's a bootp packet, so we should just
	   forward it unchanged. */
	if (memcmp(packet->options, DHCP_OPTIONS_COOKIE, 4))
		return (length);

	max = ((u_int8_t *)packet) + dhcp_max_agent_option_packet_length;

	/* Commence processing after the cookie. */
	sp = op = &packet->options[4];

	while (op < max) {
		switch(*op) {
			/* Skip padding... */
		      case DHO_PAD:
			/* Remember the first pad byte so we can commandeer
			 * padded space.
			 *
			 * XXX: Is this really a good idea?  Sure, we can
			 * seemingly reduce the packet while we're looking,
			 * but if the packet was signed by the client then
			 * this padding is part of the checksum(RFC3118),
			 * and its nonpresence would break authentication.
			 */
			if (end_pad == NULL)
				end_pad = sp;

			if (sp != op)
				*sp++ = *op++;
			else
				sp = ++op;

			continue;

			/* If we see a message type, it's a DHCP packet. */
		      case DHO_DHCP_MESSAGE_TYPE:
			is_dhcp = 1;
			goto skip;

			/*
			 * If there's a maximum message size option, we
			 * should pay attention to it
			 */
		      case DHO_DHCP_MAX_MESSAGE_SIZE:
			mms = ntohs(*(op + 2));
			if (mms < dhcp_max_agent_option_packet_length &&
			    mms >= DHCP_MTU_MIN)
				max = ((u_int8_t *)packet) + mms;
			goto skip;

			/* Quit immediately if we hit an End option. */
		      case DHO_END:
			goto out;

		      case DHO_DHCP_AGENT_OPTIONS:
			if (!add_agent_options)
				goto skip;
			/* We shouldn't see a relay agent option in a
			   packet before we've seen the DHCP packet type,
			   but if we do, we have to leave it alone. */
			if (!is_dhcp)
				goto skip;

			end_pad = NULL;

			/* There's already a Relay Agent Information option
			   in this packet.   How embarrassing.   Decide what
			   to do based on the mode the user specified. */

			switch(agent_relay_mode) {
			      case forward_and_append:
				goto skip;
			      case forward_untouched:
				return (length);
			      case discard:
				return (0);
			      case forward_and_replace:
			      default:
				break;
			}

			/* Skip over the agent option and start copying
			   if we aren't copying already. */
			op += op[1] + 2;
			break;
/* purpose     : CONFIG_NK_DHCP_SERVER_RELAY author : ChunRu date : 2011-11-03 */
/* description : add option 132 exists, parse it for remote_id of RAI          */
		      case DHO_VLAN_PORT_ID:
			vlan_id = ((unsigned int)(*(op + 2)) << 8) + (unsigned int)(*(op + 3));
			vlan_port = ((unsigned int)(*(op + 4)) << 8) + (unsigned int)(*(op + 5));
			add_remote_id = 1;
			
			if (test_add_opt132 == 1) {
				has_opt132 = 1;
				goto skip;
			}
			op += op[1] + 2;
			break;

		      skip:
			/* Skip over other options. */
		      default:
			/* Fail if processing this option will exceed the
			 * buffer(op[1] is malformed).
			 */
			nextop = op + op[1] + 2;
			if (nextop > max)
				return (0);

			end_pad = NULL;

			if (sp != op) {
				memmove(sp, op, op[1] + 2);
				sp += op[1] + 2;
				op = nextop;
			} else
				op = sp = nextop;

			break;
		}
	}
      out:

	/* If it's not a DHCP packet, we're not supposed to touch it. */
	if (!is_dhcp)
		return (length);

	/* If the packet was padded out, we can store the agent option
	   at the beginning of the padding. */

	if (end_pad != NULL)
		sp = end_pad;

	/* Remember where the end of the packet was after parsing
	   it. */
	op = sp;

/* purpose     : CONFIG_NK_DHCP_SERVER_RELAY author : ChunRu date : 2011-11-03 */
/* description : simulating switch add option 132 for testing server function  */
	if (test_add_opt132 == 1 && has_opt132 == 0) {
		optlen = 4 + 2;
		if ((optlen < 3) ||(optlen > 255))
			log_fatal("Total agent option length(%u) out of range "
				"[3 - 255] on %s\n", optlen, ip->name);
		if (max - sp >= optlen + 3) { // keep 1 byte END option
			log_debug("Adding %d-byte option-132", optlen + 3);
			*sp++ = DHO_VLAN_PORT_ID;
			*sp++ = 4;
			*sp++ = vlan_id / 256;
			*sp++ = vlan_id % 256;
			*sp++ = vlan_port / 256;
			*sp++ = vlan_port % 256;
		}
	}
	
/* purpose     : CONFIG_NK_DHCP_SERVER_RELAY author : ChunRu date : 2011-11-03 */
/* description : refactory remote_id and add subscribe_id in RAI option        */
	if (add_agent_options == 1)
	{
		if ((ip->circuit_id_len > 255) ||(ip->circuit_id_len < 1))
		log_fatal("Circuit ID length %d out of range [1-255] on "
			  "%s\n", ip->circuit_id_len, ip->name);
		optlen = ip->circuit_id_len + 2;            /* RAI_CIRCUIT_ID + len */
		if ((test_add_opt132 == 1) || (has_opt132 == 1))
			optlen += 4 + 2;
		if (strlen(test_subscribe_id) > 0)
			optlen += strlen(test_subscribe_id) + 2;
			
		if ((optlen < 3) ||(optlen > 255))
			log_fatal("Total agent option length(%u) out of range "
				"[3 - 255] on %s\n", optlen, ip->name);
		if (max - sp >= optlen + 3) {
			log_debug("Adding %d-byte relay agent option", optlen + 3);
			/* Okay, cons up *our* Relay Agent Information option. */
			*sp++ = DHO_DHCP_AGENT_OPTIONS;
			*sp++ = optlen;
			
			/* Copy in the circuit id... */
			*sp++ = RAI_CIRCUIT_ID;
			*sp++ = ip->circuit_id_len;
			memcpy(sp, ip->circuit_id, ip->circuit_id_len);
			sp += ip->circuit_id_len;
			/* Copy in the remote id... */
			if (add_remote_id == 1)
			{
				*sp++ = RAI_REMOTE_ID;
				*sp++ = 4;
				*sp++ = vlan_id / 256;
				*sp++ = vlan_id % 256;
				*sp++ = vlan_port / 256;
				*sp++ = vlan_port % 256;
			}
			/* Copy in the subscribe id... */
			if (strlen(test_subscribe_id) > 0) {
				*sp++ = RAI_SUBSCRIBE_ID;
				*sp++ = strlen(test_subscribe_id);
				memcpy(sp, test_subscribe_id, strlen(test_subscribe_id));
				sp += strlen(test_subscribe_id);
			}
		} else {
			++agent_option_errors;
			log_error("No room in packet (used %d of %d) "
				"for %d-byte relay agent option: omitted",
				(int) (sp - ((u_int8_t *) packet)),
				(int) (max - ((u_int8_t *) packet)),
				optlen + 3);
		}
	}

	/*
	 * Deposit an END option unless the packet is full (shouldn't
	 * be possible).
	 */
	if (sp < max)
		*sp++ = DHO_END;

	/* Recalculate total packet length. */
	length = sp -((u_int8_t *)packet);

	/* Make sure the packet isn't short(this is unlikely, but WTH) */
	if (length < BOOTP_MIN_LEN) {
		memset(sp, DHO_PAD, BOOTP_MIN_LEN - length);
		return (BOOTP_MIN_LEN);
	}

	return (length);
}

#ifdef DHCPv6
/*
 * Parse a downstream argument: [address%]interface[#index].
 */
static struct stream_list *
parse_downstream(char *arg) {
	struct stream_list *dp;
	struct interface_info *ifp = NULL;
	char *ifname, *addr, *iid;
	isc_result_t status;

	if (!supports_multiple_interfaces(ifp) &&
	    (downstreams != NULL))
		log_fatal("No support for multiple interfaces.");

	/* Decode the argument. */
	ifname = strchr(arg, '%');
	if (ifname == NULL) {
		ifname = arg;
		addr = NULL;
	} else {
		*ifname++ = '\0';
		addr = arg;
	}
	iid = strchr(ifname, '#');
	if (iid != NULL) {
		*iid++ = '\0';
	}
	if (strlen(ifname) >= sizeof(ifp->name)) {
		log_fatal("Interface name '%s' too long", ifname);
	}

	for (ifp = interfaces; ifp; ifp = ifp -> next) {
		// ifp may belong to lower, upper, or server interface
		if (strcmp(ifname, ifp->name) == 0)
		{
			if ((ifp->flags & INTERFACE_DOWNSTREAM) == INTERFACE_DOWNSTREAM)
				log_fatal("Down interface '%s' declared twice.", ifname);
			else
				ifp->flags |= INTERFACE_DOWNSTREAM;
			break;
		}
	}

	/* New interface. */
	if (ifp == NULL) {
		status = interface_allocate(&ifp, MDL);
		if (status != ISC_R_SUCCESS)
			log_fatal("%s: interface_allocate: %s",
				  arg, isc_result_totext(status));
		strcpy(ifp->name, ifname);
		if (interfaces) {
			interface_reference(&ifp->next, interfaces, MDL);
			interface_dereference(&interfaces, MDL);
		}
		interface_reference(&interfaces, ifp, MDL);
		ifp->flags |= INTERFACE_REQUESTED | INTERFACE_DOWNSTREAM;
	}
	
	/* New downstream. */
	dp = (struct stream_list *) dmalloc(sizeof(*dp), MDL);
	if (!dp)
		log_fatal("No memory for downstream.");
	dp->ifp = ifp;
	if (iid != NULL) {
		dp->id = atoi(iid);
	} else {
		dp->id = -1;
	}
	/* !addr case handled by setup. */
	if (addr && (inet_pton(AF_INET6, addr, &dp->link.sin6_addr) <= 0))
		log_fatal("Bad link address '%s'", addr);

	return dp;
}

/*
 * Parse an upstream argument: [address]%interface.
 */
static struct stream_list *
parse_upstream(char *arg) {
	struct stream_list *up;
	struct interface_info *ifp = NULL;
	char *ifname, *addr;
	isc_result_t status;

	/* Decode the argument. */
	ifname = strchr(arg, '%');
	if (ifname == NULL) {
		ifname = arg;
		addr = All_DHCP_Servers;
	} else {
		*ifname++ = '\0';
		addr = arg;
	}
	if (strlen(ifname) >= sizeof(ifp->name)) {
		log_fatal("Interface name '%s' too long", ifname);
	}

	for (ifp = interfaces; ifp; ifp = ifp -> next) {
		// ifp may belong to lower, upper, or server interface
		if (strcmp(ifname, ifp->name) == 0)
		{
			ifp->flags |= INTERFACE_UPSTREAM;
			break;
		}
	}

	/* New interface. */
	if (ifp == NULL) {
		status = interface_allocate(&ifp, MDL);
		if (status != ISC_R_SUCCESS)
			log_fatal("%s: interface_allocate: %s",
				  arg, isc_result_totext(status));
		strcpy(ifp->name, ifname);
		if (interfaces) {
			interface_reference(&ifp->next, interfaces, MDL);
			interface_dereference(&interfaces, MDL);
		}
		interface_reference(&interfaces, ifp, MDL);
		ifp->flags |= INTERFACE_REQUESTED | INTERFACE_UPSTREAM;
	}

	/* New upstream. */
	up = (struct stream_list *) dmalloc(sizeof(*up), MDL);
	if (up == NULL)
		log_fatal("No memory for upstream.");

	up->ifp = ifp;

	if (inet_pton(AF_INET6, addr, &up->link.sin6_addr) <= 0)
		log_fatal("Bad address %s", addr);

	return up;
}

/*
 * Setup downstream interfaces.
 */
static void
setup_streams(void) {
	struct stream_list *dp, *up;
	int i;
	isc_boolean_t link_is_set;

	for (dp = downstreams; dp; dp = dp->next) {
		/* Check interface */
		if (dp->ifp->v6address_count == 0)
			log_fatal("Interface '%s' has no IPv6 addresses.",
				  dp->ifp->name);

		/* Check/set link. */
		if (IN6_IS_ADDR_UNSPECIFIED(&dp->link.sin6_addr))
			link_is_set = ISC_FALSE;
		else
			link_is_set = ISC_TRUE;
		for (i = 0; i < dp->ifp->v6address_count; i++) {
			if (IN6_IS_ADDR_LINKLOCAL(&dp->ifp->v6addresses[i]))
				continue;
			if (!link_is_set)
				break;
			if (!memcmp(&dp->ifp->v6addresses[i],
				    &dp->link.sin6_addr,
				    sizeof(dp->link.sin6_addr)))
				break;
		}
		if (i == dp->ifp->v6address_count)
			log_fatal("Can't find link address for interface '%s'.",
				  dp->ifp->name);
		if (!link_is_set)
			memcpy(&dp->link.sin6_addr,
			       &dp->ifp->v6addresses[i],
			       sizeof(dp->link.sin6_addr));

		/* Set interface-id. */
		if (dp->id == -1)
			dp->id = dp->ifp->index;
	}

	for (up = upstreams; up; up = up->next) {
		up->link.sin6_port = local_port_relay;
		up->link.sin6_family = AF_INET6;
#ifdef HAVE_SA_LEN
		up->link.sin6_len = sizeof(up->link);
#endif

		if (up->ifp->v6address_count == 0)
			log_fatal("Interface '%s' has no IPv6 addresses.",
				  up->ifp->name);
	}
}

/*
 * Add DHCPv6 agent options here.
 */
static const int required_forw_opts[] = {
	D6O_INTERFACE_ID,
	D6O_REMOTE_ID,
	D6O_SUBSCRIBER_ID,
	D6O_RELAY_MSG,
	0
};

/*
 * Process a packet upwards, i.e., from client to server.
 */
static void
process_up6(struct packet *packet, struct stream_list *dp) {
	char forw_data[65535];
	unsigned cursor;
	struct dhcpv6_relay_packet *relay;
	struct option_state *opts;
	struct stream_list *up;

	/* Check if the message should be relayed to the server. */
	switch (packet->dhcpv6_msg_type) {
	      case DHCPV6_SOLICIT:
	      case DHCPV6_REQUEST:
	      case DHCPV6_CONFIRM:
	      case DHCPV6_RENEW:
	      case DHCPV6_REBIND:
	      case DHCPV6_RELEASE:
	      case DHCPV6_DECLINE:
	      case DHCPV6_INFORMATION_REQUEST:
	      case DHCPV6_RELAY_FORW:
	      case DHCPV6_LEASEQUERY:
		log_info("Relaying %s from %s port %d going up.",
			 dhcpv6_type_names[packet->dhcpv6_msg_type],
			 piaddr(packet->client_addr),
			 ntohs(packet->client_port));
		break;

	      case DHCPV6_ADVERTISE:
	      case DHCPV6_REPLY:
	      case DHCPV6_RECONFIGURE:
	      case DHCPV6_RELAY_REPL:
	      case DHCPV6_LEASEQUERY_REPLY:
		log_info("Discarding %s from %s port %d going up.",
			 dhcpv6_type_names[packet->dhcpv6_msg_type],
			 piaddr(packet->client_addr),
			 ntohs(packet->client_port));
		return;

	      default:
		log_info("Unknown %d type from %s port %d going up.",
			 packet->dhcpv6_msg_type,
			 piaddr(packet->client_addr),
			 ntohs(packet->client_port));
		return;
	}

	/* Build the relay-forward header. */
	relay = (struct dhcpv6_relay_packet *) forw_data;
	cursor = sizeof(*relay);
	relay->msg_type = DHCPV6_RELAY_FORW;
	if (packet->dhcpv6_msg_type == DHCPV6_RELAY_FORW) {
		if (packet->dhcpv6_hop_count >= max_hop_count) {
			log_info("Hop count exceeded,");
			return;
		}
		relay->hop_count = packet->dhcpv6_hop_count + 1;
		if (dp) {
			memcpy(&relay->link_address, &dp->link.sin6_addr, 16);
		} else {
			/* On smart relay add: && !global. */
			if (!use_if_id && downstreams->next) {
				log_info("Shan't get back the interface.");
				return;
			}
			memset(&relay->link_address, 0, 16);
		}
	} else {
		relay->hop_count = 0;
		if (!dp)
			return;
		memcpy(&relay->link_address, &dp->link.sin6_addr, 16);
	}
	memcpy(&relay->peer_address, packet->client_addr.iabuf, 16);

	/* Get an option state. */
	opts = NULL;
	if (!option_state_allocate(&opts, MDL)) {
		log_fatal("No memory for upwards options.");
	}
	
	/* Add an interface-id (if used). */
	if (use_if_id) {
		int if_id;

		if (dp) {
			if_id = dp->id;
		} else if (!downstreams->next) {
			if_id = downstreams->id;
		} else {
			log_info("Don't know the interface.");
			option_state_dereference(&opts, MDL);
			return;
		}

		if (!save_option_buffer(&dhcpv6_universe, opts,
					NULL, (unsigned char *) &if_id,
					sizeof(int),
					D6O_INTERFACE_ID, 0)) {
			log_error("Can't save interface-id.");
			option_state_dereference(&opts, MDL);
			return;
		}
	}
/* purpose     : CONFIG_NK_DHCP_SERVER_RELAY author : ChunRu date : 2011-11-03 */
/* description : add option remote_id (37) and subscribe_id (38)               */
	/* Add a remote-id (if used). */
	/* check if option132 exists */
	struct option_cache *oc;
	oc = lookup_option(&dhcpv6_universe, packet->options,
			   D6O_VLAN_PORT_ID);
	if ((oc != NULL) || (add_remote_id == 1))
	{
		struct data_string remote_id_data;
		unsigned char remote_id_buf[8];
		memset(remote_id_buf, 0x00, 4 * sizeof(unsigned char));
		if (oc != NULL) 
		{
			if (!evaluate_option_cache(&remote_id_data, packet, NULL, NULL,
					   packet->options, NULL,
					   &global_scope, oc, MDL) ||
					(remote_id_data.len != sizeof(int))) {
				log_info("Can't evaluate interface-id.");
				remote_id_buf[4] = (unsigned char)(test_vlan_id / 256);
				remote_id_buf[5] = (unsigned char)(test_vlan_id % 256);
				remote_id_buf[6] = (unsigned char)(test_vlan_port / 256);
				remote_id_buf[7] = (unsigned char)(test_vlan_port % 256);
			}
			else
				memcpy(remote_id_buf, remote_id_data.data, 4 * sizeof(unsigned char));
		}
		else
		{
			remote_id_buf[4] = (unsigned char)(test_vlan_id / 256);
			remote_id_buf[5] = (unsigned char)(test_vlan_id % 256);
			remote_id_buf[6] = (unsigned char)(test_vlan_port / 256);
			remote_id_buf[7] = (unsigned char)(test_vlan_port % 256);
		}

		if (!save_option_buffer(&dhcpv6_universe, opts,
					NULL, remote_id_buf,
					8 * sizeof(unsigned char),
					D6O_REMOTE_ID, 0)) {
			log_error("Can't save subscribe-id.");
			option_state_dereference(&opts, MDL);
			return;
		}
	}
	/* Add a subscribe-id (if used). */
	if (strlen(test_subscribe_id) > 0) {
		if (!save_option_buffer(&dhcpv6_universe, opts,
					NULL, test_subscribe_id,
					strlen(test_subscribe_id) * sizeof(unsigned char),
					D6O_SUBSCRIBER_ID, 0)) {
			log_error("Can't save subscribe-id.");
			option_state_dereference(&opts, MDL);
			return;
		}
	}

/* purpose     : CONFIG_NK_DHCP_SERVER_RELAY author : ChunRu date : 2011-11-03 */
/* description : simulating switch add option 132 for testing server function  */
	unsigned char *modified_packet;
	int modified_packet_len;
	if (test_add_opt132 == 1) {
		modified_packet_len = packet->packet_length + 8;
		modified_packet = (unsigned char *) dmalloc(modified_packet_len * sizeof(unsigned char), MDL);
		memcpy(modified_packet, packet->raw, packet->packet_length);
		*(modified_packet+modified_packet_len-8) = (unsigned char)(D6O_VLAN_PORT_ID / 256);
		*(modified_packet+modified_packet_len-7) = (unsigned char)(D6O_VLAN_PORT_ID % 256);
		*(modified_packet+modified_packet_len-6) = 0x00;
		*(modified_packet+modified_packet_len-5) = 0x04;
		*(modified_packet+modified_packet_len-4) = (unsigned char)(test_vlan_id / 256);
		*(modified_packet+modified_packet_len-3) = (unsigned char)(test_vlan_id % 256);
		*(modified_packet+modified_packet_len-2) = (unsigned char)(test_vlan_port / 256);
		*(modified_packet+modified_packet_len-1) = (unsigned char)(test_vlan_port % 256);
	} else {
		modified_packet = (unsigned char *) packet->raw;
		modified_packet_len = packet->packet_length;
	}
	if (!save_option_buffer(&dhcpv6_universe, opts,
				NULL, (unsigned char *) modified_packet,
				modified_packet_len,
				D6O_RELAY_MSG, 0)) {
		log_error("Can't save relay-msg.");
		option_state_dereference(&opts, MDL);
		return;
	}

	/* Finish the relay-forward message. */
	cursor += store_options6(forw_data + cursor,
				 sizeof(forw_data) - cursor,
				 opts, packet, 
				 required_forw_opts, NULL);
	option_state_dereference(&opts, MDL);

	/* Send it to all upstreams. */
	for (up = upstreams; up; up = up->next) {
		send_packet6(up->ifp, (unsigned char *) forw_data,
			     (size_t) cursor, &up->link);
	}
	if (test_add_opt132 == 1) {
		// TODO: check if it would not crash = =
		dfree (modified_packet, MDL);
	}
}
			     
/*
 * Process a packet downwards, i.e., from server to client.
 */
static void
process_down6(struct packet *packet) {
	struct stream_list *dp;
	struct option_cache *oc;
	struct data_string relay_msg;
	const struct dhcpv6_packet *msg;
	struct data_string if_id;
	struct sockaddr_in6 to;
	struct iaddr peer;

	/* The packet must be a relay-reply message. */
	if (packet->dhcpv6_msg_type != DHCPV6_RELAY_REPL) {
		if (packet->dhcpv6_msg_type < dhcpv6_type_name_max)
			log_info("Discarding %s from %s port %d going down.",
				 dhcpv6_type_names[packet->dhcpv6_msg_type],
				 piaddr(packet->client_addr),
				 ntohs(packet->client_port));
		else
			log_info("Unknown %d type from %s port %d going down.",
				 packet->dhcpv6_msg_type,
				 piaddr(packet->client_addr),
				 ntohs(packet->client_port));
		return;
	}

	/* Inits. */
	memset(&relay_msg, 0, sizeof(relay_msg));
	memset(&if_id, 0, sizeof(if_id));
	memset(&to, 0, sizeof(to));
	to.sin6_family = AF_INET6;
#ifdef HAVE_SA_LEN
	to.sin6_len = sizeof(to);
#endif
	to.sin6_port = remote_port_relay;
	peer.len = 16;

	/* Get the relay-msg option (carrying the message to relay). */
	oc = lookup_option(&dhcpv6_universe, packet->options, D6O_RELAY_MSG);
	if (oc == NULL) {
		log_info("No relay-msg.");
		return;
	}
	if (!evaluate_option_cache(&relay_msg, packet, NULL, NULL,
				   packet->options, NULL,
				   &global_scope, oc, MDL) ||
	    (relay_msg.len < sizeof(struct dhcpv6_packet))) {
		log_error("Can't evaluate relay-msg.");
		return;
	}
	msg = (const struct dhcpv6_packet *) relay_msg.data;

	/* Get the interface-id (if exists) and the downstream. */
	oc = lookup_option(&dhcpv6_universe, packet->options,
			   D6O_INTERFACE_ID);
	if (oc != NULL) {
		int if_index;

		if (!evaluate_option_cache(&if_id, packet, NULL, NULL,
					   packet->options, NULL,
					   &global_scope, oc, MDL) ||
		    (if_id.len != sizeof(int))) {
			log_info("Can't evaluate interface-id.");
			goto cleanup;
		}
		memcpy(&if_index, if_id.data, sizeof(int));
		for (dp = downstreams; dp; dp = dp->next) {
			if (dp->id == if_index)
				break;
		}
	} else {
		if (use_if_id) {
			/* Require an interface-id. */
			log_info("No interface-id.");
			goto cleanup;
		}
		for (dp = downstreams; dp; dp = dp->next) {
			/* Get the first matching one. */
			if (!memcmp(&dp->link.sin6_addr,
				    &packet->dhcpv6_link_address,
				    sizeof(struct in6_addr)))
				break;
		}
	}
	/* Why bother when there is no choice. */
	if (!dp && !downstreams->next)
		dp = downstreams;
	if (!dp) {
		log_info("Can't find the down interface.");
		goto cleanup;
	}
	memcpy(peer.iabuf, &packet->dhcpv6_peer_address, peer.len);
	to.sin6_addr = packet->dhcpv6_peer_address;

	/* Check if we should relay the carried message. */
	switch (msg->msg_type) {
		/* Relay-Reply of for another relay, not a client. */
	      case DHCPV6_RELAY_REPL:
		to.sin6_port = local_port_relay;
		/* Fall into: */

	      case DHCPV6_ADVERTISE:
	      case DHCPV6_REPLY:
	      case DHCPV6_RECONFIGURE:
	      case DHCPV6_RELAY_FORW:
	      case DHCPV6_LEASEQUERY_REPLY:
		log_info("Relaying %s to %s port %d down.",
			 dhcpv6_type_names[msg->msg_type],
			 piaddr(peer),
			 ntohs(to.sin6_port));
		break;

	      case DHCPV6_SOLICIT:
	      case DHCPV6_REQUEST:
	      case DHCPV6_CONFIRM:
	      case DHCPV6_RENEW:
	      case DHCPV6_REBIND:
	      case DHCPV6_RELEASE:
	      case DHCPV6_DECLINE:
	      case DHCPV6_INFORMATION_REQUEST:
	      case DHCPV6_LEASEQUERY:
		log_info("Discarding %s to %s port %d down.",
			 dhcpv6_type_names[msg->msg_type],
			 piaddr(peer),
			 ntohs(to.sin6_port));
		goto cleanup;

	      default:
		log_info("Unknown %d type to %s port %d down.",
			 msg->msg_type,
			 piaddr(peer),
			 ntohs(to.sin6_port));
		goto cleanup;
	}

	/* Send the message to the downstream. */
	send_packet6(dp->ifp, (unsigned char *) relay_msg.data,
		     (size_t) relay_msg.len, &to);

      cleanup:
	if (relay_msg.data != NULL)
		data_string_forget(&relay_msg, MDL);
	if (if_id.data != NULL)
		data_string_forget(&if_id, MDL);
}

/*
 * Called by the dispatch packet handler with a decoded packet.
 */
void
dhcpv6_relay(struct packet *packet) {
	struct stream_list *dp;

	/* Try all relay-replies downwards. */
	if (packet->dhcpv6_msg_type == DHCPV6_RELAY_REPL) {
		process_down6(packet);
		return;
	}
	/* Others are candidates to go up if they come from down. */
	for (dp = downstreams; dp; dp = dp->next) {
		if (packet->interface != dp->ifp)
			continue;
		process_up6(packet, dp);
		return;
	}
	/* Relay-forward could work from an unknown interface. */
	if (packet->dhcpv6_msg_type == DHCPV6_RELAY_FORW) {
		process_up6(packet, NULL);
		return;
	}

	log_info("Can't process packet from interface '%s'.",
		 packet->interface->name);
}
#endif

void nk_do_packet4(struct interface_info *interface, struct dhcp_packet *packet,
	  unsigned int len, unsigned int from_port, struct iaddr from,
	  struct hardware *hfrom) 
{
	struct option_cache *op;
	struct packet *decoded_packet;
#if defined (DEBUG_MEMORY_LEAKAGE)
	unsigned long previous_outstanding = dmalloc_outstanding;
#endif

#if defined (TRACING)
	trace_inpacket_stash (interface, packet, len, from_port, from, hfrom);
#endif
  	struct data_string packet_oro;	
	struct option_cache *oc;	
	char cmdBuf[256];	
	int i;

	decoded_packet = (struct packet *)0;
	if (!packet_allocate (&decoded_packet, MDL)) {
		log_error ("do_packet: no memory for incoming packet!");
		return;
	}
	decoded_packet -> raw = packet;
	decoded_packet -> packet_length = len;
	decoded_packet -> client_port = from_port;
	decoded_packet -> client_addr = from;
	interface_reference (&decoded_packet -> interface, interface, MDL);
	decoded_packet -> haddr = hfrom;

	if (packet -> hlen > sizeof packet -> chaddr) {
		packet_dereference (&decoded_packet, MDL);
		log_info ("Discarding packet with bogus hlen.");
		return;
	}

	// If there's an option buffer, try to parse it. 
	if (decoded_packet -> packet_length >= DHCP_FIXED_NON_UDP + 4) {
		if (!parse_options (decoded_packet)) {
			if (decoded_packet -> options)
				option_state_dereference
					(&decoded_packet -> options, MDL);
			packet_dereference (&decoded_packet, MDL);
			log_debug("DHCP parse_options ERROR");
			return;
		}
		else
		{
			log_debug("DHCP parse_options OK");
		}

		if (decoded_packet -> options_valid &&
		    (op = lookup_option (&dhcp_universe,
					 decoded_packet -> options, 
					 DHO_DHCP_MESSAGE_TYPE))) {
			struct data_string dp;
			memset (&dp, 0, sizeof dp);
			evaluate_option_cache (&dp, decoded_packet,
					       (struct lease *)0,
					       (struct client_state *)0,
					       decoded_packet -> options,
					       (struct option_state *)0,
					       (struct binding_scope **)0,
					       op, MDL);
			if (dp.len > 0)
				decoded_packet -> packet_type = dp.data [0];
			else
				decoded_packet -> packet_type = 0;
			data_string_forget (&dp, MDL);
		}
	}
	
	
	if (vlan_tag)
	{	
		oc = lookup_option(&dhcp_universe, decoded_packet->options, DHO_VLAN_PORT_ID); // option 132

		memset(&packet_oro,0, sizeof(packet_oro));
		
		if (oc!=NULL && evaluate_option_cache(&packet_oro, decoded_packet, NULL, NULL,decoded_packet->options, NULL,&global_scope, oc, MDL))
		{
			sprintf(cmdBuf,"%s",print_hex_1(packet_oro.len,packet_oro.data, 60));
			for (i=0;i<vlan_number;i++)
			{
				if ( !strncmp(db8021Q[i].vlan_id, cmdBuf,5) )
				{
					switch(db8021Q[i].vlan_service) //SEVER or RELAY
					{ 
					      case 1:
						if ((interface->flags & INTERFACE_SERVER) == INTERFACE_SERVER)
						{
							if (decoded_packet -> packet_type)
								dhcp (decoded_packet);
							else
								bootp (decoded_packet);
						}
						break;
					      case 2:
						if ((interface->flags & INTERFACE_STREAMS) != 0)
							do_relay4(interface, packet, len, from_port, from, hfrom, i);
						break;
					      default:
						break;
					}
					break;
				}
			}
		}
		else
			do_relay4(interface, packet, len, from_port, from, hfrom, -1);
	}
	else
	{
		if ((interface->flags & INTERFACE_SERVER) && (interface->flags & INTERFACE_STREAMS))
		{
			if (packet->op == BOOTREPLY) {
				// offer, ack, nack
				return do_relay4(interface, packet, len, from_port, from, hfrom, -1);
			}
			else
			{
				// discover, request, decline, release, inform
				if (decoded_packet -> packet_type)
					dhcp (decoded_packet);
				else
					bootp (decoded_packet);
			}
		}
		else if ((interface->flags & INTERFACE_SERVER) == INTERFACE_SERVER)
		{
			if (decoded_packet -> packet_type)
				dhcp (decoded_packet);
			else
				bootp (decoded_packet);
		}
		else
			return do_relay4(interface, packet, len, from_port, from, hfrom, -1);
	}
	
	// If the caller kept the packet, they'll have upped the refcnt. 
	packet_dereference (&decoded_packet, MDL);	
	
#if defined (DEBUG_MEMORY_LEAKAGE)
	log_info ("generation %ld: %ld new, %ld outstanding, %ld long-term",
		  dmalloc_generation,
		  dmalloc_outstanding - previous_outstanding,
		  dmalloc_outstanding, dmalloc_longterm);
#endif
#if defined (DEBUG_MEMORY_LEAKAGE)
	dmalloc_dump_outstanding ();
#endif
#if defined (DEBUG_RC_HISTORY_EXHAUSTIVELY)
	dump_rc_history (0);
#endif
}

#ifdef DHCPv6
void  nk_do_packet6(struct interface_info *interface, const char *packet, 
	   int len, int from_port, const struct iaddr *from, 
	   isc_boolean_t was_unicast) {
	unsigned char msg_type;
	const struct dhcpv6_packet *msg;
	const struct dhcpv6_relay_packet *relay; 
	struct packet *decoded_packet;
	
	struct data_string packet_oro;	
	struct option_cache *oc;	
	char cmdBuf[256];	
	int i;
	
	if (!packet6_len_okay(packet, len)) {
		log_info("do_packet6: "
			 "short packet from %s port %d, len %d, dropped",
			 piaddr(*from), from_port, len);
		return;
	}

	decoded_packet = NULL;
	if (!packet_allocate(&decoded_packet, MDL)) {
		log_error("do_packet6: no memory for incoming packet.");
		return;
	}

	if (!option_state_allocate(&decoded_packet->options, MDL)) {
		log_error("do_packet6: no memory for options.");
		packet_dereference(&decoded_packet, MDL);
		return;
	}

	/* IPv4 information, already set to 0 */
	/* decoded_packet->packet_type = 0; */
	/* memset(&decoded_packet->haddr, 0, sizeof(decoded_packet->haddr)); */
	/* decoded_packet->circuit_id = NULL; */
	/* decoded_packet->circuit_id_len = 0; */
	/* decoded_packet->remote_id = NULL; */
	/* decoded_packet->remote_id_len = 0; */
	decoded_packet->raw = (struct dhcp_packet *) packet;
	decoded_packet->packet_length = (unsigned) len;
	decoded_packet->client_port = from_port;
	decoded_packet->client_addr = *from;
	interface_reference(&decoded_packet->interface, interface, MDL);

	decoded_packet->unicast = was_unicast;

	msg_type = packet[0];
	if ((msg_type == DHCPV6_RELAY_FORW) || 
	    (msg_type == DHCPV6_RELAY_REPL)) {
		relay = (const struct dhcpv6_relay_packet *)packet;
		decoded_packet->dhcpv6_msg_type = relay->msg_type;

		/* relay-specific data */
		decoded_packet->dhcpv6_hop_count = relay->hop_count;
		memcpy(&decoded_packet->dhcpv6_link_address,
		       relay->link_address, sizeof(relay->link_address));
		memcpy(&decoded_packet->dhcpv6_peer_address,
		       relay->peer_address, sizeof(relay->peer_address));

		if (!parse_option_buffer(decoded_packet->options, 
					 relay->options, len-sizeof(*relay), 
					 &dhcpv6_universe)) {
			/* no logging here, as parse_option_buffer() logs all
			   cases where it fails */
			packet_dereference(&decoded_packet, MDL);
			return;
		}
	} else {
		msg = (const struct dhcpv6_packet *)packet;
		decoded_packet->dhcpv6_msg_type = msg->msg_type;

		/* message-specific data */
		memcpy(decoded_packet->dhcpv6_transaction_id, 
		       msg->transaction_id, 
		       sizeof(decoded_packet->dhcpv6_transaction_id));

		if (!parse_option_buffer(decoded_packet->options, 
					 msg->options, len-sizeof(*msg), 
					 &dhcpv6_universe)) {
			/* no logging here, as parse_option_buffer() logs all
			   cases where it fails */
			packet_dereference(&decoded_packet, MDL);
			return;
		}
	}
/*
	if (vlan_tag)
	{			 
		  oc = lookup_option(&dhcpv6_universe, decoded_packet->options, D6O_VLAN_PORT_ID); // option 132

		  memset(&packet_oro,0, sizeof(packet_oro));
		  
		  if (oc!=NULL && evaluate_option_cache(&packet_oro, decoded_packet, NULL, NULL,decoded_packet->options, NULL,&global_scope, oc, MDL))
		  {
			  sprintf(cmdBuf,"%s",print_hex_1(packet_oro.len,packet_oro.data, 60));
			  for (i=0;i<NK_VLANID_NUMBER;i++)
			  {
				  if ( !strncmp(db8021Q[i].vlan_id, cmdBuf,5) )
				  {
					  switch(db8021Q[i].vlan_service[0]) //SEVER or RELAY
					  { 
						case 'S':
						  if ((interface->flags & INTERFACE_SERVER) == INTERFACE_SERVER)
							dhcpv6(decoded_packet);
						  break;
						case 'R':
						  if ((interface->flags & INTERFACE_STREAMS) != 0)
							dhcpv6_relay(decoded_packet);
						  break;
						default:
						  break;
					  }
					  break;
				  }
			  }
		  }
	}
	else
*/
	{
		if ((interface->flags & INTERFACE_SERVER) && (interface->flags & INTERFACE_STREAMS))
		{
			// only relay-reply goes Relay demand
			if (msg_type == DHCPV6_RELAY_REPL)
				dhcpv6_relay(decoded_packet);
			else
				dhcpv6(decoded_packet);
		}
		else if ((interface->flags & INTERFACE_SERVER) == INTERFACE_SERVER)
			dhcpv6(decoded_packet);
		else
			dhcpv6_relay(decoded_packet);
	}
	packet_dereference(&decoded_packet, MDL);
};

void  read_8021Q_database(){
    	int i=0,mode,vid;
	struct in_addr relay_ip, lan_ip;
	char temp[128];
	char buf[512];
	char *pp,*p;
	
	kd_doCommand("8021Q_SIMPLE_LAN ENABLED", CMD_PRINT, ASH_DO_NOTHING, (char *) temp);	
	if (!strcmp(temp, "YES"))
	      vlan_tag=1;

	kd_doCommand("MULTIPLE_SUBNET NUMBER", CMD_PRINT, ASH_DO_NOTHING, temp);
	vlan_number=atoi(temp);	
	
	for (i=1; i<=vlan_number; i++)
	{
		sprintf(temp,"MULTIPLE_SUBNET ID %d",i);
		NK_db_read(temp,buf);
		
		name_get_value(buf, "VID", temp, sizeof(temp), NULL);
		vid=atoi(temp);
		
		name_get_value(buf, "DHCP_MODE", temp, sizeof(temp), NULL);
		mode=atoi(temp);
		
		name_get_value(buf, "IP", temp, sizeof(temp), NULL);
		inet_aton(temp, &lan_ip);
		
		name_get_value(buf, "RELAY", temp, sizeof(temp), NULL);
		inet_aton(temp, &relay_ip);
		/*
		pp=buf;
		
		p=strstr(pp,"VID"); 
		*p='\0';
		strcpy(temp,pp);
		
		pp=p+1;
		p=strstr(pp,"&"); 
		*p='\0';
		strcpy(temp,pp);	
		sscanf(temp,"ID=%d",&vid);	

		pp=p+1;
		p=strstr(pp,"&"); 
		*p='\0';
		strcpy(temp,pp);	
		sscanf(temp,"IP=%s",&buf);	
		inet_aton(buf, &lan_ip);
		
		pp=p+1;
		p=strstr(pp,"&DHCP_MODE"); 
		*p='\0';
		strcpy(temp,pp);	
		
		pp=p+1;
		p=strstr(pp,"&"); 
		*p='\0';
		strcpy(temp,pp);	
		sscanf(temp,"DHCP_MODE=%d",&mode);	
		
		pp=p+1;
		p=strstr(pp,"&"); 
		*p='\0';
		strcpy(temp,pp);	
		sscanf(temp,"RELAY=%s",&buf);	
		inet_aton(buf, &relay_ip);
		*/
		if (mode!=0 && vid !=0)
		{ 
		    sprintf(db8021Q[i-1].vlan_id,"%02x:%02x", vid/256, vid%256);
		    db8021Q[i-1].vlan_service=mode;  
		    db8021Q[i-1].to.sin_addr=relay_ip;
		    db8021Q[i-1].to.sin_port = local_port_relay;
		    db8021Q[i-1].to.sin_family = AF_INET;
		    db8021Q[i-1].lan=lan_ip;
		}	  
	}
}

#endif /* DHCPv6 */

#endif /* CONFIG_NK_DHCP_SERVER_RELAY */
