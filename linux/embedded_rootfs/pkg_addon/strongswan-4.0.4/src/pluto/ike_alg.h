/* IKE modular algorithm handling interface
 * Author: JuanJo Ciarlante <jjo-ipsec@mendoza.gov.ar>
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
 * RCSID $Id: ike_alg.h 12119 2013-07-09 13:59:35Z dio.li $
 */
 
#ifndef _IKE_ALG_H
#define _IKE_ALG_H

#include "connections.h"

struct ike_alg {
    u_int16_t algo_type;
    u_int16_t algo_id;
    struct ike_alg *algo_next;
};

struct encrypt_desc {
    u_int16_t algo_type;
    u_int16_t algo_id;
    struct ike_alg *algo_next;

    size_t enc_ctxsize;
    size_t enc_blocksize;
    u_int keydeflen;
    u_int keymaxlen;
    u_int keyminlen;
    void (*do_crypt)(u_int8_t *dat, size_t datasize, u_int8_t *key, size_t key_size, u_int8_t *iv, bool enc);
};

typedef struct hash_testvector hash_testvector_t;

struct hash_testvector {
    const size_t  msg_size;
    const u_char *msg;
    const u_char *msg_digest;
};

typedef struct hmac_testvector hmac_testvector_t;

struct hmac_testvector {
    const size_t  key_size;
    const u_char *key;
    const size_t  msg_size;
    const u_char *msg;
    const u_char *hmac;
};
struct hash_desc {
    u_int16_t algo_type;
    u_int16_t algo_id;
    struct ike_alg *algo_next;

    size_t hash_ctx_size;
    size_t hash_block_size;
    size_t hash_digest_size;
    const hash_testvector_t *hash_testvectors;
    const hmac_testvector_t *hmac_testvectors;
    void (*hash_init)(void *ctx);
    void (*hash_update)(void *ctx, const u_int8_t *in, size_t datasize);
    void (*hash_final)(u_int8_t *out, void *ctx);
};

#define IKE_ALG_ENCRYPT		0
#define IKE_ALG_HASH		1
#define IKE_ALG_MAX		IKE_ALG_HASH

extern int ike_alg_add(struct ike_alg *a);
extern struct hash_desc *ike_alg_get_hasher(u_int alg);
extern struct encrypt_desc *ike_alg_get_encrypter(u_int alg);
extern bool ike_alg_enc_present(u_int ealg);
extern bool ike_alg_hash_present(u_int halg);
extern int ike_alg_register_hash(struct hash_desc *a);
extern int ike_alg_register_enc(struct encrypt_desc *e);
extern const struct oakley_group_desc* ike_alg_pfsgroup(struct connection *c
    , lset_t policy);
extern struct db_context * ike_alg_db_new(struct alg_info_ike *ai, lset_t policy);
extern void ike_alg_list(void);
extern void ike_alg_show_connection(struct connection *c, const char *instance);
extern bool ike_alg_test(void);
extern bool ike_alg_ok_final(u_int ealg, u_int key_len, u_int aalg, u_int group
    , struct alg_info_ike *alg_info_ike);
extern int ike_alg_init(void);

#endif /* _IKE_ALG_H */
