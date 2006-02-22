#ifndef CRYPTO__HASH__H__
#define CRYPTO__HASH__H__

#include <sysdeps.h>

struct hash_state;

typedef void (*hash_compress_fn)(struct hash_state* state, const unsigned char* block);
typedef void (*hash_extract_fn)(const struct hash_state* state, unsigned char* digest);
typedef void (*hash_packlength_fn)(uint64 bytes, unsigned char* output);

struct hash_control_block
{
  unsigned state_size;
  unsigned block_shift;
  unsigned block_mask;
  unsigned length_size;
  unsigned digest_size;
  const void *init;
  hash_compress_fn compress;
  hash_packlength_fn packlength;
  hash_extract_fn extract;
};

struct hash_context
{
  const struct hash_control_block* hcb;
  uint64 bytes;
  unsigned char data[0] __attribute__((aligned(8)));
};

extern const struct hash_control_block hash_md4;
extern const struct hash_control_block hash_md5;
extern const struct hash_control_block hash_sha1;
extern const struct hash_control_block hash_sha256;
extern const struct hash_control_block hash_sha384;
extern const struct hash_control_block hash_sha512;

extern struct hash_context* hash_alloc(const struct hash_control_block* hcb);
extern void hash_init(struct hash_context* hc);
extern void hash_update(struct hash_context* hc,
			const unsigned char* data, unsigned long len);
extern void hash_finish(struct hash_context* hc, unsigned char* digest);
extern int hash_block(const struct hash_control_block* hcb,
		      const unsigned char* data, unsigned long len,
		      unsigned char* digest);

/* Helper functions used by the individual hashes. */
extern void hash_pack_uint64_lsb(uint64 bytes, unsigned char* output);
extern void hash_pack_uint64_msb(uint64 bytes, unsigned char* output);
extern void hash_pack_uint128_msb(uint64 bytes, unsigned char* output);

#endif
