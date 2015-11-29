#ifndef __NK_ETH_H__
#define __NK_ETH_H__


#include <linux/if_ether.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>


#ifndef NIPQUAD
    #define NIPQUAD(addr)       \
        ((uint8_t *)&addr)[0],  \
        ((uint8_t *)&addr)[1],  \
        ((uint8_t *)&addr)[2],  \
        ((uint8_t *)&addr)[3]
#endif

#ifndef IPC2L
    #define IPC2L(addr)     \
        ((addr[0]<<24)|(addr[1]<<16)|(addr[2]<<8)|(addr[3]))
#endif

#ifndef NMACHEX
    #define NMACHEX(haddr)       \
        ((uint8_t *)&haddr)[0],  \
        ((uint8_t *)&haddr)[1],  \
        ((uint8_t *)&haddr)[2],  \
        ((uint8_t *)&haddr)[3],  \
        ((uint8_t *)&haddr)[4],  \
        ((uint8_t *)&haddr)[5]

    #define NMACHEXL(haddr)      \
        ((uint8_t *)&haddr)[2],  \
        ((uint8_t *)&haddr)[3],  \
        ((uint8_t *)&haddr)[4],  \
        ((uint8_t *)&haddr)[5],  \
        ((uint8_t *)&haddr)[6],  \
        ((uint8_t *)&haddr)[7]
#endif

#ifndef MACC2L
    #define MACC2L(haddr)               \
        (((uint64_t)haddr[0])<<40)|     \
        (((uint64_t)haddr[1])<<32)|     \
        (((uint64_t)haddr[2])<<24)|     \
        (((uint64_t)haddr[3])<<16)|     \
        (((uint64_t)haddr[4])<<8 )|     \
        (((uint64_t)haddr[5]))
#endif

#ifndef DMACHEX
    #define DMACHEX(dmac_hi,dmac_lo)    \
            NIPQUAD(dmac_hi),           \
            (dmac_lo>>8) & 0xFF,        \
            (dmac_lo   ) & 0xFF
#endif

#ifndef SMACHEX
    #define SMACHEX(smac_hi,smac_lo)    \
            (smac_hi>>8) & 0xFF,        \
            (smac_hi   ) & 0xFF,        \
            NIPQUAD(smac_lo)
#endif

#define NK_UDP_BOOTPS     (67)
#define NK_UDP_BOOTPC     (68)

#define NK_DMAC_HI_OFFSET (16)
#define NK_SMAC_HI_OFFSET (32)
#define NK_DMAC_HI_MASK   (0xFFFFFFFF)
#define NK_DMAC_LO_MASK   (0xFFFF)
#define NK_SMAC_HI_MASK   (0xFFFF)
#define NK_SMAC_LO_MASK   (0xFFFFFFFF)


typedef uint32_t dmac_hi_t;
typedef uint16_t dmac_lo_t;
typedef uint16_t smac_hi_t;
typedef uint32_t smac_lo_t;


typedef struct
{
    dmac_hi_t dst_hi;
    dmac_lo_t dst_lo;
    smac_hi_t src_hi;
    smac_lo_t src_lo;

    union
    {
        struct
        {
            uint16_t type;
            uint8_t  pad[6];
        } s;

        struct
        {
            uint32_t hdr    : 16;
            uint32_t qos    :  4;
            uint32_t vid    : 12;

            uint16_t type;
            uint8_t  pad[2];
        } v;
    } t;
} nk_eth_t;

typedef struct
{
    uint16_t htype;
    uint16_t ptype;
    uint8_t  hsize;
    uint8_t  psize;
    uint16_t opcode;

    uint8_t smac[6];    /* sender hardware address  */
    uint8_t sip[4];     /* sender IP address        */
    uint8_t tmac[6];    /* target hardware address  */
    uint8_t tip[4];     /* target IP address        */
} nk_arp_t;


#endif
