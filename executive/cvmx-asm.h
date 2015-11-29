/***********************license start***************
 * Copyright (c) 2003-2010  Cavium Networks (support@cavium.com). All rights
 * reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.

 *   * Neither the name of Cavium Networks nor the names of
 *     its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.

 * This Software, including technical data, may be subject to U.S. export  control
 * laws, including the U.S. Export Administration Act and its  associated
 * regulations, and may be subject to export or import  regulations in other
 * countries.

 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
 * AND WITH ALL FAULTS AND CAVIUM  NETWORKS MAKES NO PROMISES, REPRESENTATIONS OR
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO
 * THE SOFTWARE, INCLUDING ITS CONDITION, ITS CONFORMITY TO ANY REPRESENTATION OR
 * DESCRIPTION, OR THE EXISTENCE OF ANY LATENT OR PATENT DEFECTS, AND CAVIUM
 * SPECIFICALLY DISCLAIMS ALL IMPLIED (IF ANY) WARRANTIES OF TITLE,
 * MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF
 * VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. THE ENTIRE  RISK ARISING OUT OF USE OR
 * PERFORMANCE OF THE SOFTWARE LIES WITH YOU.
 ***********************license end**************************************/

/**
 * @file
 *
 * This is file defines ASM primitives for the executive.

 * $Id: cvmx-asm.h 12995 2013-12-05 08:15:17Z incifer $ $Name$
 *
 *
 */
#ifndef __CVMX_ASM_H__
#define __CVMX_ASM_H__

#ifdef	__cplusplus
extern "C" {
#endif

/* other useful stuff */
#define CVMX_BREAK {asm volatile ("break");}
#define CVMX_SYNC asm volatile ("sync" : : :"memory")
#ifdef __OCTEON__
    #define CVMX_SYNCIO asm volatile ("nop")   /* Deprecated, will be removed in future release */
    #define CVMX_SYNCIOBDMA asm volatile ("synciobdma" : : :"memory")
    #define CVMX_SYNCIOALL asm volatile ("nop")   /* Deprecated, will be removed in future release */
    /* We actually use two syncw instructions in a row when we need a write
        memory barrier. This is because the CN3XXX series of Octeons have
        errata Core-401. This can cause a single syncw to not enforce
        ordering under very rare conditions. Even if it is rare, better safe
        than sorry */
    #define CVMX_SYNCW asm volatile ("syncw\nsyncw\n" : : :"memory")
#if defined(__linux__) || (defined(OCTEON_MODEL) && (OCTEON_MODEL == OCTEON_CN38XX_PASS1))
    /* Define new sync instructions to be normal SYNC instructions for pass 1 */
    #define CVMX_SYNCWS CVMX_SYNCW
    #define CVMX_SYNCS  CVMX_SYNC
#else
    /* Again, just like syncw, we may need two syncws instructions in a row due
        errata Core-401 */
    #define CVMX_SYNCWS asm volatile ("syncws\nsyncws\n" : : :"memory")
    #define CVMX_SYNCS asm volatile ("syncs" : : :"memory")
#endif
#else
    /* Not using a Cavium compiler, always use the slower sync so the assembler stays happy */
    #define CVMX_SYNCIO asm volatile ("nop")   /* Deprecated, will be removed in future release */
    #define CVMX_SYNCIOBDMA asm volatile ("sync" : : :"memory")
    #define CVMX_SYNCIOALL asm volatile ("nop")   /* Deprecated, will be removed in future release */
    #define CVMX_SYNCW asm volatile ("sync" : : :"memory")
    #define CVMX_SYNCWS CVMX_SYNCW
    #define CVMX_SYNCS  CVMX_SYNC
#endif
#define CVMX_SYNCI(address, offset) asm volatile ("synci " CVMX_TMP_STR(offset) "(%[rbase])" : : [rbase] "d" (address) )
#define CVMX_PREFETCH0(address) CVMX_PREFETCH(address, 0)
#define CVMX_PREFETCH128(address) CVMX_PREFETCH(address, 128)
// a normal prefetch
#define CVMX_PREFETCH(address, offset) CVMX_PREFETCH_PREF0(address, offset)
// normal prefetches that use the pref instruction
#define CVMX_PREFETCH_PREF0(address, offset) asm volatile ("pref 0, " CVMX_TMP_STR(offset) "(%[rbase])" : : [rbase] "d" (address) )
#define CVMX_PREFETCH_PREF1(address, offset) asm volatile ("pref 1, " CVMX_TMP_STR(offset) "(%[rbase])" : : [rbase] "d" (address) )
#define CVMX_PREFETCH_PREF6(address, offset) asm volatile ("pref 6, " CVMX_TMP_STR(offset) "(%[rbase])" : : [rbase] "d" (address) )
#define CVMX_PREFETCH_PREF7(address, offset) asm volatile ("pref 7, " CVMX_TMP_STR(offset) "(%[rbase])" : : [rbase] "d" (address) )
// prefetch into L1, do not put the block in the L2
#define CVMX_PREFETCH_NOTL2(address, offset) asm volatile ("pref 4, " CVMX_TMP_STR(offset) "(%[rbase])" : : [rbase] "d" (address) )
#define CVMX_PREFETCH_NOTL22(address, offset) asm volatile ("pref 5, " CVMX_TMP_STR(offset) "(%[rbase])" : : [rbase] "d" (address) )
// prefetch into L2, do not put the block in the L1
#define CVMX_PREFETCH_L2(address, offset) asm volatile ("pref 28, " CVMX_TMP_STR(offset) "(%[rbase])" : : [rbase] "d" (address) )
// CVMX_PREPARE_FOR_STORE makes each byte of the block unpredictable (actually old value or zero) until
// that byte is stored to (by this or another processor. Note that the value of each byte is not only
// unpredictable, but may also change again - up until the point when one of the cores stores to the
// byte.
#define CVMX_PREPARE_FOR_STORE(address, offset) asm volatile ("pref 30, " CVMX_TMP_STR(offset) "(%[rbase])" : : [rbase] "d" (address) )
// This is a command headed to the L2 controller to tell it to clear its dirty bit for a
// block. Basically, SW is telling HW that the current version of the block will not be
// used.
#define CVMX_DONT_WRITE_BACK(address, offset) asm volatile ("pref 29, " CVMX_TMP_STR(offset) "(%[rbase])" : : [rbase] "d" (address) )

#define CVMX_ICACHE_INVALIDATE  { CVMX_SYNC; asm volatile ("synci 0($0)" : : ); }    // flush stores, invalidate entire icache
#define CVMX_ICACHE_INVALIDATE2 { CVMX_SYNC; asm volatile ("cache 0, 0($0)" : : ); } // flush stores, invalidate entire icache
#define CVMX_DCACHE_INVALIDATE  { CVMX_SYNC; asm volatile ("cache 9, 0($0)" : : ); } // complete prefetches, invalidate entire dcache

/* new instruction to make RC4 run faster */
#define CVMX_BADDU(result, input1, input2) asm ("baddu %[rd],%[rs],%[rt]" : [rd] "=d" (result) : [rs] "d" (input1) , [rt] "d" (input2))

// misc v2 stuff
#define CVMX_ROTR(result, input1, shiftconst) asm ("rotr %[rd],%[rs]," CVMX_TMP_STR(shiftconst) : [rd] "=d" (result) : [rs] "d" (input1))
#define CVMX_ROTRV(result, input1, input2) asm ("rotrv %[rd],%[rt],%[rs]" : [rd] "=d" (result) : [rt] "d" (input1) , [rs] "d" (input2))
#define CVMX_DROTR(result, input1, shiftconst) asm ("drotr %[rd],%[rs]," CVMX_TMP_STR(shiftconst) : [rd] "=d" (result) : [rs] "d" (input1))
#define CVMX_DROTRV(result, input1, input2) asm ("drotrv %[rd],%[rt],%[rs]" : [rd] "=d" (result) : [rt] "d" (input1) , [rs] "d" (input2))
#define CVMX_SEB(result, input1) asm ("seb %[rd],%[rt]" : [rd] "=d" (result) : [rt] "d" (input1))
#define CVMX_SEH(result, input1) asm ("seh %[rd],%[rt]" : [rd] "=d" (result) : [rt] "d" (input1))
#define CVMX_DSBH(result, input1) asm ("dsbh %[rd],%[rt]" : [rd] "=d" (result) : [rt] "d" (input1))
#define CVMX_DSHD(result, input1) asm ("dshd %[rd],%[rt]" : [rd] "=d" (result) : [rt] "d" (input1))
#define CVMX_WSBH(result, input1) asm ("wsbh %[rd],%[rt]" : [rd] "=d" (result) : [rt] "d" (input1))

// Endian swap
#define CVMX_ES64(result, input) CVMX_DSBH(result, input); \
                                 CVMX_DSHD(result, result)
#define CVMX_ES32(result, input) CVMX_WSBH(result, input); \
                                 CVMX_ROTR(result, result, 16)

/* extract and insert - NOTE that pos and len variables must be constants! */
/* the P variants take len rather than lenm1 */
/* the M1 variants take lenm1 rather than len */
#define CVMX_EXTS(result,input,pos,lenm1) asm ("exts %[rt],%[rs]," CVMX_TMP_STR(pos) "," CVMX_TMP_STR(lenm1) : [rt] "=d" (result) : [rs] "d" (input))
#define CVMX_EXTSP(result,input,pos,len) CVMX_EXTS(result,input,pos,(len)-1)

#define CVMX_DEXT(result,input,pos,len) asm ("dext %[rt],%[rs]," CVMX_TMP_STR(pos) "," CVMX_TMP_STR(len) : [rt] "=d" (result) : [rs] "d" (input))
#define CVMX_DEXTM1(result,input,pos,lenm1) CVMX_DEXT(result,input,pos,(lenm1)+1)

#define CVMX_EXT(result,input,pos,len) asm ("ext %[rt],%[rs]," CVMX_TMP_STR(pos) "," CVMX_TMP_STR(len) : [rt] "=d" (result) : [rs] "d" (input))
#define CVMX_EXTM1(result,input,pos,lenm1) CVMX_EXT(result,input,pos,(lenm1)+1)

// removed
// #define CVMX_EXTU(result,input,pos,lenm1) asm ("extu %[rt],%[rs]," CVMX_TMP_STR(pos) "," CVMX_TMP_STR(lenm1) : [rt] "=d" (result) : [rs] "d" (input))
// #define CVMX_EXTUP(result,input,pos,len) CVMX_EXTU(result,input,pos,(len)-1)

#define CVMX_CINS(result,input,pos,lenm1) asm ("cins %[rt],%[rs]," CVMX_TMP_STR(pos) "," CVMX_TMP_STR(lenm1) : [rt] "=d" (result) : [rs] "d" (input))
#define CVMX_CINSP(result,input,pos,len) CVMX_CINS(result,input,pos,(len)-1)

#define CVMX_DINS(result,input,pos,len) asm ("dins %[rt],%[rs]," CVMX_TMP_STR(pos) "," CVMX_TMP_STR(len): [rt] "=d" (result): [rs] "d" (input), "[rt]" (result))
#define CVMX_DINSM1(result,input,pos,lenm1) CVMX_DINS(result,input,pos,(lenm1)+1)
#define CVMX_DINSC(result,pos,len) asm ("dins %[rt],$0," CVMX_TMP_STR(pos) "," CVMX_TMP_STR(len): [rt] "=d" (result): "[rt]" (result))
#define CVMX_DINSCM1(result,pos,lenm1) CVMX_DINSC(result,pos,(lenm1)+1)

#define CVMX_INS(result,input,pos,len) asm ("ins %[rt],%[rs]," CVMX_TMP_STR(pos) "," CVMX_TMP_STR(len): [rt] "=d" (result): [rs] "d" (input), "[rt]" (result))
#define CVMX_INSM1(result,input,pos,lenm1) CVMX_INS(result,input,pos,(lenm1)+1)
#define CVMX_INSC(result,pos,len) asm ("ins %[rt],$0," CVMX_TMP_STR(pos) "," CVMX_TMP_STR(len): [rt] "=d" (result): "[rt]" (result))
#define CVMX_INSCM1(result,pos,lenm1) CVMX_INSC(result,pos,(lenm1)+1)

// removed
// #define CVMX_INS0(result,input,pos,lenm1) asm("ins0 %[rt],%[rs]," CVMX_TMP_STR(pos) "," CVMX_TMP_STR(lenm1): [rt] "=d" (result): [rs] "d" (input), "[rt]" (result))
// #define CVMX_INS0P(result,input,pos,len) CVMX_INS0(result,input,pos,(len)-1)
// #define CVMX_INS0C(result,pos,lenm1) asm ("ins0 %[rt],$0," CVMX_TMP_STR(pos) "," CVMX_TMP_STR(lenm1) : [rt] "=d" (result) : "[rt]" (result))
// #define CVMX_INS0CP(result,pos,len) CVMX_INS0C(result,pos,(len)-1)

#define CVMX_CLZ(result, input) asm ("clz %[rd],%[rs]" : [rd] "=d" (result) : [rs] "d" (input))
#define CVMX_DCLZ(result, input) asm ("dclz %[rd],%[rs]" : [rd] "=d" (result) : [rs] "d" (input))
#define CVMX_CLO(result, input) asm ("clo %[rd],%[rs]" : [rd] "=d" (result) : [rs] "d" (input))
#define CVMX_DCLO(result, input) asm ("dclo %[rd],%[rs]" : [rd] "=d" (result) : [rs] "d" (input))
#define CVMX_POP(result, input) asm ("pop %[rd],%[rs]" : [rd] "=d" (result) : [rs] "d" (input))
#define CVMX_DPOP(result, input) asm ("dpop %[rd],%[rs]" : [rd] "=d" (result) : [rs] "d" (input))

// some new cop0-like stuff
#define CVMX_RDHWR(result, regstr) asm volatile ("rdhwr %[rt],$" CVMX_TMP_STR(regstr) : [rt] "=d" (result))
#define CVMX_RDHWRNV(result, regstr) asm ("rdhwr %[rt],$" CVMX_TMP_STR(regstr) : [rt] "=d" (result))
#define CVMX_DI(result) asm volatile ("di %[rt]" : [rt] "=d" (result))
#define CVMX_DI_NULL asm volatile ("di")
#define CVMX_EI(result) asm volatile ("ei %[rt]" : [rt] "=d" (result))
#define CVMX_EI_NULL asm volatile ("ei")
#define CVMX_EHB asm volatile ("ehb")

/* mul stuff */
#define CVMX_MTM0(m) asm volatile ("mtm0 %[rs]" : : [rs] "d" (m))
#define CVMX_MTM1(m) asm volatile ("mtm1 %[rs]" : : [rs] "d" (m))
#define CVMX_MTM2(m) asm volatile ("mtm2 %[rs]" : : [rs] "d" (m))
#define CVMX_MTP0(p) asm volatile ("mtp0 %[rs]" : : [rs] "d" (p))
#define CVMX_MTP1(p) asm volatile ("mtp1 %[rs]" : : [rs] "d" (p))
#define CVMX_MTP2(p) asm volatile ("mtp2 %[rs]" : : [rs] "d" (p))
// VMUL: {p3,p2,p1,p0,rd} = mpcand[63:0]*Mplier[255:0]+accum[63:0]
#define CVMX_VMUL(dest,mpcand,accum) asm volatile ("vmul %[rd],%[rs],%[rt]" : [rd] "=d" (dest) : [rs] "d" (mpcand), [rt] "d" (accum))
#define CVMX_VMULU(dest,mpcand,accum) asm volatile ("vmulu %[rd],%[rs],%[rt]" : [rd] "=d" (dest) : [rs] "d" (mpcand), [rt] "d" (accum))
// VSMUL:      {p1,p0,rd} = mpcand[63:0]*Mplier[127:0]+accum[63:0]
#define CVMX_V3MUL(dest,mpcand,accum) asm volatile ("v3mul %[rd],%[rs],%[rt]" : [rd] "=d" (dest) : [rs] "d" (mpcand), [rt] "d" (accum))
#define CVMX_V3MULU(dest,mpcand,accum) asm volatile ("v3mulu %[rd],%[rs],%[rt]" : [rd] "=d" (dest) : [rs] "d" (mpcand), [rt] "d" (accum))

/* branch stuff */
// these are hard to make work because the compiler does not realize that the
// instruction is a branch so may optimize away the label
// the labels to these next two macros must not include a ":" at the end
#define CVMX_BBIT1(var, pos, label) asm volatile ("bbit1 %[rs]," CVMX_TMP_STR(pos) "," CVMX_TMP_STR(label) : : [rs] "d" (var))
#define CVMX_BBIT0(var, pos, label) asm volatile ("bbit0 %[rs]," CVMX_TMP_STR(pos) "," CVMX_TMP_STR(label) : : [rs] "d" (var))
// the label to this macro must include a ":" at the end
#define CVMX_ASM_LABEL(label) label \
                             asm volatile (CVMX_TMP_STR(label) : : )

//
// Low-latency memory stuff
//
// set can be 0-1
#define CVMX_MT_LLM_READ_ADDR(set,val)    asm volatile ("dmtc2 %[rt],0x0400+(8*(" CVMX_TMP_STR(set) "))" : : [rt] "d" (val))
#define CVMX_MT_LLM_WRITE_ADDR_INTERNAL(set,val)   asm volatile ("dmtc2 %[rt],0x0401+(8*(" CVMX_TMP_STR(set) "))" : : [rt] "d" (val))
#define CVMX_MT_LLM_READ64_ADDR(set,val)  asm volatile ("dmtc2 %[rt],0x0404+(8*(" CVMX_TMP_STR(set) "))" : : [rt] "d" (val))
#define CVMX_MT_LLM_WRITE64_ADDR_INTERNAL(set,val) asm volatile ("dmtc2 %[rt],0x0405+(8*(" CVMX_TMP_STR(set) "))" : : [rt] "d" (val))
#define CVMX_MT_LLM_DATA(set,val)         asm volatile ("dmtc2 %[rt],0x0402+(8*(" CVMX_TMP_STR(set) "))" : : [rt] "d" (val))
#define CVMX_MF_LLM_DATA(set,val)         asm volatile ("dmfc2 %[rt],0x0402+(8*(" CVMX_TMP_STR(set) "))" : [rt] "=d" (val) : )


// load linked, store conditional
#define CVMX_LL(dest, address, offset) asm volatile ("ll %[rt], " CVMX_TMP_STR(offset) "(%[rbase])" : [rt] "=d" (dest) : [rbase] "d" (address) )
#define CVMX_LLD(dest, address, offset) asm volatile ("lld %[rt], " CVMX_TMP_STR(offset) "(%[rbase])" : [rt] "=d" (dest) : [rbase] "d" (address) )
#define CVMX_SC(srcdest, address, offset) asm volatile ("sc %[rt], " CVMX_TMP_STR(offset) "(%[rbase])" : [rt] "=d" (srcdest) : [rbase] "d" (address), "[rt]" (srcdest) )
#define CVMX_SCD(srcdest, address, offset) asm volatile ("scd %[rt], " CVMX_TMP_STR(offset) "(%[rbase])" : [rt] "=d" (srcdest) : [rbase] "d" (address), "[rt]" (srcdest) )

// load/store word left/right
#define CVMX_LWR(srcdest, address, offset) asm volatile ("lwr %[rt], " CVMX_TMP_STR(offset) "(%[rbase])" : [rt] "=d" (srcdest) : [rbase] "d" (address), "[rt]" (srcdest) )
#define CVMX_LWL(srcdest, address, offset) asm volatile ("lwl %[rt], " CVMX_TMP_STR(offset) "(%[rbase])" : [rt] "=d" (srcdest) : [rbase] "d" (address), "[rt]" (srcdest) )
#define CVMX_LDR(srcdest, address, offset) asm volatile ("ldr %[rt], " CVMX_TMP_STR(offset) "(%[rbase])" : [rt] "=d" (srcdest) : [rbase] "d" (address), "[rt]" (srcdest) )
#define CVMX_LDL(srcdest, address, offset) asm volatile ("ldl %[rt], " CVMX_TMP_STR(offset) "(%[rbase])" : [rt] "=d" (srcdest) : [rbase] "d" (address), "[rt]" (srcdest) )

#define CVMX_SWR(src, address, offset) asm volatile ("swr %[rt], " CVMX_TMP_STR(offset) "(%[rbase])" : : [rbase] "d" (address), [rt] "d" (src) )
#define CVMX_SWL(src, address, offset) asm volatile ("swl %[rt], " CVMX_TMP_STR(offset) "(%[rbase])" : : [rbase] "d" (address), [rt] "d" (src) )
#define CVMX_SDR(src, address, offset) asm volatile ("sdr %[rt], " CVMX_TMP_STR(offset) "(%[rbase])" : : [rbase] "d" (address), [rt] "d" (src) )
#define CVMX_SDL(src, address, offset) asm volatile ("sdl %[rt], " CVMX_TMP_STR(offset) "(%[rbase])" : : [rbase] "d" (address), [rt] "d" (src) )



//
// Useful crypto ASM's
//

// CRC

#define CVMX_MT_CRC_POLYNOMIAL(val)         asm volatile ("dmtc2 %[rt],0x4200" : : [rt] "d" (val))
#define CVMX_MT_CRC_IV(val)                 asm volatile ("dmtc2 %[rt],0x0201" : : [rt] "d" (val))
#define CVMX_MT_CRC_LEN(val)                asm volatile ("dmtc2 %[rt],0x1202" : : [rt] "d" (val))
#define CVMX_MT_CRC_BYTE(val)               asm volatile ("dmtc2 %[rt],0x0204" : : [rt] "d" (val))
#define CVMX_MT_CRC_HALF(val)               asm volatile ("dmtc2 %[rt],0x0205" : : [rt] "d" (val))
#define CVMX_MT_CRC_WORD(val)               asm volatile ("dmtc2 %[rt],0x0206" : : [rt] "d" (val))
#define CVMX_MT_CRC_DWORD(val)              asm volatile ("dmtc2 %[rt],0x1207" : : [rt] "d" (val))
#define CVMX_MT_CRC_VAR(val)                asm volatile ("dmtc2 %[rt],0x1208" : : [rt] "d" (val))
#define CVMX_MT_CRC_POLYNOMIAL_REFLECT(val) asm volatile ("dmtc2 %[rt],0x4210" : : [rt] "d" (val))
#define CVMX_MT_CRC_IV_REFLECT(val)         asm volatile ("dmtc2 %[rt],0x0211" : : [rt] "d" (val))
#define CVMX_MT_CRC_BYTE_REFLECT(val)       asm volatile ("dmtc2 %[rt],0x0214" : : [rt] "d" (val))
#define CVMX_MT_CRC_HALF_REFLECT(val)       asm volatile ("dmtc2 %[rt],0x0215" : : [rt] "d" (val))
#define CVMX_MT_CRC_WORD_REFLECT(val)       asm volatile ("dmtc2 %[rt],0x0216" : : [rt] "d" (val))
#define CVMX_MT_CRC_DWORD_REFLECT(val)      asm volatile ("dmtc2 %[rt],0x1217" : : [rt] "d" (val))
#define CVMX_MT_CRC_VAR_REFLECT(val)        asm volatile ("dmtc2 %[rt],0x1218" : : [rt] "d" (val))

#define CVMX_MF_CRC_POLYNOMIAL(val)         asm volatile ("dmfc2 %[rt],0x0200" : [rt] "=d" (val) : )
#define CVMX_MF_CRC_IV(val)                 asm volatile ("dmfc2 %[rt],0x0201" : [rt] "=d" (val) : )
#define CVMX_MF_CRC_IV_REFLECT(val)         asm volatile ("dmfc2 %[rt],0x0203" : [rt] "=d" (val) : )
#define CVMX_MF_CRC_LEN(val)                asm volatile ("dmfc2 %[rt],0x0202" : [rt] "=d" (val) : )

// MD5 and SHA-1

// pos can be 0-6
#define CVMX_MT_HSH_DAT(val,pos)    asm volatile ("dmtc2 %[rt],0x0040+" CVMX_TMP_STR(pos) :                 : [rt] "d" (val))
#define CVMX_MT_HSH_DATZ(pos)       asm volatile ("dmtc2    $0,0x0040+" CVMX_TMP_STR(pos) :                 :               )
// pos can be 0-14
#define CVMX_MT_HSH_DATW(val,pos)   asm volatile ("dmtc2 %[rt],0x0240+" CVMX_TMP_STR(pos) :                 : [rt] "d" (val))
#define CVMX_MT_HSH_DATWZ(pos)      asm volatile ("dmtc2    $0,0x0240+" CVMX_TMP_STR(pos) :                 :               )
#define CVMX_MT_HSH_STARTMD5(val)   asm volatile ("dmtc2 %[rt],0x4047"                   :                 : [rt] "d" (val))
#define CVMX_MT_HSH_STARTSHA(val)   asm volatile ("dmtc2 %[rt],0x4057"                   :                 : [rt] "d" (val))
#define CVMX_MT_HSH_STARTSHA256(val)   asm volatile ("dmtc2 %[rt],0x404f"                   :                 : [rt] "d" (val))
#define CVMX_MT_HSH_STARTSHA512(val)   asm volatile ("dmtc2 %[rt],0x424f"                   :                 : [rt] "d" (val))
// pos can be 0-3
#define CVMX_MT_HSH_IV(val,pos)     asm volatile ("dmtc2 %[rt],0x0048+" CVMX_TMP_STR(pos) :                 : [rt] "d" (val))
// pos can be 0-7
#define CVMX_MT_HSH_IVW(val,pos)     asm volatile ("dmtc2 %[rt],0x0250+" CVMX_TMP_STR(pos) :                 : [rt] "d" (val))

// pos can be 0-6
#define CVMX_MF_HSH_DAT(val,pos)    asm volatile ("dmfc2 %[rt],0x0040+" CVMX_TMP_STR(pos) : [rt] "=d" (val) :               )
// pos can be 0-14
#define CVMX_MF_HSH_DATW(val,pos)   asm volatile ("dmfc2 %[rt],0x0240+" CVMX_TMP_STR(pos) : [rt] "=d" (val) :               )
// pos can be 0-3
#define CVMX_MF_HSH_IV(val,pos)     asm volatile ("dmfc2 %[rt],0x0048+" CVMX_TMP_STR(pos) : [rt] "=d" (val) :               )
// pos can be 0-7
#define CVMX_MF_HSH_IVW(val,pos)     asm volatile ("dmfc2 %[rt],0x0250+" CVMX_TMP_STR(pos) : [rt] "=d" (val) :               )

// 3DES

// pos can be 0-2
#define CVMX_MT_3DES_KEY(val,pos)   asm volatile ("dmtc2 %[rt],0x0080+" CVMX_TMP_STR(pos) :                 : [rt] "d" (val))
#define CVMX_MT_3DES_IV(val)        asm volatile ("dmtc2 %[rt],0x0084"                   :                 : [rt] "d" (val))
#define CVMX_MT_3DES_ENC_CBC(val)   asm volatile ("dmtc2 %[rt],0x4088"                   :                 : [rt] "d" (val))
#define CVMX_MT_3DES_ENC(val)       asm volatile ("dmtc2 %[rt],0x408a"                   :                 : [rt] "d" (val))
#define CVMX_MT_3DES_DEC_CBC(val)   asm volatile ("dmtc2 %[rt],0x408c"                   :                 : [rt] "d" (val))
#define CVMX_MT_3DES_DEC(val)       asm volatile ("dmtc2 %[rt],0x408e"                   :                 : [rt] "d" (val))
#define CVMX_MT_3DES_RESULT(val)    asm volatile ("dmtc2 %[rt],0x0098"                   :                 : [rt] "d" (val))

// pos can be 0-2
#define CVMX_MF_3DES_KEY(val,pos)   asm volatile ("dmfc2 %[rt],0x0080+" CVMX_TMP_STR(pos) : [rt] "=d" (val) :               )
#define CVMX_MF_3DES_IV(val)        asm volatile ("dmfc2 %[rt],0x0084"                   : [rt] "=d" (val) :               )
#define CVMX_MF_3DES_RESULT(val)    asm volatile ("dmfc2 %[rt],0x0088"                   : [rt] "=d" (val) :               )

// AES

#define CVMX_MT_AES_ENC_CBC0(val)   asm volatile ("dmtc2 %[rt],0x0108"                   :                 : [rt] "d" (val))
#define CVMX_MT_AES_ENC_CBC1(val)   asm volatile ("dmtc2 %[rt],0x3109"                   :                 : [rt] "d" (val))
#define CVMX_MT_AES_ENC0(val)       asm volatile ("dmtc2 %[rt],0x010a"                   :                 : [rt] "d" (val))
#define CVMX_MT_AES_ENC1(val)       asm volatile ("dmtc2 %[rt],0x310b"                   :                 : [rt] "d" (val))
#define CVMX_MT_AES_DEC_CBC0(val)   asm volatile ("dmtc2 %[rt],0x010c"                   :                 : [rt] "d" (val))
#define CVMX_MT_AES_DEC_CBC1(val)   asm volatile ("dmtc2 %[rt],0x310d"                   :                 : [rt] "d" (val))
#define CVMX_MT_AES_DEC0(val)       asm volatile ("dmtc2 %[rt],0x010e"                   :                 : [rt] "d" (val))
#define CVMX_MT_AES_DEC1(val)       asm volatile ("dmtc2 %[rt],0x310f"                   :                 : [rt] "d" (val))
// pos can be 0-3
#define CVMX_MT_AES_KEY(val,pos)    asm volatile ("dmtc2 %[rt],0x0104+" CVMX_TMP_STR(pos) :                 : [rt] "d" (val))
// pos can be 0-1
#define CVMX_MT_AES_IV(val,pos)     asm volatile ("dmtc2 %[rt],0x0102+" CVMX_TMP_STR(pos) :                 : [rt] "d" (val))
#define CVMX_MT_AES_KEYLENGTH(val)  asm volatile ("dmtc2 %[rt],0x0110"                   :                 : [rt] "d" (val)) // write the keylen
// pos can be 0-1
#define CVMX_MT_AES_RESULT(val,pos) asm volatile ("dmtc2 %[rt],0x0100+" CVMX_TMP_STR(pos) :                 : [rt] "d" (val))

// pos can be 0-1
#define CVMX_MF_AES_RESULT(val,pos) asm volatile ("dmfc2 %[rt],0x0100+" CVMX_TMP_STR(pos) : [rt] "=d" (val) :               )
// pos can be 0-1
#define CVMX_MF_AES_IV(val,pos)     asm volatile ("dmfc2 %[rt],0x0102+" CVMX_TMP_STR(pos) : [rt] "=d" (val) :               )
// pos can be 0-3
#define CVMX_MF_AES_KEY(val,pos)    asm volatile ("dmfc2 %[rt],0x0104+" CVMX_TMP_STR(pos) : [rt] "=d" (val) :               )
#define CVMX_MF_AES_KEYLENGTH(val)  asm volatile ("dmfc2 %[rt],0x0110"                   : [rt] "=d" (val) :               ) // read the keylen
#define CVMX_MF_AES_DAT0(val)       asm volatile ("dmfc2 %[rt],0x0111"                   : [rt] "=d" (val) :               ) // first piece of input data
/* GFM COP2 macros */
/* index can be 0 or 1 */
#define CVMX_MF_GFM_MUL(val, index)     asm volatile ("dmfc2 %[rt],0x0258+" CVMX_TMP_STR(index) : [rt] "=d" (val) :               )
#define CVMX_MF_GFM_POLY(val)           asm volatile ("dmfc2 %[rt],0x025e"                      : [rt] "=d" (val) :               )
#define CVMX_MF_GFM_RESINP(val, index)  asm volatile ("dmfc2 %[rt],0x025a+" CVMX_TMP_STR(index) : [rt] "=d" (val) :               )

#define CVMX_MT_GFM_MUL(val, index)     asm volatile ("dmtc2 %[rt],0x0258+" CVMX_TMP_STR(index) :                 : [rt] "d" (val))
#define CVMX_MT_GFM_POLY(val)           asm volatile ("dmtc2 %[rt],0x025e"                      :                 : [rt] "d" (val))
#define CVMX_MT_GFM_RESINP(val, index)  asm volatile ("dmtc2 %[rt],0x025a+" CVMX_TMP_STR(index) :                 : [rt] "d" (val))
#define CVMX_MT_GFM_XOR0(val)           asm volatile ("dmtc2 %[rt],0x025c"                      :                 : [rt] "d" (val))
#define CVMX_MT_GFM_XORMUL1(val)        asm volatile ("dmtc2 %[rt],0x425d"                      :                 : [rt] "d" (val))


/* check_ordering stuff */
#if 0
#define CVMX_MF_CHORD(dest)         asm volatile ("dmfc2 %[rt],0x400" : [rt] "=d" (dest) : )
#else
#define CVMX_MF_CHORD(dest)         CVMX_RDHWR(dest, 30)
#endif

#if 0
#define CVMX_MF_CYCLE(dest)         asm volatile ("dmfc0 %[rt],$9,6" : [rt] "=d" (dest) : ) // Use (64-bit) CvmCount register rather than Count
#else
#define CVMX_MF_CYCLE(dest)         CVMX_RDHWR(dest, 31) /* reads the current (64-bit) CvmCount value */
#endif

#define CVMX_MT_CYCLE(src)         asm volatile ("dmtc0 %[rt],$9,6" : [rt] "d" (src) : )

#define CVMX_MF_CACHE_ERR(val)            asm volatile ("dmfc0 %[rt],$27,0" :  [rt] "=d" (val):)
#define CVMX_MF_DCACHE_ERR(val)           asm volatile ("dmfc0 %[rt],$27,1" :  [rt] "=d" (val):)
#define CVMX_MF_CVM_MEM_CTL(val)          asm volatile ("dmfc0 %[rt],$11,7" :  [rt] "=d" (val):)
#define CVMX_MF_CVM_CTL(val)              asm volatile ("dmfc0 %[rt],$9,7"  :  [rt] "=d" (val):)
#define CVMX_MT_CACHE_ERR(val)            asm volatile ("dmtc0 %[rt],$27,0" : : [rt] "d" (val))
#define CVMX_MT_DCACHE_ERR(val)           asm volatile ("dmtc0 %[rt],$27,1" : : [rt] "d" (val))
#define CVMX_MT_CVM_MEM_CTL(val)          asm volatile ("dmtc0 %[rt],$11,7" : : [rt] "d" (val))
#define CVMX_MT_CVM_CTL(val)              asm volatile ("dmtc0 %[rt],$9,7"  : : [rt] "d" (val))

/* Macros for TLB */
#define CVMX_TLBWI                       asm volatile ("tlbwi" : : )
#define CVMX_TLBWR                       asm volatile ("tlbwr" : : )
#define CVMX_TLBR                        asm volatile ("tlbr" : : )
#define CVMX_MT_ENTRY_HIGH(val)          asm volatile ("dmtc0 %[rt],$10,0" : : [rt] "d" (val))
#define CVMX_MT_ENTRY_LO_0(val)          asm volatile ("dmtc0 %[rt],$2,0" : : [rt] "d" (val))
#define CVMX_MT_ENTRY_LO_1(val)          asm volatile ("dmtc0 %[rt],$3,0" : : [rt] "d" (val))
#define CVMX_MT_PAGEMASK(val)            asm volatile ("mtc0 %[rt],$5,0" : : [rt] "d" (val))
#define CVMX_MT_TLB_INDEX(val)           asm volatile ("mtc0 %[rt],$0,0" : : [rt] "d" (val))
#define CVMX_MT_TLB_CONTEXT(val)         asm volatile ("dmtc0 %[rt],$4,0" : : [rt] "d" (val))
#define CVMX_MT_TLB_WIRED(val)           asm volatile ("mtc0 %[rt],$6,0" : : [rt] "d" (val))
#define CVMX_MT_TLB_RANDOM(val)          asm volatile ("mtc0 %[rt],$1,0" : : [rt] "d" (val))
#define CVMX_MF_ENTRY_LO_0(val)          asm volatile ("dmfc0 %[rt],$2,0" :  [rt] "=d" (val):)
#define CVMX_MF_ENTRY_LO_1(val)          asm volatile ("dmfc0 %[rt],$3,0" :  [rt] "=d" (val):)
#define CVMX_MF_ENTRY_HIGH(val)          asm volatile ("dmfc0 %[rt],$10,0" :  [rt] "=d" (val):)
#define CVMX_MF_PAGEMASK(val)            asm volatile ("mfc0 %[rt],$5,0" :  [rt] "=d" (val):)
#define CVMX_MF_TLB_WIRED(val)           asm volatile ("mfc0 %[rt],$6,0" :  [rt] "=d" (val):)
#define CVMX_MF_TLB_RANDOM(val)          asm volatile ("mfc0 %[rt],$1,0" :  [rt] "=d" (val):)
#define TLB_DIRTY   (0x1ULL<<2)
#define TLB_VALID   (0x1ULL<<1)
#define TLB_GLOBAL  (0x1ULL<<0)



/* assembler macros to guarantee byte loads/stores are used */
/* for an unaligned 16-bit access (these use AT register) */
/* we need the hidden argument (__a) so that GCC gets the dependencies right */
#define CVMX_LOADUNA_INT16(result, address, offset) \
	{ char *__a = (char *)(address); \
	  asm ("ulh %[rdest], " CVMX_TMP_STR(offset) "(%[rbase])" : [rdest] "=d" (result) : [rbase] "d" (__a), "m"(__a[offset]), "m"(__a[offset + 1])); }
#define CVMX_LOADUNA_UINT16(result, address, offset) \
	{ char *__a = (char *)(address); \
	  asm ("ulhu %[rdest], " CVMX_TMP_STR(offset) "(%[rbase])" : [rdest] "=d" (result) : [rbase] "d" (__a), "m"(__a[offset + 0]), "m"(__a[offset + 1])); }
#define CVMX_STOREUNA_INT16(data, address, offset) \
	{ char *__a = (char *)(address); \
	  asm ("ush %[rsrc], " CVMX_TMP_STR(offset) "(%[rbase])" : "=m"(__a[offset + 0]), "=m"(__a[offset + 1]): [rsrc] "d" (data), [rbase] "d" (__a)); }

#define CVMX_LOADUNA_INT32(result, address, offset) \
	{ char *__a = (char *)(address); \
	  asm ("ulw %[rdest], " CVMX_TMP_STR(offset) "(%[rbase])" : [rdest] "=d" (result) : \
	       [rbase] "d" (__a), "m"(__a[offset + 0]), "m"(__a[offset + 1]), "m"(__a[offset + 2]), "m"(__a[offset + 3])); }
#define CVMX_STOREUNA_INT32(data, address, offset) \
	{ char *__a = (char *)(address); \
	  asm ("usw %[rsrc], " CVMX_TMP_STR(offset) "(%[rbase])" : \
	       "=m"(__a[offset + 0]), "=m"(__a[offset + 1]), "=m"(__a[offset + 2]), "=m"(__a[offset + 3]) : \
	       [rsrc] "d" (data), [rbase] "d" (__a)); }

#define CVMX_LOADUNA_INT64(result, address, offset) \
	{ char *__a = (char *)(address); \
	  asm ("uld %[rdest], " CVMX_TMP_STR(offset) "(%[rbase])" : [rdest] "=d" (result) : \
	       [rbase] "d" (__a), "m"(__a[offset + 0]), "m"(__a[offset + 1]), "m"(__a[offset + 2]), "m"(__a[offset + 3]), \
	       "m"(__a[offset + 4]), "m"(__a[offset + 5]), "m"(__a[offset + 6]), "m"(__a[offset + 7])); }
#define CVMX_STOREUNA_INT64(data, address, offset) \
	{ char *__a = (char *)(address); \
	  asm ("usd %[rsrc], " CVMX_TMP_STR(offset) "(%[rbase])" : \
	       "=m"(__a[offset + 0]), "=m"(__a[offset + 1]), "=m"(__a[offset + 2]), "=m"(__a[offset + 3]), \
	       "=m"(__a[offset + 4]), "=m"(__a[offset + 5]), "=m"(__a[offset + 6]), "=m"(__a[offset + 7]) : \
	       [rsrc] "d" (data), [rbase] "d" (__a)); }

#ifdef	__cplusplus
}
#endif

#endif /* __CVMX_ASM_H__ */
