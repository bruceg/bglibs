#ifndef CRYPTO__HMAC__H__
#define CRYPTO__HMAC__H__

typedef void (*hmac_init_fn)(void*);
typedef void (*hmac_update_fn)(void*, unsigned char*, unsigned long);
typedef void (*hmac_finalize_fn)(void*, unsigned char*);

struct hmac_control_block
{
  unsigned state_size;
  unsigned final_size;
  unsigned block_size;
  hmac_init_fn init;
  hmac_update_fn update;
  hmac_finalize_fn finalize;
};

extern const struct hmac_control_block hmac_md5;
extern const struct hmac_control_block hmac_sha1;
extern const struct hmac_control_block hmac_sha256;
extern const struct hmac_control_block hmac_sha384;
extern const struct hmac_control_block hmac_sha512;

struct str;
extern void hmac(const struct hmac_control_block* hcb,
		 const struct str* secret,
		 const struct str* nonce,
		 void* output);

#endif
