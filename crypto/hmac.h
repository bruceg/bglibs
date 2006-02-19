#ifndef CRYPTO__HMAC__H__
#define CRYPTO__HMAC__H__

typedef void (*hmac_init_fn)(void*);
typedef void (*hmac_update_fn)(void*, const unsigned char*, unsigned long);
typedef void (*hmac_finalize_fn)(void*, unsigned char*);
typedef void (*hmac_extract_fn)(const void*, void*);
typedef void (*hmac_inject_fn)(void*, const void*);

struct hmac_control_block
{
  unsigned state_size;
  unsigned block_size;
  unsigned digest_size;
  unsigned midstate_size;
  hmac_init_fn init;
  hmac_update_fn update;
  hmac_finalize_fn finalize;
  hmac_extract_fn extract;
  hmac_inject_fn inject;
};

extern const struct hmac_control_block hmac_md5;
extern const struct hmac_control_block hmac_sha1;
extern const struct hmac_control_block hmac_sha256;
extern const struct hmac_control_block hmac_sha384;
extern const struct hmac_control_block hmac_sha512;

struct str;
extern void hmac_prepare(const struct hmac_control_block* hcb,
			 void* midstate,
			 const struct str* secret);
extern void hmac_finish(const struct hmac_control_block* hcb,
			void* midstate,
			const struct str* nonce,
			void* outout);
extern void hmac(const struct hmac_control_block* hcb,
		 const struct str* secret,
		 const struct str* nonce,
		 void* output);

#endif
