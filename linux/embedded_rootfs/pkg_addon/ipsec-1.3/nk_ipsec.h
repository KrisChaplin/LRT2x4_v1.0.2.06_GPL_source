

/* Here only define data structure, macro, and, constants */
/* operation funtions are declared in nk_ipsec_task.h */
#ifndef _NK_IPSEC_H_
#define _NK_IPSEC_H_

#include <netinet/in.h>
#include "nkdef.h"
#include "nkutil.h"
#include "nkuserlandconf.h"



//
#define MAX_DATABASE_LINE 1024

#ifdef CONFIG_NK_SUPPORT_USB_3G
#define WANNUM_MAX	CONFIG_NK_NUM_WAN+CONFIG_NK_NUM_DMZ+CONFIG_NK_NUM_USB
#else
#define WANNUM_MAX	CONFIG_NK_NUM_WAN+CONFIG_NK_NUM_DMZ
#endif

#define MAXACTCONNS 999
#define MAXTOTALCONNS 999
#define IFNAME_MAX 16

#define DEFAULT_NEG_ATTEMPS 0
#define DEFAULT_REKEY_MARGIN 50
#define DEFAULT_REKEY_FUZZ 20

#define NKIPSECD_IPC "/var/nk_ipsecd.fifo"
#define NKIPSEC_STATUS "/var/nk_ipsec_conn.status"
#define NK_EZLINKVPN_STATUS "/var/nk_ezlinkvpn_conn.status"
//2008/01/07 trenchen : support usbkey status update

/*purpose     : add group VPN   author : Jacky.Chen date : 2011-09-09 */
/*description : Support Group VPN */
#define NKIPSEC_GRP_STATUS "/var/nk_ipsec_grpconn.status"

#define NK_IPSEC_STATUS_USBKEY "/var/nk_ipsec_usbkey.status"
#define NK_IPSECDAEMON "/sbin/nk_ipsecd"
#define STRONGSWAN_PLUTO "/sbin/pluto"
#define STRONGSWAN_UPDOWN "/sbin/_updown"
#define PLUTOSECRETSFILE "/etc/ipsec.d/ipsec.secrets"
//#define DEBUG 1
//2008/08/06 trenchen : bug fix, cancel uniqueids, it make only one connection can eastablish
//#ifndef DEBUG
//#define START_IKED(x) system(STRONGSWAN_PLUTO " --secretsfile " PLUTOSECRETSFILE " --nofork --nat_traversal --uniqueids" x)
#define START_IKED(x) system(STRONGSWAN_PLUTO " --secretsfile " PLUTOSECRETSFILE " --nofork --nat_traversal" x)
//#else 
//#define START_IKED(x) system(STRONGSWAN_PLUTO " --secretsfile " PLUTOSECRETSFILE " --nofork --nat_traversal --uniqueids --debug-all --perpeerlog" x)
//#define START_IKED(x) system(STRONGSWAN_PLUTO " --secretsfile " PLUTOSECRETSFILE " --nofork --nat_traversal --debug-all --perpeerlog" x)
//#endif

/*purpose     : add VPN Backup author : Max.Yang date : 2011-01-24 */
/*description : Support VPN Backup */  
#define MAX_VPN_TUNNEL_NUM 1000
#define cEntry_size	1024
#define DNS_NAME_LEN	12
/*<<*/


/* NATT, DPD, PFS, BACKUP, KEEPALIVE, IP BY DNS, NETBIOS_BC, SPLIT_DNS, AGGRESSIVE, IPCOMP, AH hash, MANUAL KEYING 
	   VPN QOS, VPN ACCESSRULE, VPN LOADBALANCE */
#define IPSEC_POLICY_NATT	0x00000001
#define IPSEC_POLICY_DPD	0x00000002
#define IPSEC_POLICY_PFS	0x00000004
#define IPSEC_POLICY_BACKUP	0x00000008
#define IPSEC_POLICY_KEEPALIVE	0x00000010
#define IPSEC_POLICY_COD	0x00000020
#define IPSEC_POLICY_NETBIOSBC	0x00000040
#define IPSEC_POLICY_SPLITDNS	0x00000080
#define IPSEC_POLICY_AGGRESSIVE 0x00000100
#define IPSEC_POLICY_IPCOMP	0x00000200
#define IPSEC_POLICY_AHHASH	0x00000400
#define IPSEC_POLICY_MANUAL	0x00000800  // will be obsoleted
#define IPSEC_POLICY_QOS	0x00001000
#define IPSEC_POLICY_ACCESSRULE	0x00002000
#define IPSEC_POLICY_LOADBALANCE	0x00004000
#define IPSEC_POLICY_OPPO	0x00008000

/*2007/10/23 trenhcen : modify for support DPD function*/
#define IS_CONN_KEEP_ALIVE(x) (((x)->policy&IPSEC_POLICY_KEEPALIVE) ? TRUE: FALSE)
#define IS_CONN_PFS(x)        (((x)->policy&IPSEC_POLICY_PFS) ? TRUE: FALSE)
#define IS_CONN_DPD(x)        (((x)->policy&IPSEC_POLICY_DPD) ? TRUE: FALSE)
#define IS_CONN_NETBIOSBC(x)    (((x)->policy&IPSEC_POLICY_NETBIOSBC)? TRUE: FALSE)
#define IS_CONN_COD(x)    (((x)->policy&IPSEC_POLICY_COD)? TRUE: FALSE)
#define IS_CONN_QOS(x)    (((x)->policy&IPSEC_POLICY_COD)? TRUE: FALSE)
#define IS_CONN_ACCESSRULE(x)    (((x)->policy&IPSEC_POLICY_COD)? TRUE: FALSE)
#define IS_CONN_LB(x)    (((x)->policy&IPSEC_POLICY_COD)? TRUE: FALSE)
#define IS_CONN_IF_LINKDOWN(x)   (((x)->iface->status == LINK_DOWN) ? TRUE: FALSE)
#define IS_DNS(x)             (((x) == INET_DNS || (x) == INET_DNS4 || (x) == INET_DNS6) ? TRUE: FALSE)

/*2007/11/12 trenchen : support aggrmode*/
#define IS_CONN_AGGRESSIVE(x) (((x)->policy&IPSEC_POLICY_AGGRESSIVE) ? TRUE: FALSE)
#define IS_CONN_NATT(x) (((x)->policy&IPSEC_POLICY_NATT) ? TRUE: FALSE)
//20100103 trenchen : support split dns
#define IS_CONN_SPLITDNS(x) (((x)->policy&IPSEC_POLICY_SPLITDNS) ? TRUE: FALSE)

//20100129 charles: support IPCOMP, AUTH, Group VPN
#define IS_CONN_IPCOMP(x)        (((x)->policy&IPSEC_POLICY_IPCOMP) ? TRUE: FALSE)

//support AUTH
#define IS_CONN_AUTH(x) (((x)->policy&IPSEC_POLICY_AHHASH) ? TRUE: FALSE)

/*purpose     : add group VPN   author : Jacky.Chen date : 2011-09-09 */
/*description : Support Group VPN */
#define IS_CONN_OPPO(x)        (((x)->policy&IPSEC_POLICY_OPPO) ? TRUE: FALSE)

#define TRUE	1
#define FALSE	0  /* tricky, sometimes we need 0 to be successful sign*/

/*purpose     : bts id 12453 author : trenchen date : 2010-05-25    */
/*description : when iptables need reset rule, ipsec add it's rule  */
#define IPSEC_BUFFER_LENGTH 100
/* enum constants zone */

enum ipsec_ipc_t {
	IPSEC_IPC_NONE	=0, // internal usage
	IPSEC_IPC_ADD_CONN = 1,
	IPSEC_IPC_DEL_CONN_BYNAME = 2,   // deleting a connection only require its name
	IPSEC_IPC_INIT_CONN_BYNAME = 3,  // same as above
	IPSEC_IPC_DISCONN_BYNAME = 4,
	IPSEC_IPC_DISABLE_DB_CONN = 5, // UI request
	IPSEC_IPC_ENABLE_DB_CONN = 6,  // UI request? maybe not
	IPSEC_IPC_CONN_ISUP = 7,  // informational, someconn is up
	IPSEC_IPC_CONN_ISDOWN = 8,  //informational, someconn is down
	IPSEC_IPC_WAN_LINKDOWN = 9,
	IPSEC_IPC_WAN_LINKUP = 10,
	IPSEC_IPC_INIT_CONN = 11,
	IPSEC_IPC_QUERY_STATUS = 12,  //update tunnel status (trenchen)
	IPSEC_IPC_SHOW_LIST = 13,
	IPSEC_IPC_QUERY_STATUS_USBKEY = 14, //2008/01/07 trenchen : support usbkey status update
	IPSEC_IPC_USBKEY_DEL_GROUP = 15,  //2008/1/14 trenchen : support usbkey del group tunnel
//max add for vpn backup
//both ipsec_doi.h and nsd.c set IPSEC_IPC_NSD_LINKDOWN
	IPSEC_IPC_NSD_LINKDOWN = 16,
//both ipsec_doi.h and nsd.c.c set IPSEC_IPC_NSD_LINKUP
	IPSEC_IPC_NSD_LINKUP = 17,
//<<	
/**************************************************
 * purpose: 0012453, 0017928, 0017935, 0017941
 * author : trenchen
 * date : 2010-05-25
 * description : when iptables need reset rule, ipsec add it's rule 
 **************************************************/
	IPSEC_IPC_SET_IPTABLES_RULE = 18,
	IPSEC_IPC_MAX = 18,
};

/* INET_4 or INET_6 or doman name */
enum inet_family_t {
	INET_NONE = -1, /* dummy */
	INET_IPV4 = 0,
	INET_IPV6 = 1,
	INET_DNS  = 2, /* resolve ip by hostname */
	INET_DNS4 = 3, /* when success resolved in v4 domain */
	INET_DNS6 = 4, /* when success resolved in v6 domain */
	INET_UNSPEC = 5,
	INET_ANY = 6,
	INET_ANY4 = 7,
	INET_ANY6 = 8,
};

enum link_status_t {
	LINK_UP = 1,
	LINK_DOWN = 2,
};

enum ipsec_vpn_status_t {
	IPSEC_STATUS_SETTING_ERROR =1,
	IPSEC__STATUS_IF_DOWN,
	IPSEC_STATUS_DNS_FAILURE , // normally, this means down...
	IPSEC_STATUS_SETTING_READY,
	IPSEC_STATUS_DOWN,         //below is in listen mode
	IPSEC_STATUS_UP ,	   //below is connected
	IPSEC_STATUS_NEGO
};

enum nk_vpn_type_t {
	IPSEC_VPN_GUESS=0,  // parsing from conn name or in query string
	IPSEC_VPN_G2G = 1,
	IPSEC_VPN_C2G = 2,
	IPSEC_VPN_GRP = 3,
	IPSEC_VPN_EZLINK = 4,
	IPSEC_VPN_QKN = 5,
	IPSEC_VPN_USB = 6,
	IPSEC_VPN_BAK = 7,
	IPSEC_VPN_UNKNOWN = 99,
};

enum ike_id_t {
	IPSEC_ID_NONE        = -1,   /* special usage*/
	IPSEC_ID_IP_ADDR     = 0,
	IPSEC_ID_IP_SUBNET   = 1,
	IPSEC_ID_IP_RANGE    = 2,
	IPSEC_ID_IP_ADDR6    = 3,
	IPSEC_ID_IP_SUBNET6  = 4,
	IPSEC_ID_IP_RANGE6   = 5,
	IPSEC_ID_FQDN        = 6,
	IPSEC_ID_USR_FQDN    = 7,
	IPSEC_ID_DER_ASN1_DN = 8,
	IPSEC_ID_DER_ASN1_GN = 9,
	IPSEC_ID_KEY_ID      = 10,
	IPSEC_ID_GATEWAY_IP = 98,
	IPSEC_ID_UNKNOWN    = 99,
};

enum ike_auth_t {
	IPSEC_IKE_PRESHARED	= 0,
	IPSEC_IKE_RSASIG	= 1,
	IPSEC_IKE_X509CERT	= 2,
	IPSEC_IKE_XAUTHPRESHARED	=3, /* unsupported */
};

enum ipsec_kx_t {
	IPSEC_KM_MANUAL	= 0,
	IPSEC_KM_AUTO_IKE	=1,
	IPSEC_KM_AUTO_IKEV2	=2,
	IPSEC_KM_AUTO_KINK	=3,
	IPSEC_KM_UNKNOWN	=99,
};
#define IPSEC_KM_AUTO	IPSEC_KM_AUTO_IKE, /* for simplicity, means ikev1 */

enum ipsec_crypto_alg_t {
	IPSEC_CRYPTO_NULL	= 0,
	IPSEC_CRYPTO_DES	= 1,
	IPSEC_CRYPTO_3DES	= 2,
	IPSEC_CRYPTO_AES128	= 3,
	IPSEC_CRYPTO_AES192	= 4,
	IPSEC_CRYPTO_AES256	= 5,
	IPSEC_CRYPTO_UNKNOWN	= 99,
};

enum ipsec_hash_alg_t {
	IPSEC_AUTH_NONE	= 0,
	IPSEC_AUTH_MD5	= 1,
	IPSEC_AUTH_SHA	= 2,
	IPSEC_AUTH_DES	= 3,
	IPSEC_AUTH_UNKNOWN	= 99,
};
#define ipsec_auth_alg_t ipsec_hash_alg_t
#define ipsec_auth_alg_adv   ipsec_hash_alg_t
#define IPSEC_AUTH_HMAC_MD5	IPSEC_AUTH_MD5
#define IPSEC_AUTH_HMAC_SHA1	IPSEC_AUTH_SHA1
#define IPSEC_AUTH_HMAC_SHA	IPSEC_AUTH_SHA
#define IPSEC_AUTH_DES_MAC	IPSEC_AUTH_DES
#define IPSEC_AUTH_SHA1	IPSEC_AUTH_SHA

enum ipsec_dh_group_t {
	IPSEC_DH_NONE	=0,
	IPSEC_DH_GRP_1	=1,
	IPSEC_DH_GRP_2	=2,
	IPSEC_DH_GRP_5	=5,
	IPSEC_DH_GRP_14	=14,
	IPSEC_DH_GRP_15	=15,
	IPSEC_DH_GRP_16	=16,
	IPSEC_DH_GRP_17	=17,
	IPSEC_DH_GRP_18	=18,
};
#define IPSEC_DH_MODP768	IPSEC_DH_GRP_1
#define IPSEC_DH_MODP1024	IPSEC_DH_GRP_2
#define IPSEC_DH_MODP1536	IPSEC_DH_GRP_5
#define IPSEC_DH_MODP2048	IPSEC_DH_GRP_14
#define IPSEC_DH_MODP3072	IPSEC_DH_GRP_15
#define IPSEC_DH_MODP4096	IPSEC_DH_GRP_16
#define IPSEC_DH_MODP6144	IPSEC_DH_GRP_17
#define IPSEC_DH_MODP8192	IPSEC_DH_GRP_18

enum ipsec_dpd_action_t {
	IPSEC_DPD_CLEAR	=1,
	IPSEC_DPD_NOTHING	=2,
	IPSEC_DPD_TRAP	=3,
};
/* end of enum constants zone */

//typedef int bool;
typedef unsigned char u8;
typedef unsigned int u32;

/* forward declaration */
typedef struct if_t if_t;
typedef struct ipsec_conn_t ipsec_conn_t;
typedef struct ipsec_netbios_bc_t ipsec_netbios_bc_t ;
typedef struct grpRmt_t grpRmt_t;
/* end of forward declaration */

typedef struct {
  enum inet_family_t family;
  char *dns; /* if any */
  union {
      struct in_addr  ip;
      struct in6_addr ip6;
  } v;
} ipaddress;

struct grpRmt_t{
	ipaddress gatewayip;
	ipaddress remotelanip;
	time_t startTime;
	struct grpRmt_t *next;
};

typedef struct {
  enum inet_family_t family;
  union {
        struct in_addr ip;
        struct in6_addr ip6;
  } v;
  struct in_addr netmask; //2007/12/14 trenchen : support netbios broadcast
  int count; // prefix
} ipsubnet;

typedef struct {
  enum inet_family_t family;
  union {
        struct in_addr  ip;
        struct in6_addr ip6;
  } from;

  union {
        struct in_addr  ip;
        struct in6_addr ip6;
  } to;

  struct in_addr netmask; // netmask if range to network
  int count; // prefix if range to network
} iprange;

typedef struct {
	enum ike_id_t kind;
	union {
		ipaddress isipaddr; // ipv4/v6 address only
		ipsubnet  issubnet; // ipv4/v6 subnet
		iprange   isrange;  // ipv4/v6 range
		char *isFQDN;   // a pointer to FQDN string
		char *isUSRFQDN;  // a pointer to USRFQDN string
		void *isOTHER; // ????
	} value;
} ipsec_id_t;

typedef struct {
	u32 period;
	u32 timeout;
	u32  retries; //2007/10/23 trenchen : modify for support DPD
	enum ipsec_dpd_action_t action;
} ipsec_ike_dpd_t;

typedef struct {
	ipaddress remote_bkup_dst;
	char isMaster; // am I backup tunnel or primary one?
	ipsec_conn_t *bakconn;
	if_t *bakif;  /* backup WAN interface */
	u32 idle_time; /* ???? what's this */
} ipsec_vpn_backup_t;

struct ipsec_netbios_bc_t {
};
//20100103 trenchen : support split dns
#ifndef SPLITSTRUCT
#define SPLITSTRUCT
#define SPDNSMAXDNAME 4
#define SPDNSMAXIP 2
typedef struct ipsec_split_dns_t{
	struct ipsec_split_dns_t *next;
	//20100509 trenchen : bug fix, domain name buffer length increase to 64byte
	//char domain_name[SPDNSMAXDNAME][32];
	char domain_name[SPDNSMAXDNAME][64];
	u32 ip[SPDNSMAXIP];
	unsigned int NumDname;
	unsigned int NumIp;
	char tunnel_name[NAME_MAX];
} ipsec_split_dns_t;
#endif


typedef struct {
  /* not yet */
} ipsec_vpn_qos_t;
typedef struct {
  /* not yet */
} ipsec_vpn_accessrule_t;
typedef struct {
  /* not yet */
} ipsec_vpn_loadbalance_t;

/* automatic key exchange management (IKE) */
typedef struct {
	u8 neg_attemps; /* times of retrying initiation of connection, 0 = OO forever, never give up trying */
	u8 rekey_margin; /* default: 50 */
	u8 rekey_fuzz;   /* default: 20 */
	
	enum ike_auth_t auth_method; /* currently only preshared key is supported */
	union {
		char *psk;
		void *rsakey; /* unsupported */
		void *cert;  /* unsupported */
	} secret;
	
	ipsec_id_t local_p1id;
	ipsec_id_t remote_p1id;
	
	/* phase 1 SA */
	enum ipsec_crypto_alg_t	p1_enc;
	enum ipsec_auth_alg_t	p1_auth;
	enum ipsec_dh_group_t	p1_grp;
	u32 ikesa_lifetime;    /* time seconds */
	/* phase 2 SA */
	enum ipsec_crypto_alg_t	p2_enc;
	enum ipsec_hash_alg_t	p2_auth;
	enum ipsec_dh_group_t	pfs_grp;
	u32 ipsecsa_lifetime;  /* time seconds */
	
	ipsec_ike_dpd_t *dpdm;
	
} ipsec_conn_ike_t;

/* manual key exchange management */
typedef struct {

	char *inspi;   /* maxinum is 4, give it more... */
	char *outspi;  /* maxinum is 4, give it more... */
	enum ipsec_crypto_alg_t enc_alg;
	char *esp_enc_key;
	enum ipsec_auth_alg_t auth_alg;
	char *esp_auth_key;
	

} ipsec_conn_manual_t;

/* ipsec generic connection information structure */
struct ipsec_conn_t {

	char name[NAME_MAX];  /* unique tunnel name, g2gips2, usbips4 ...*/
	char webname[NAME_MAX];
	if_t *iface; /* pointer to which interface we are attached */
	enum nk_vpn_type_t nk_vpn_type; /* this can be parsed from name also...*/

	/* local tunnel endpoint can be retrieved from iface */
	ipsec_id_t local_client;  // should only be ip_addr, ip subnet, ip range only
	ipsec_id_t remote_client; // should only be ip_addr, ip subnet, ip range only
	ipaddress remote_tunnel_endpoint; // dynamic means family == INET_ANY
	ipaddress remote_nexthop; // if any

	grpRmt_t *grpRmtHead; //Group VPN Remote Info
	
	/* NATT, DPD, PFS, BACKUP, KEEPALIVE, IP BY DNS, NETBIOS_BC, SPLIT_DNS, AGGRESSIVE, IPCOMP, AH hash,
	   VPN QOS, VPN ACCESSRULE, VPN LOADBALANCE */
	u32 policy;
	
	enum ipsec_vpn_status_t status;  /* what is the status of connection ? */
	ipsec_vpn_backup_t *bakm;   /* Maybe manual key can use too ! */
	ipsec_netbios_bc_t *netbiosm;

	// enum hash_algorithm AHhash;  /* not support*/
//20100103 trenchen : support split dns
	 ipsec_split_dns_t sdnsm; 
	// ipsec_vpn_qos_t qosm;  /* not support yet */
	// ipsec_vpn_accessrule_t arm;  /* not support yet */
	// ipsec_vpn_loadbalance_t lbm;   /* not support yet */
	
	enum ipsec_kx_t kx_kind;
	union {
		ipsec_conn_ike_t akm;
		ipsec_conn_manual_t mkm;
	} kx;
	
	enum ipsec_auth_alg_adv auth_alg_adv;
	// david, add for Softkey schedule
	void (*timer)(void *_data);
			
	ipsec_conn_t *next;
	ipsec_conn_t *if_next;
};


typedef struct {
  u8 wichWANnum; // just for quickly matching, reference only
  ipaddress local; // we might unable to know if we are NATed...but still assume WAN IP
  ipaddress remote;
  u32 count; // how many conns uses the same pairs?
  char *pskey; // should be in string format
  /* what else you want to record here?? */
} ipsec_shared_t;

struct if_t {
  /* well, an interface might have both IPv4 and IPv6 simulateously */
  ipaddress	ip;
  ipaddress	nexthop;
  ipaddress	ip6;
  ipaddress	nexthop6;
  ipsec_conn_t *if_conns; //same interface
  char interface[IFNAME_MAX]; // "ppp0" or "eth1" ...
  u8 nwan;  // WAN number starting from 1... = index+1
  enum link_status_t status; // up/down ?
  // if_t *next; /* array declaration, no need */
};

/* global table header */
typedef struct {
   ipsec_conn_t *ac; //all connection
   ipsec_shared_t *psks; // we only record main mode preshared key...
   if_t ifs[WANNUM_MAX]; //interface information
   u32 active_conns; // current active connections(up....)
   u32 total_conns; // current_conns plus unactive connections...(excluding disabled ones)
} ipsec_t;

#endif
