#ifndef __NK_SWITCH_COMMON_H__
#define __NK_SWITCH_COMMON_H__


#if defined(__KERNEL__)
#include <linux/kernel.h>
#include <linux/module.h>
#include <asm-mips/mipsregs.h>  /*  For OCTEON_IS_MODEL */
#include "cvmx.h"
#include "cvmx-csr.h"
#ifdef CONFIG_NK_DYNAMIC_PORT_NUM
#include <linux/dynamic_port_num.h>
#endif
#else
#include <stdint.h>
#endif



#ifdef CONFIG_NK_8021Q_SIMPLE
#define NK_SWITCH_MAX_DIR   4
#define NK_SWITCH_8021Q_DEBUG                   0   /* 0: disable debug message */
#define NK_SWITCH_8021Q_INFO_TYPE_VTABLE        (0x0)
#define NK_SWITCH_8021Q_INFO_TYPE_PSTATE        (0x1)
#define NK_SWITCH_8021Q_INFO_TYPE_SUBNET        (0x2)
#define NK_SWITCH_8021Q_INFO_TYPE_NUMS          (0x3)
#define NK_SWITCH_8021Q_INFO_TYPE_VTABLE_MASK   (1 << NK_SWITCH_8021Q_INFO_TYPE_VTABLE)
#define NK_SWITCH_8021Q_INFO_TYPE_PSTATE_MASK   (1 << NK_SWITCH_8021Q_INFO_TYPE_PSTATE)
#define NK_SWITCH_8021Q_INFO_TYPE_SUBNET_MASK   (1 << NK_SWITCH_8021Q_INFO_TYPE_SUBNET)
#define QTYPE_VTABLE                            NK_SWITCH_8021Q_INFO_TYPE_VTABLE
#define QTYPE_PSTATE                            NK_SWITCH_8021Q_INFO_TYPE_PSTATE
#define QTYPE_SUBNET                            NK_SWITCH_8021Q_INFO_TYPE_SUBNET
#define QTYPE_NUMS                              NK_SWITCH_8021Q_INFO_TYPE_NUMS
#define QTYPE_VTABLE_MASK                       NK_SWITCH_8021Q_INFO_TYPE_VTABLE_MASK
#define QTYPE_PSTATE_MASK                       NK_SWITCH_8021Q_INFO_TYPE_PSTATE_MASK
#define QTYPE_SUBNET_MASK                       NK_SWITCH_8021Q_INFO_TYPE_SUBNET_MASK
#define QTYPE_VTABLE_DB                         "802_1Q_VTABLE"
#define QTYPE_PSTATE_DB                         "802_1Q_PSTATE"
#define QTYPE_SUBNET_DB                         "802_1Q_SUBNET"
#define QTYPE_VTABLE_DB_NUM                     "802_1Q_VTABLE NUM"
#define QTYPE_PSTATE_DB_NUM                     "802_1Q_PSTATE NUM"
#define QTYPE_SUBNET_DB_NUM                     "802_1Q_SUBNET NUM"
#define qinfo_t                                 nk_switch_8021q_info_t
#define qvtable_t                               nk_switch_8021q_vtable_t
#define qpstate_t                               nk_switch_8021q_pstate_t
#define qsubnet_t                               nk_switch_8021q_subnet_t

#define NK_SWITCH_8021Q_MODE_UNTAG_BY_PORT      (0x0)
#define NK_SWITCH_8021Q_MODE_UNTAG_BY_VLAN      (0x1)
#define NK_SWITCH_8021Q_MODE_MIXED_VLAN         (0x2)
#define NK_SWITCH_8021Q_MODE_UNTAG_BY_VLAN_ASHLAND  (0x3)
#define QMODE_UNTAG_BY_PORT                     NK_SWITCH_8021Q_MODE_UNTAG_BY_PORT
#define QMODE_UNTAG_BY_VLAN                     NK_SWITCH_8021Q_MODE_UNTAG_BY_VLAN
#define QMODE_MIXED_VLAN                        NK_SWITCH_8021Q_MODE_MIXED_VLAN
#define QMODE_UNTAG_BY_VLAN_ASHLAND             NK_SWITCH_8021Q_MODE_UNTAG_BY_VLAN_ASHLAND

#define NK_SWITCH_8021Q_UNTAG                   (0x0)
#define NK_SWITCH_8021Q_TAGED                   (0x1)
#define QUNTAG                                  NK_SWITCH_8021Q_UNTAG
#define QTAGED                                  NK_SWITCH_8021Q_TAGED

#define QSUBNET_HASH_SIZE                       (0x100)

#define NK_ETH_8021Q_LAN_ID                     (4001)
#define NK_ETH_8021Q_WAN1_ID                    (4081)
#define NK_ETH_8021Q_VGROUP_ID                  (4051)
#define NK_SWITCH_8021Q_VID_NUMS                (4096)
#endif
#define NK_SWITCH_LAN   0
#define NK_SWITCH_WAN   1
#define NK_SWITCH_LAN2  2
#define NK_SWITCH_WAN2  3

#define NK_SWITCH_IND_MAC 7
#define NK_SWITCH_IND_PHY 8

/* For Set Port priority */
#define NK_SWITCH_PRI_0             (0)
#define NK_SWITCH_PRI_1             (1)
#define NK_SWITCH_PRI_2             (2)
#define NK_SWITCH_PRI_3             (3)
#define NK_SWITCH_PRI_4             (4)
#define NK_SWITCH_PRI_5             (5)
#define NK_SWITCH_PRI_6             (6)
#define NK_SWITCH_PRI_7             (7)

/* For Set Port Status */
#define NK_SWITCH_PORT_DISABLE      0
#define NK_SWITCH_PORT_ENABLE       1
#define NK_SWITCH_PORT_1000M        0x2
#define NK_SWITCH_PORT_100M         0x1
#define NK_SWITCH_PORT_10M          0x0
#define NK_SWITCH_PORT_FULL         0x1
#define NK_SWITCH_PORT_HALF         0x0

#define NK_SWITCH_SERI_LOW_BIT_FIRST  0x0
#define NK_SWITCH_SERI_HIGH_BIT_FIRST 0x1


#ifdef CONFIG_NK_8021Q_SIMPLE
typedef struct {
    uint32_t numbers[QTYPE_NUMS]; /* numbers of each struct     */
    uint32_t payload_size;        /* size of payload            */
    unsigned char entry[0];
} nk_switch_8021q_info_t;

typedef struct {
    uint8_t  dir;           /* 0: lan, 1: wan           */
    uint8_t  cpu;           /* 0: vlan group not include cpu port, 1: vlan group include cpu port */
    uint16_t idx;           /* vlan group index of each dir, asigned at kernel  */
    uint16_t vid;           /* vlan group vid           */
    uint32_t member;        /* vlan group member        */
    uint32_t untag_member;  /* vlan group untag member  */
} nk_switch_8021q_vtable_t;

typedef struct {
    uint8_t  dir;           /* 0: lan, 1: wan           */
    uint8_t  port;          /* front/switch port        */
    uint16_t pvid;
    uint16_t mode;          /* QUNTAG, QTAGED           */
    uint16_t idx;           /* used by MIXED_VLAN       */
} nk_switch_8021q_pstate_t;

typedef struct nk_switch_8021q_subnet {
    uint32_t ip;
    uint32_t mask;
    uint16_t vid;
    struct nk_switch_8021q_subnet *next;
} nk_switch_8021q_subnet_t;
#endif


typedef struct {
    uint32_t sw_reset;
    uint32_t sw_reset_act;
    uint32_t reset_bt;
    uint32_t reset_bt_act;

    uint32_t diag;
    uint32_t diag_act;
    uint32_t dmz;
    uint32_t dmz_act;
    uint32_t dmz_seri;      /* Is DMZ Serial, at Some Model DMZ is not a GPIO Pin */
    uint32_t vpn;           /* VPN LED have self GPIO Pin */
    uint32_t vpn_act;       /* value of make VPN led on when use VPN GPIO Pin */
    uint32_t vpn_seri;      /* control VPN led through serial,1:YES,0:NO*/
    uint32_t vpn_seri_bit;  /* which bit location can control vpn led when use vpn_seri*/

    uint32_t cs_lan;
    uint32_t cs_lan_st;
    uint32_t cs_wan;
    uint32_t cs_wan_st;

    uint32_t cs_mac; 
    uint32_t cs_phy; 
    uint32_t cs_mac_st; 
    uint32_t cs_phy_st;

    uint32_t clk;
    uint32_t sda;
    uint32_t oda;

    uint32_t    seri_clk;      /* Serial Clock, it maybe control the WAN Connect LED */
    uint32_t    seri_dat;      /* Serial Data, it maybe control the WAN Connect LED */
    uint32_t    seri_no;       /* Serial Number */
    uint32_t    seri_bit_pri;  /* Bit priority, High Bit First or Low Bit First */

    uint32_t    usb_clk;       /* USB LED Clock */
    uint32_t    usb_dat;       /* USB LED Data */
} nk_gpio_t;

typedef struct {
    uint32_t *lan2sw;/* LAN Front Port to Switch Port */
    uint32_t *wan2sw;/* WAN Front Port to Switch Port */
#ifdef CONFIG_NK_8021Q_SIMPLE
    uint32_t *fp2sp;        /* front port to switch port */
    uint32_t *fp2dir;       /* front port to dir         */
    uint32_t *fp2eth;       /* front port to eth idx     */
#endif
} nk_portmap_t;

typedef struct {
    uint32_t port;
    uint32_t index;
    uint32_t vid;
    uint32_t member;
} nk_switch_vlan_table_t;

typedef struct {
    uint32_t dir;
    uint32_t fport;
    uint32_t sport;
    uint32_t enable;
    uint32_t an;
    uint32_t speed;
    uint32_t duplex;
    uint32_t link;
    uint32_t priority;
    uint32_t inited;

    uint32_t recv_packet_cnt;
    uint32_t recv_byte_cnt;
    uint32_t tran_packet_cnt;
    uint32_t tran_byte_cnt;
    uint32_t collision_cnt;
    uint32_t error_cnt;

    nk_switch_vlan_table_t table;
} nk_switch_port_status_t;

/* purpose : 0013292 author : incifer date : 2010-10-06 */
/* description : hw mac clone                                        */
/*               mode : 0 : disable                                  */
/*                      x : wanx use hw macclone                     */
/*               mac : macclone mac                                  */
typedef struct
{
    uint8_t  mode;
    uint64_t mac;
    uint32_t sport;
} nk_switch_hw_macclone_t;

typedef struct
{
/* purpose : 0013292 author : incifer date : 2010-10-06 */
/* description : hw macclone                            */
/*               0 : not support                        */
/*               > 0 : hw macclone rule numbers         */
    uint8_t hw_macclone;
} nk_switch_opt_t;

typedef struct {
    void ( *ind_phy_read ) ( uint32_t paddr, uint32_t raddr, uint64_t *rdata, uint32_t reserve );
    void ( *ind_phy_write ) ( uint32_t paddr, uint32_t raddr, uint64_t rdata, uint32_t reserve );

    void ( *ind_phy_set_port_disable ) ( uint32_t sport, uint32_t status );
    int ( *ind_phy_get_port_status ) ( nk_switch_port_status_t *status );
    void ( *ind_phy_set_port_status ) ( nk_switch_port_status_t status );
    void ( *ind_phy_get_link_status ) ( uint32_t sport, uint32_t *status );

} nk_ind_phy_func_t;

typedef struct {
    uint32_t type;
    uint32_t switchnum;
    uint32_t lcpu_port;
    uint32_t wcpu_port;
    uint32_t mii_speed;
    uint32_t mii_clk;
    uint32_t usb_type;/* USB LED Control: 1: SERI MIX USB, share Clock */
    uint32_t wanled_type;/* WAN LED Control: 1: WAN <-> LAN LED Control, and No Connect LED, ex RV016 */
#ifdef CONFIG_NK_8021Q_SIMPLE
    uint32_t eth2pvid[32];  /* pvid of each eth device, used by ethernet */
    uint8_t *vid_2_eth;     /* vid associates to what eth_device's vid */
#endif
    uint32_t ind_phy;/*MAC and PHY of port are not in the same chip*/
    nk_switch_opt_t opt;

    nk_gpio_t *gpio;
    nk_portmap_t *portmap;
    nk_switch_vlan_table_t *vtable;
    nk_switch_port_status_t *port_status[4];
    nk_ind_phy_func_t *ind_phy_func;

    void ( *read ) ( uint32_t paddr, uint32_t raddr, uint64_t *rdata, uint32_t reserve );
    void ( *write ) ( uint32_t paddr, uint32_t raddr, uint64_t rdata, uint32_t reserve );
    void ( *pread ) ( uint32_t paddr, uint32_t raddr, uint64_t *rdata, uint32_t reserve );
    void ( *pwrite ) ( uint32_t paddr, uint32_t raddr, uint64_t rdata, uint32_t reserve );

    void ( *init_switch ) ( void );
    void ( *init_vlan ) ( void );

    void ( *set_vlan ) ( nk_switch_vlan_table_t *vtable );
    void ( *print_vlan ) ( void );

    void ( *print_pqos ) ( void );
    void ( *get_link_status ) ( uint32_t fport, uint32_t *status );
    void ( *set_port_disable ) ( uint32_t sport, uint32_t status );
    void ( *get_port_status ) ( nk_switch_port_status_t *status );
    void ( *set_port_status ) ( nk_switch_port_status_t status );
    void ( *set_port_priority ) ( nk_switch_port_status_t status );

    /* The Second Switch */
    nk_switch_vlan_table_t *vtable2;

    void ( *read2 ) ( uint32_t paddr, uint32_t raddr, uint64_t *rdata, uint32_t reserve );
    void ( *write2 ) ( uint32_t paddr, uint32_t raddr, uint64_t rdata, uint32_t reserve );
    void ( *pread2 ) ( uint32_t paddr, uint32_t raddr, uint64_t *rdata, uint32_t reserve );
    void ( *pwrite2 ) ( uint32_t paddr, uint32_t raddr, uint64_t rdata, uint32_t reserve );

    void ( *set_vlan2 ) ( nk_switch_vlan_table_t *vtable );
    void ( *print_vlan2 ) ( void );
#ifdef CONFIG_NK_8021Q_SIMPLE
    int ( *qvset ) ( uint32_t dir, int numbers, qvtable_t *vtable, uint32_t cpu_mask , int idx);
    int ( *qpset ) ( uint32_t dir, int numbers, qpstate_t *pstate, int idx );
    int ( *clear_vlan ) ( int idx );
#endif

    void ( *print_pqos2 ) ( void );
    void ( *get_link_status2 ) ( uint32_t fport, uint32_t *status );
    void ( *set_port_disable2 ) ( uint32_t sport, uint32_t status );
    void ( *get_port_status2 ) ( nk_switch_port_status_t *status );
    void ( *set_port_status2 ) ( nk_switch_port_status_t status );

    void ( *sync_speeds_phy_to_mac) ( uint32_t dir, nk_switch_port_status_t *status, uint32_t reserved );

    void ( *hw_macclone ) ( const nk_switch_hw_macclone_t *clone );
    void ( *print_hw_macclone ) ( void );
    void ( *init_STP ) ( int idx );
    void ( *init_STP2 ) ( int idx );
} nk_switch_t;


#endif

