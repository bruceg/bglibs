
#include <string.h>
#include "sysdeps.h"
#include "hmac.h"
#include "sha384.h"

struct midstate
{
  uint64 H[8];
};

static void extract(const void* statep, void* midstatep)
{
  const SHA384_ctx* ctx = statep;
  struct midstate* mid = midstatep;
  memcpy(mid->H, ctx->H, 8 * sizeof(mid->H[0]));
}

static void inject(void* statep, const void* midstatep)
{
  SHA384_ctx* ctx = statep;
  const struct midstate* mid = midstatep;
  memcpy(ctx->H, mid->H, 8 * sizeof(mid->H[0]));
  ctx->bytes = 128;
}

const struct hmac_control_block hmac_sha384 = {
  .state_size = sizeof(SHA384_ctx),
  .block_size = 128,
  .digest_size = SHA384_DIGEST_LENGTH,
  .midstate_size = sizeof(struct midstate),
  .init = (hmac_init_fn)SHA384_init,
  .update = (hmac_update_fn)SHA384_update,
  .finalize = (hmac_finalize_fn)SHA384_final,
  .extract = extract,
  .inject = inject,
};

#ifdef SELFTEST_MAIN
#include "obuf.h"
#include "str/str.h"

static void test_hmac(const char* key, const char* data)
{
  const str key_str = { (char*)key, strlen(key), 0 };
  const str data_str = { (char*)data, strlen(data), 0 };
  unsigned char digest[SHA384_DIGEST_LENGTH];
  unsigned i;
  hmac(&hmac_sha384, &key_str, &data_str, digest);
  for (i = 0; i < sizeof digest; ++i)
    obuf_putxw(&outbuf, digest[i], 2, '0');
  obuf_endl(&outbuf);
}

MAIN
{
  test_hmac("\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b",
	    "Hi There");
  test_hmac("Jefe", "what do ya want for nothing?");
  test_hmac("\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa",
	    "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
	    "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
	    "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
	    "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
	    "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd");
}
#endif
#ifdef SELFTEST_EXP
7afaa633e20d379b02395915fbc385ff8dc27dcd3885e1068ab942eeab52ec1f20ad382a92370d8b2e0ac8b83c4d53bf
af45d2e376484031617f78d2b58a6b1b9c7ef464f5a01b47e42ec3736322445e8e2240ca5e69e2c78b3239ecfab21649
1383e82e28286b91f4cc7afbd13d5b5c6f887c05e7c4542484043a37a5fe45802a9470fb663bd7b6570fe2f503fc92f5
#endif
