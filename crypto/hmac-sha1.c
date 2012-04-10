
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#include "sysdeps.h"
#include "hmac.h"
#include "sha1.h"

struct midstate
{
  uint32 state[5];
};

static void extract(const void* statep, void* midstatep)
{
  const SHA1_CTX* ctx = statep;
  struct midstate* mid = midstatep;
  memcpy(mid->state, ctx->state, 5 * sizeof(mid->state[0]));
}

static void inject(void* statep, const void* midstatep)
{
  SHA1_CTX* ctx = statep;
  const struct midstate* mid = midstatep;
  memcpy(ctx->state, mid->state, 5 * sizeof(mid->state[0]));
  ctx->bytes = 64;
}

const struct hmac_control_block hmac_sha1 = {
  .state_size = sizeof(SHA1_CTX),
  .block_size = SHA1_BLOCK_LENGTH,
  .digest_size = SHA1_DIGEST_LENGTH,
  .midstate_size = sizeof(struct midstate),
  .init = (hmac_init_fn)SHA1Init,
  .update = (hmac_update_fn)SHA1Update,
  .finalize = (hmac_finalize_fn)SHA1Final,
  .extract = extract,
  .inject = inject,
};

#ifdef SELFTEST_MAIN
#include "bglibs/str.h"

static void test_hmac(const char* key, const char* data)
{
  const str key_str = { (char*)key, strlen(key), 0 };
  const str data_str = { (char*)data, strlen(data), 0 };
  unsigned char digest[SHA1_DIGEST_LENGTH];
  unsigned i;
  hmac(&hmac_sha1, &key_str, &data_str, digest);
  for (i = 0; i < sizeof digest; ++i)
    obuf_putXw(&outbuf, digest[i], 2, '0');
  NL();
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
675B0B3A1B4DDF4E124872DA6C2F632BFED957E9
EFFCDF6AE5EB2FA2D27416D5F184DF9C259A7C79
D730594D167E35D5956FD8003D0DB3D3F46DC7BB
#endif
