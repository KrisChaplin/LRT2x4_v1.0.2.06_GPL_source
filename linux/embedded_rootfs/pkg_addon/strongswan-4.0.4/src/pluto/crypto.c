/* crypto interfaces
 * Copyright (C) 1998-2001  D. Hugh Redelmeier
 * Copyright (C) 2007 Andreas Steffen
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
 * RCSID $Id: crypto.c 12119 2013-07-09 13:59:35Z dio.li $
 */

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <sys/types.h>

#include <freeswan.h>
#define HEADER_DES_LOCL_H   /* stupid trick to force prototype decl in <des.h> */
#include <libdes/des.h>

#include <errno.h>

#include "constants.h"
#include "defs.h"
#include "state.h"
#include "log.h"
#include "md5.h"
#include "sha1.h"
#include "crypto.h" /* requires sha1.h and md5.h */
#include "alg_info.h"
#include "ike_alg.h"

//nclude <sys/ioctl.h>

/* moduli and generator. */

//Charles: Test for HW accel
       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>

#include "cvmx.h"

#define OCT_SUCCESS          1
#define OCT_FAILURE          0
static int oct_crypto_fd = 0;
#if _MIPS_SIM == _ABIN32
typedef uint32_t cvm_ptr_long_t;
#elif _MIPS_SIM == _ABI64
typedef uint64_t cvm_ptr_long_t;
#else
#error "Unsupported ABI"
#endif


typedef struct
{
	uint64_t sizeofptr;
	uint64_t arg1;
	uint64_t arg2;
	uint64_t arg3;
	uint64_t arg4;
	uint64_t arg5;
	int64_t arg6;
	int64_t arg7;
	int64_t arg8;
	int64_t arg9;
}cvm_crypto_op_t;

/* ioctl command */
#define CVM_MOD_EXP_BASE 0xbb

/* ioctl command */
#define CRYPT_MODEXPCRT		_IOWR(CVM_MOD_EXP_BASE,0,cvm_crypto_op_t) 
#define CRYPT_MODEXP        _IOWR(CVM_MOD_EXP_BASE,1,cvm_crypto_op_t)          
#define CRYPT_ENABLE        _IOWR(CVM_MOD_EXP_BASE,2,cvm_crypto_op_t)         
#define CRYPT_DISABLE       _IOWR(CVM_MOD_EXP_BASE,3,cvm_crypto_op_t)
#define CRYPT_IPSEC         _IOWR(CVM_MOD_EXP_BASE,4,cvm_crypto_op_t)  //Charles add to support HW IPSEC HSK


static void
crypto_display_hex (void *ptr, int len, char *msg)
{
  int i;

loglog(RC_LOG_SERIOUS,"User Space");
loglog(RC_LOG_SERIOUS,"%s Len=%d", msg, len);

  for (i = 0; i < len; i=i+8) {
    loglog(RC_LOG_SERIOUS,"%02x %02x %02x %02x  %02x %02x %02x %02x", ((uint8_t *) ptr)[i],((uint8_t *) ptr)[i+1],((uint8_t *) ptr)[i+2],((uint8_t *) ptr)[i+3]
				, ((uint8_t *) ptr)[i+4],((uint8_t *) ptr)[i+5],((uint8_t *) ptr)[i+6],((uint8_t *) ptr)[i+7]);
  }
}

static int
crypto_ipsec(u_int8_t *buf, size_t buf_len, u_int8_t *key1, u_int8_t *key2, u_int8_t *key3, size_t block_size , u_int8_t *iv, bool enc, u_int8_t algo_id)
{
   cvm_crypto_op_t tokernel;
   int ret;

   tokernel.sizeofptr = sizeof(void *);
   tokernel.arg1 = (u_int64_t)(cvm_ptr_long_t)buf;
   tokernel.arg2 = (u_int64_t)(cvm_ptr_long_t)key1;
   tokernel.arg3 = (u_int64_t)(cvm_ptr_long_t)key2;
   tokernel.arg4 = (u_int64_t)(cvm_ptr_long_t)key3;
   tokernel.arg5 = (u_int64_t)(cvm_ptr_long_t)iv;
   tokernel.arg6 = (u_int64_t)buf_len;
   tokernel.arg7 = (u_int64_t)block_size;
   tokernel.arg8 = (u_int64_t)enc;
   tokernel.arg9 = (u_int64_t)algo_id;
   ret = ioctl(oct_crypto_fd, CRYPT_IPSEC, (u_int64_t)(cvm_ptr_long_t)&tokernel);
   if (ret)
      return OCT_FAILURE;
   return OCT_SUCCESS;
}

static int
oct_crypto_init(void)
{
   if (oct_crypto_fd)
      return OCT_SUCCESS;
   oct_crypto_fd = open("/dev/octcrypto", O_RDWR);
   if (oct_crypto_fd < 0)
      return OCT_FAILURE;

printf("Octeon crypto engine for IPSec is starting...");
   return OCT_SUCCESS;
}

static int
oct_crypto_close(void)
{
   close(oct_crypto_fd);
   return OCT_SUCCESS;
}

/**********************For IPSec HW accel************************************/

static MP_INT
    modp768_modulus,
    modp1024_modulus,
    modp1536_modulus,
    modp2048_modulus,
    modp3072_modulus,
    modp4096_modulus,
    modp6144_modulus,
    modp8192_modulus;

MP_INT groupgenerator;	/* MODP group generator (2) */

static void do_3des(u_int8_t *buf, size_t buf_len, u_int8_t *key, size_t key_size, u_int8_t *iv, bool enc);
static void do_des(u_int8_t *buf, size_t buf_len, u_int8_t *key, size_t key_size, u_int8_t *iv, bool enc);

static struct encrypt_desc crypto_encryptor_des =
{ 	
	algo_type: 	IKE_ALG_ENCRYPT,
	algo_id:   	OAKLEY_DES_CBC, 
	algo_next: 	NULL,
	enc_ctxsize: 	sizeof(des_key_schedule) * 1,
	enc_blocksize: 	DES_CBC_BLOCK_SIZE, 
	keydeflen: 	DES_CBC_BLOCK_SIZE * 1 * BITS_PER_BYTE,
	keyminlen: 	DES_CBC_BLOCK_SIZE * 1 * BITS_PER_BYTE,
	keymaxlen: 	DES_CBC_BLOCK_SIZE * 1 * BITS_PER_BYTE,
	do_crypt: 	do_des,
};

static struct encrypt_desc crypto_encryptor_3des =
{ 	
	algo_type: 	IKE_ALG_ENCRYPT,
	algo_id:   	OAKLEY_3DES_CBC, 
	algo_next: 	NULL,
	enc_ctxsize: 	sizeof(des_key_schedule) * 3,
	enc_blocksize: 	DES_CBC_BLOCK_SIZE, 
	keydeflen: 	DES_CBC_BLOCK_SIZE * 3 * BITS_PER_BYTE,
	keyminlen: 	DES_CBC_BLOCK_SIZE * 3 * BITS_PER_BYTE,
	keymaxlen: 	DES_CBC_BLOCK_SIZE * 3 * BITS_PER_BYTE,
	do_crypt: 	do_3des,
};

/* MD5 hash test vectors
 * from RFC 1321 "MD5 Message-Digest Algorithm"
 * April 1992, R. Rivest, RSA Data Security
 */

static const u_char md5_test0_msg[] = {

};

static const u_char md5_test0_msg_digest[] = {
    0xd4, 0x1d, 0x8c, 0xd9, 0x8f, 0x00, 0xb2, 0x04,
    0xe9, 0x80, 0x09, 0x98, 0xec, 0xf8, 0x42, 0x7e
};

static const u_char md5_test1_msg[] = {
    0x61
};

static const u_char md5_test1_msg_digest[] = {
    0x0c, 0xc1, 0x75, 0xb9, 0xc0, 0xf1, 0xb6, 0xa8,
    0x31, 0xc3, 0x99, 0xe2, 0x69, 0x77, 0x26, 0x61
};

static const u_char md5_test2_msg[] = {
    0x61, 0x62, 0x63
};

static const u_char md5_test2_msg_digest[] = {
    0x90, 0x01, 0x50, 0x98, 0x3c, 0xd2, 0x4f, 0xb0,
    0xd6, 0x96, 0x3f, 0x7d, 0x28, 0xe1, 0x7f, 0x72
};

static const u_char md5_test3_msg[] = {
    0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x20,
    0x64, 0x69, 0x67, 0x65, 0x73, 0x74
};

static const u_char md5_test3_msg_digest[] = {
    0xf9, 0x6b, 0x69, 0x7d, 0x7c, 0xb7, 0x93, 0x8d,
    0x52, 0x5a, 0x2f, 0x31, 0xaa, 0xf1, 0x61, 0xd0
};

static const u_char md5_test4_msg[] = {
    0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
    0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70,
    0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
    0x79, 0x7a
};

static const u_char md5_test4_msg_digest[] = {
    0xc3, 0xfc, 0xd3, 0xd7, 0x61, 0x92, 0xe4, 0x00,
    0x7d, 0xfb, 0x49, 0x6c, 0xca, 0x67, 0xe1, 0x3b
};

static const u_char md5_test5_msg[] = {
    0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
    0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
    0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
    0x59, 0x5a, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
    0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e,
    0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76,
    0x77, 0x78, 0x79, 0x7a, 0x30, 0x31, 0x32, 0x33,
    0x34, 0x35, 0x36, 0x37, 0x38, 0x39
};

static const u_char md5_test5_msg_digest[] = {
    0xd1, 0x74, 0xab, 0x98, 0xd2, 0x77, 0xd9, 0xf5,
    0xa5, 0x61, 0x1c, 0x2c, 0x9f, 0x41, 0x9d, 0x9f
};

static const u_char md5_test6_msg[] = {
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
    0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36,
    0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34,
    0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32,
    0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
    0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36,
    0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34,
    0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32,
    0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30
};

static const u_char md5_test6_msg_digest[] = {
    0x57, 0xed, 0xf4, 0xa2, 0x2b, 0xe3, 0xc9, 0x55,
    0xac, 0x49, 0xda, 0x2e, 0x21, 0x07, 0xb6, 0x7a
};

static const hash_testvector_t md5_hash_testvectors[] = {
    { sizeof(md5_test0_msg), md5_test0_msg, md5_test0_msg_digest },
    { sizeof(md5_test1_msg), md5_test1_msg, md5_test1_msg_digest },
    { sizeof(md5_test2_msg), md5_test2_msg, md5_test2_msg_digest },
    { sizeof(md5_test3_msg), md5_test3_msg, md5_test3_msg_digest },
    { sizeof(md5_test4_msg), md5_test4_msg, md5_test4_msg_digest },
    { sizeof(md5_test5_msg), md5_test5_msg, md5_test5_msg_digest },
    { sizeof(md5_test6_msg), md5_test6_msg, md5_test6_msg_digest },
    { 0, NULL, NULL }
};

/* MD5 hmac test vectors
 * from RFC 2202 "Test Cases for HMAC-MD5 and HMAC-SHA-1"
 * September 1997, P. Cheng, IBM & R. Glenn, NIST
 */

static const u_char md5_hmac1_key[] = {
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b
};

static const u_char md5_hmac1_msg[] = {
    0x48, 0x69, 0x20, 0x54, 0x68, 0x65, 0x72, 0x65
};

static const u_char md5_hmac1[] = {
    0x92, 0x94, 0x72, 0x7a, 0x36, 0x38, 0xbb, 0x1c,
    0x13, 0xf4, 0x8e, 0xf8, 0x15, 0x8b, 0xfc, 0x9d
};

static const u_char md5_hmac2_key[] = {
    0x4a, 0x65, 0x66, 0x65
};

static const u_char md5_hmac2_msg[] = {
    0x77, 0x68, 0x61, 0x74, 0x20, 0x64, 0x6f, 0x20,
    0x79, 0x61, 0x20, 0x77, 0x61, 0x6e, 0x74, 0x20,
    0x66, 0x6f, 0x72, 0x20, 0x6e, 0x6f, 0x74, 0x68,
    0x69, 0x6e, 0x67, 0x3f
};

static const u_char md5_hmac2[] = {
    0x75, 0x0c, 0x78, 0x3e, 0x6a, 0xb0, 0xb5, 0x03,
    0xea, 0xa8, 0x6e, 0x31, 0x0a, 0x5d, 0xb7, 0x38
};

static const u_char md5_hmac3_key[] = {
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa
};

static const u_char md5_hmac3_msg[] = {
    0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd,
    0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd,
    0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd,
    0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd,
    0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd,
    0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd, 0xdd,
    0xdd, 0xdd
};

static const u_char md5_hmac3[] = {
    0x56, 0xbe, 0x34, 0x52, 0x1d, 0x14, 0x4c, 0x88,
    0xdb, 0xb8, 0xc7, 0x33, 0xf0, 0xe8, 0xb3, 0xf6
};

static const u_char md5_hmac4_key[] = {
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
    0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
    0x19
};

static const u_char md5_hmac4_msg[] = {
    0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd,
    0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd,
    0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd,
    0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd,
    0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd,
    0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd,
    0xcd, 0xcd 
};

static const u_char md5_hmac4[] = {
    0x69, 0x7e, 0xaf, 0x0a, 0xca, 0x3a, 0x3a, 0xea,
    0x3a, 0x75, 0x16, 0x47, 0x46, 0xff, 0xaa, 0x79
};

static const u_char md5_hmac6_key[] = {
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
};

static const u_char md5_hmac6_msg[] = {
    0x54, 0x65, 0x73, 0x74, 0x20, 0x55, 0x73, 0x69,
    0x6e, 0x67, 0x20, 0x4c, 0x61, 0x72, 0x67, 0x65,
    0x72, 0x20, 0x54, 0x68, 0x61, 0x6e, 0x20, 0x42,
    0x6c, 0x6f, 0x63, 0x6b, 0x2d, 0x53, 0x69, 0x7a,
    0x65, 0x20, 0x4b, 0x65, 0x79, 0x20, 0x2d, 0x20,
    0x48, 0x61, 0x73, 0x68, 0x20, 0x4b, 0x65, 0x79,
    0x20, 0x46, 0x69, 0x72, 0x73, 0x74
};

static const u_char md5_hmac6[] = {
    0x6b, 0x1a, 0xb7, 0xfe, 0x4b, 0xd7, 0xbf, 0x8f,
    0x0b, 0x62, 0xe6, 0xce, 0x61, 0xb9, 0xd0, 0xcd
};

static const u_char md5_hmac7_msg[] = {
    0x54, 0x65, 0x73, 0x74, 0x20, 0x55, 0x73, 0x69,
    0x6e, 0x67, 0x20, 0x4c, 0x61, 0x72, 0x67, 0x65,
    0x72, 0x20, 0x54, 0x68, 0x61, 0x6e, 0x20, 0x42,
    0x6c, 0x6f, 0x63, 0x6b, 0x2d, 0x53, 0x69, 0x7a,
    0x65, 0x20, 0x4b, 0x65, 0x79, 0x20, 0x61, 0x6e,
    0x64, 0x20, 0x4c, 0x61, 0x72, 0x67, 0x65, 0x72,
    0x20, 0x54, 0x68, 0x61, 0x6e, 0x20, 0x4f, 0x6e,
    0x65, 0x20, 0x42, 0x6c, 0x6f, 0x63, 0x6b, 0x2d,
    0x53, 0x69, 0x7a, 0x65, 0x20, 0x44, 0x61, 0x74,
    0x61
};

static const u_char md5_hmac7[] = {
    0x6f, 0x63, 0x0f, 0xad, 0x67, 0xcd, 0xa0, 0xee,
    0x1f, 0xb1, 0xf5, 0x62, 0xdb, 0x3a, 0xa5, 0x3e
};

static const hmac_testvector_t md5_hmac_testvectors[] = {
    { sizeof(md5_hmac1_key), md5_hmac1_key, sizeof(md5_hmac1_msg), md5_hmac1_msg, md5_hmac1 },
    { sizeof(md5_hmac2_key), md5_hmac2_key, sizeof(md5_hmac2_msg), md5_hmac2_msg, md5_hmac2 },
    { sizeof(md5_hmac3_key), md5_hmac3_key, sizeof(md5_hmac3_msg), md5_hmac3_msg, md5_hmac3 },
    { sizeof(md5_hmac4_key), md5_hmac4_key, sizeof(md5_hmac4_msg), md5_hmac4_msg, md5_hmac4 },
    { sizeof(md5_hmac6_key), md5_hmac6_key, sizeof(md5_hmac6_msg), md5_hmac6_msg, md5_hmac6 },
    { sizeof(md5_hmac6_key), md5_hmac6_key, sizeof(md5_hmac7_msg), md5_hmac7_msg, md5_hmac7 },
    { 0, NULL, 0, NULL, NULL }
};

static struct hash_desc crypto_hasher_md5 =
{ 	
	algo_type: IKE_ALG_HASH,
	algo_id:   OAKLEY_MD5,
	algo_next: NULL, 
	hash_ctx_size: sizeof(MD5_CTX),
	hash_block_size:  MD5_BLOCK_SIZE,
	hash_digest_size: MD5_DIGEST_SIZE,
	hash_testvectors: md5_hash_testvectors,
	hmac_testvectors: md5_hmac_testvectors,
	hash_init: (void (*)(void *)) MD5Init,
	hash_update: (void (*)(void *, const u_int8_t *, size_t)) MD5Update,
	hash_final: (void (*)(u_char *, void *)) MD5Final
};

/* SHA-1 test vectors
 * from "The Secure Hash Algorithm Validation System (SHAVS)"
 * July 22, 2004, Lawrence E. Bassham III, NIST
 */

static const u_char sha1_short2_msg[] = {
    0x5e
};

static const u_char sha1_short2_msg_digest[] = {
    0x5e, 0x6f, 0x80, 0xa3, 0x4a, 0x97, 0x98, 0xca,
    0xfc, 0x6a, 0x5d, 0xb9, 0x6c, 0xc5, 0x7b, 0xa4,
    0xc4, 0xdb, 0x59, 0xc2
};

static const u_char sha1_short4_msg[] = {
    0x9a, 0x7d, 0xfd, 0xf1, 0xec, 0xea, 0xd0, 0x6e,
    0xd6, 0x46, 0xaa, 0x55, 0xfe, 0x75, 0x71, 0x46
};

static const u_char sha1_short4_msg_digest[] = {
    0x82, 0xab, 0xff, 0x66, 0x05, 0xdb, 0xe1, 0xc1,
    0x7d, 0xef, 0x12, 0xa3, 0x94, 0xfa, 0x22, 0xa8,
    0x2b, 0x54, 0x4a, 0x35
};

static const u_char sha1_long2_msg[] = {
    0xf7, 0x8f, 0x92, 0x14, 0x1b, 0xcd, 0x17, 0x0a,
    0xe8, 0x9b, 0x4f, 0xba, 0x15, 0xa1, 0xd5, 0x9f,
    0x3f, 0xd8, 0x4d, 0x22, 0x3c, 0x92, 0x51, 0xbd,
    0xac, 0xbb, 0xae, 0x61, 0xd0, 0x5e, 0xd1, 0x15,
    0xa0, 0x6a, 0x7c, 0xe1, 0x17, 0xb7, 0xbe, 0xea,
    0xd2, 0x44, 0x21, 0xde, 0xd9, 0xc3, 0x25, 0x92,
    0xbd, 0x57, 0xed, 0xea, 0xe3, 0x9c, 0x39, 0xfa,
    0x1f, 0xe8, 0x94, 0x6a, 0x84, 0xd0, 0xcf, 0x1f,
    0x7b, 0xee, 0xad, 0x17, 0x13, 0xe2, 0xe0, 0x95,
    0x98, 0x97, 0x34, 0x7f, 0x67, 0xc8, 0x0b, 0x04,
    0x00, 0xc2, 0x09, 0x81, 0x5d, 0x6b, 0x10, 0xa6,
    0x83, 0x83, 0x6f, 0xd5, 0x56, 0x2a, 0x56, 0xca,
    0xb1, 0xa2, 0x8e, 0x81, 0xb6, 0x57, 0x66, 0x54,
    0x63, 0x1c, 0xf1, 0x65, 0x66, 0xb8, 0x6e, 0x3b,
    0x33, 0xa1, 0x08, 0xb0, 0x53, 0x07, 0xc0, 0x0a,
    0xff, 0x14, 0xa7, 0x68, 0xed, 0x73, 0x50, 0x60,
    0x6a, 0x0f, 0x85, 0xe6, 0xa9, 0x1d, 0x39, 0x6f,
    0x5b, 0x5c, 0xbe, 0x57, 0x7f, 0x9b, 0x38, 0x80,
    0x7c, 0x7d, 0x52, 0x3d, 0x6d, 0x79, 0x2f, 0x6e,
    0xbc, 0x24, 0xa4, 0xec, 0xf2, 0xb3, 0xa4, 0x27,
    0xcd, 0xbb, 0xfb
};

static const u_char sha1_long2_msg_digest[] = {
    0xcb, 0x00, 0x82, 0xc8, 0xf1, 0x97, 0xd2, 0x60,
    0x99, 0x1b, 0xa6, 0xa4, 0x60, 0xe7, 0x6e, 0x20,
    0x2b, 0xad, 0x27, 0xb3
};

static const hash_testvector_t sha1_hash_testvectors[] = {
    { sizeof(sha1_short2_msg), sha1_short2_msg, sha1_short2_msg_digest },
    { sizeof(sha1_short4_msg), sha1_short4_msg, sha1_short4_msg_digest },
    { sizeof(sha1_long2_msg),  sha1_long2_msg,  sha1_long2_msg_digest  },
    { 0, NULL, NULL }
};

/* SHA-1 hmac test vectors
 * from RFC 2202 "Test Cases for HMAC-MD5 and HMAC-SHA-1"
 * September 1997, P. Cheng, IBM & R. Glenn, NIST
 */

static const u_char sha1_hmac1_key[] = {
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
    0x0b, 0x0b, 0x0b, 0x0b
};

static const u_char sha1_hmac1[] = {
    0xb6, 0x17, 0x31, 0x86, 0x55, 0x05, 0x72, 0x64,
    0xe2, 0x8b, 0xc0, 0xb6, 0xfb, 0x37, 0x8c, 0x8e,
    0xf1, 0x46, 0xbe, 0x00
};

static const u_char sha1_hmac2[] = {
    0xef, 0xfc, 0xdf, 0x6a, 0xe5, 0xeb, 0x2f, 0xa2,
    0xd2, 0x74, 0x16, 0xd5, 0xf1, 0x84, 0xdf, 0x9c,
    0x25, 0x9a, 0x7c, 0x79
};

static const u_char sha1_hmac3_key[] = {
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    0xaa, 0xaa, 0xaa, 0xaa
};

static const u_char sha1_hmac3[] = {
    0x12, 0x5d, 0x73, 0x42, 0xb9, 0xac, 0x11, 0xcd,
    0x91, 0xa3, 0x9a, 0xf4, 0x8a, 0xa1, 0x7b, 0x4f,
    0x63, 0xf1, 0x75, 0xd3
};

static const u_char sha1_hmac4[] = {
    0x4c, 0x90, 0x07, 0xf4, 0x02, 0x62, 0x50, 0xc6,
    0xbc, 0x84, 0x14, 0xf9, 0xbf, 0x50, 0xc8, 0x6c,
    0x2d, 0x72, 0x35, 0xda
};

static const u_char sha1_hmac6[] = {
    0xaa, 0x4a, 0xe5, 0xe1, 0x52, 0x72, 0xd0, 0x0e,
    0x95, 0x70, 0x56, 0x37, 0xce, 0x8a, 0x3b, 0x55,
    0xed, 0x40, 0x21, 0x12
};

static const u_char sha1_hmac7[] = {
    0xe8, 0xe9, 0x9d, 0x0f, 0x45, 0x23, 0x7d, 0x78,
    0x6d, 0x6b, 0xba, 0xa7, 0x96, 0x5c, 0x78, 0x08,
    0xbb, 0xff, 0x1a, 0x91
};

static const hmac_testvector_t sha1_hmac_testvectors[] = {
    { sizeof(sha1_hmac1_key), sha1_hmac1_key, sizeof(md5_hmac1_msg), md5_hmac1_msg, sha1_hmac1 },
    { sizeof(md5_hmac2_key),  md5_hmac2_key,  sizeof(md5_hmac2_msg), md5_hmac2_msg, sha1_hmac2 },
    { sizeof(sha1_hmac3_key), sha1_hmac3_key, sizeof(md5_hmac3_msg), md5_hmac3_msg, sha1_hmac3 },
    { sizeof(md5_hmac4_key),  md5_hmac4_key,  sizeof(md5_hmac4_msg), md5_hmac4_msg, sha1_hmac4 },
    { sizeof(md5_hmac6_key),  md5_hmac6_key,  sizeof(md5_hmac6_msg), md5_hmac6_msg, sha1_hmac6 },
    { sizeof(md5_hmac6_key),  md5_hmac6_key,  sizeof(md5_hmac7_msg), md5_hmac7_msg, sha1_hmac7 },
    { 0, NULL, 0, NULL, NULL }
};

static struct hash_desc crypto_hasher_sha1 =
{ 	
	algo_type: IKE_ALG_HASH,
	algo_id:   OAKLEY_SHA,
	algo_next: NULL, 
	hash_ctx_size: sizeof(SHA1_CTX),
	hash_block_size:  SHA1_BLOCK_SIZE,
	hash_digest_size: SHA1_DIGEST_SIZE,
	hash_testvectors: sha1_hash_testvectors,
	hmac_testvectors: sha1_hmac_testvectors,
	hash_init: (void (*)(void *)) SHA1Init,
	hash_update: (void (*)(void *, const u_int8_t *, size_t)) SHA1Update,
	hash_final: (void (*)(u_char *, void *)) SHA1Final
};

void
init_crypto(void)
{
    if (mpz_init_set_str(&groupgenerator, MODP_GENERATOR, 10) != 0
    || mpz_init_set_str(&modp768_modulus, MODP768_MODULUS, 16) != 0
    || mpz_init_set_str(&modp1024_modulus, MODP1024_MODULUS, 16) != 0
    || mpz_init_set_str(&modp1536_modulus, MODP1536_MODULUS, 16) != 0
    || mpz_init_set_str(&modp2048_modulus, MODP2048_MODULUS, 16) != 0
    || mpz_init_set_str(&modp3072_modulus, MODP3072_MODULUS, 16) != 0
    || mpz_init_set_str(&modp4096_modulus, MODP4096_MODULUS, 16) != 0
    || mpz_init_set_str(&modp6144_modulus, MODP6144_MODULUS, 16) != 0
    || mpz_init_set_str(&modp8192_modulus, MODP8192_MODULUS, 16) != 0)
	exit_log("mpz_init_set_str() failed in init_crypto()");

    ike_alg_add((struct ike_alg *) &crypto_encryptor_des);
    ike_alg_add((struct ike_alg *) &crypto_encryptor_3des);
    ike_alg_add((struct ike_alg *) &crypto_hasher_sha1);
    ike_alg_add((struct ike_alg *) &crypto_hasher_md5);
    ike_alg_init();
    ike_alg_test();
}

/* Oakley group description
 *
 * See RFC2409 "The Internet key exchange (IKE)" 6.
 */

const struct oakley_group_desc unset_group = {0, NULL, 0};	/* magic signifier */

const struct oakley_group_desc oakley_group[OAKLEY_GROUP_SIZE] = {
#   define BYTES(bits) (((bits) + BITS_PER_BYTE - 1) / BITS_PER_BYTE)

    { OAKLEY_GROUP_MODP768, &modp768_modulus, BYTES(768) },
    { OAKLEY_GROUP_MODP1024, &modp1024_modulus, BYTES(1024) },
    { OAKLEY_GROUP_MODP1536, &modp1536_modulus, BYTES(1536) },
    { OAKLEY_GROUP_MODP2048, &modp2048_modulus, BYTES(2048) },
    { OAKLEY_GROUP_MODP3072, &modp3072_modulus, BYTES(3072) },
    { OAKLEY_GROUP_MODP4096, &modp4096_modulus, BYTES(4096) },
    { OAKLEY_GROUP_MODP6144, &modp6144_modulus, BYTES(6144) },
    { OAKLEY_GROUP_MODP8192, &modp8192_modulus, BYTES(8192) },
#   undef BYTES
};

const struct oakley_group_desc *
lookup_group(u_int16_t group)
{
    int i;

    for (i = 0; i != elemsof(oakley_group); i++)
	if (group == oakley_group[i].group)
	    return &oakley_group[i];
    return NULL;
}

/* Encryption Routines
 *
 * Each uses and updates the state object's st_new_iv.
 * This must already be initialized.
 */

/* encrypt or decrypt part of an IKE message using DES
 * See RFC 2409 "IKE" Appendix B
 */
#if 0
static void __attribute__ ((unused))
do_des(bool enc, void *buf, size_t buf_len, struct state *st)
{
    des_key_schedule ks;

    (void) des_set_key((des_cblock *)st->st_enc_key.ptr, ks);

    passert(st->st_new_iv_len >= DES_CBC_BLOCK_SIZE);
    st->st_new_iv_len = DES_CBC_BLOCK_SIZE;	/* truncate */

    des_ncbc_encrypt((des_cblock *)buf, (des_cblock *)buf, buf_len,
	ks,
	(des_cblock *)st->st_new_iv, enc);
}
#endif
static void
do_des(u_int8_t *buf, size_t buf_len, u_int8_t *key, size_t key_size, u_int8_t *iv, bool enc)
{
    des_key_schedule ks;
//struct timeval start_time, end_time;
unsigned long diff_time;
unsigned char buf2[1514];
unsigned char iv2[16];

    passert (!key_size || (key_size==(DES_CBC_BLOCK_SIZE)))
    (void) des_set_key((des_cblock *)key, ks);

    //passert(st->st_new_iv_len >= DES_CBC_BLOCK_SIZE);
    //st->st_new_iv_len = DES_CBC_BLOCK_SIZE;	/* truncate */

#if 0
crypto_display_hex (key , key_size,  "Key:");
crypto_display_hex (ks , key_size,  "Key Schedule:");
crypto_display_hex (iv , key_size,  "IV 1:");

crypto_ipsec( buf, buf_len , key , key , key , DES_CBC_BLOCK_SIZE ,tmp_iv , enc , OAKLEY_DES_CBC);
#endif

//	memcpy (iv2, iv, key_size);
//	gettimeofday(&start_time, NULL);

//   uint64_t s_time=cvmx_get_cycle();
//   des_ncbc_encrypt((des_cblock *)buf, (des_cblock *)buf, buf_len, ks, (des_cblock *)iv, enc);
//   uint64_t e_time=cvmx_get_cycle();
//loglog(RC_LOG_SERIOUS,"Software Cycle = %ld us", e_time-s_time);
//	gettimeofday(&end_time, NULL);

//    diff_time=(end_time.tv_sec  - start_time.tv_sec) * 1000000 + (end_time.tv_usec - start_time.tv_usec);
//	loglog(RC_LOG_SERIOUS,"Software TimeDiff = %ld us", diff_time);

#if 0
crypto_display_hex (tmp_iv , key_size,  "IV SW:"); 
DBG_cond_dump(DBG_CRYPT, "SW output:\n", buf2, buf_len);
#endif
	//gettimeofday(&start_time, NULL);

    //uint64_t s_time=cvmx_get_cycle();
   des_ncbc_hwencrypt((des_cblock *)buf, (des_cblock *)buf, buf_len, key, (des_cblock *)iv, enc);
    //uint64_t e_time=cvmx_get_cycle();

//loglog(RC_LOG_SERIOUS,"Hardware Cycle = %ld us", e_time-s_time);
//	gettimeofday(&end_time, NULL);

  //  diff_time=(end_time.tv_sec  - start_time.tv_sec) * 1000000 + (end_time.tv_usec - start_time.tv_usec);
	//loglog(RC_LOG_SERIOUS,"Hardware TimeDiff = %ld us", diff_time);

#if 0
DBG_cond_dump(DBG_CRYPT, "HW output:\n", buf, buf_len);
crypto_display_hex (iv , key_size,  "IV HW:");
crypto_display_hex (key , key_size,  "HW Key:");
#endif
}

/* encrypt or decrypt part of an IKE message using 3DES
 * See RFC 2409 "IKE" Appendix B
 */
static void
do_3des(u_int8_t *buf, size_t buf_len, u_int8_t *key, size_t key_size, u_int8_t *iv, bool enc)
{
    des_key_schedule ks[3];

    passert (!key_size || (key_size==(DES_CBC_BLOCK_SIZE * 3)))
    (void) des_set_key((des_cblock *)key + 0, ks[0]);
    (void) des_set_key((des_cblock *)key + 1, ks[1]);
    (void) des_set_key((des_cblock *)key + 2, ks[2]);

#if 0
crypto_display_hex ((des_cblock *)key + 0 , DES_CBC_BLOCK_SIZE,  "Key1 :");
crypto_display_hex ((des_cblock *)key + 1 , DES_CBC_BLOCK_SIZE,  "Key2 :");
crypto_display_hex ((des_cblock *)key + 2 , DES_CBC_BLOCK_SIZE,  "Key3 :");
crypto_display_hex (iv , DES_CBC_BLOCK_SIZE,  "User IV:");

	crypto_ipsec( buf, buf_len ,(des_cblock *)key + 0 ,(des_cblock *)key + 1 , (des_cblock *)key + 2 , DES_CBC_BLOCK_SIZE ,iv , enc , OAKLEY_3DES_CBC);
sleep(1);
#endif
#if 0
    des_ede3_cbc_encrypt((des_cblock *)buf, (des_cblock *)buf, buf_len,
	ks[0], ks[1], ks[2],
	(des_cblock *)iv, enc);
#endif

    des_ede3_cbc_hwencrypt((des_cblock *)buf, (des_cblock *)buf, buf_len,
	(des_cblock *)key + 0, (des_cblock *)key + 1, (des_cblock *)key + 2,
	(des_cblock *)iv, enc);

}

/* hash and prf routines */
void
crypto_cbc_encrypt(const struct encrypt_desc *e, bool enc, u_int8_t *buf, size_t size, struct state *st)
{
    passert(st->st_new_iv_len >= e->enc_blocksize);
    st->st_new_iv_len = e->enc_blocksize;	/* truncate */

    e->do_crypt(buf, size, st->st_enc_key.ptr, st->st_enc_key.len, st->st_new_iv, enc);
    /*
    e->set_key(&ctx, st->st_enc_key.ptr, st->st_enc_key.len);
    e->cbc_crypt(&ctx, buf, size, st->st_new_iv, enc);
    */
}

/* HMAC package
 * rfc2104.txt specifies how HMAC works.
 */

void
hmac_init(struct hmac_ctx *ctx,
    const struct hash_desc *h,
    const u_char *key, size_t key_len)
{
    int k;

    ctx->h = h;
    ctx->hmac_digest_size = h->hash_digest_size;

    /* Prepare the two pads for the HMAC */

    memset(ctx->buf1, '\0', h->hash_block_size);

    if (key_len <= h->hash_block_size)
    {
	memcpy(ctx->buf1, key, key_len);
    }
    else
    {
	h->hash_init(&ctx->hash_ctx);
	h->hash_update(&ctx->hash_ctx, key, key_len);
	h->hash_final(ctx->buf1, &ctx->hash_ctx);
    }

    memcpy(ctx->buf2, ctx->buf1, h->hash_block_size);

    for (k = 0; k < h->hash_block_size; k++)
    {
	ctx->buf1[k] ^= HMAC_IPAD;
	ctx->buf2[k] ^= HMAC_OPAD;
    }

    hmac_reinit(ctx);
}

void
hmac_reinit(struct hmac_ctx *ctx)
{
    ctx->h->hash_init(&ctx->hash_ctx);
    ctx->h->hash_update(&ctx->hash_ctx, ctx->buf1, ctx->h->hash_block_size);
}

void
hmac_update(struct hmac_ctx *ctx,
    const u_char *data, size_t data_len)
{
    ctx->h->hash_update(&ctx->hash_ctx, data, data_len);
}

void
hmac_final(u_char *output, struct hmac_ctx *ctx)
{
    const struct hash_desc *h = ctx->h;

    h->hash_final(output, &ctx->hash_ctx);

    h->hash_init(&ctx->hash_ctx);
    h->hash_update(&ctx->hash_ctx, ctx->buf2, h->hash_block_size);
    h->hash_update(&ctx->hash_ctx, output, h->hash_digest_size);
    h->hash_final(output, &ctx->hash_ctx);
}
