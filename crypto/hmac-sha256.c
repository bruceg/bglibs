
#include <string.h>
#include "hmac.h"
#include "sha256.h"

struct midstate
{
  uint32 H[8];
};

static void extract(const void* statep, void* midstatep)
{
  const SHA256_ctx* ctx = statep;
  struct midstate* mid = midstatep;
  memcpy(mid->H, ctx->H, 8 * sizeof(mid->H[0]));
}

static void inject(void* statep, const void* midstatep)
{
  SHA256_ctx* ctx = statep;
  const struct midstate* mid = midstatep;
  memcpy(ctx->H, mid->H, 8 * sizeof(mid->H[0]));
  ctx->bytes = 64;
}

const struct hmac_control_block hmac_sha256 = {
  .state_size = sizeof(SHA256_ctx),
  .block_size = 64,
  .digest_size = SHA256_DIGEST_LENGTH,
  .midstate_size = sizeof(struct midstate),
  .init = (hmac_init_fn)SHA256_init,
  .update = (hmac_update_fn)SHA256_update,
  .finalize = (hmac_finalize_fn)SHA256_final,
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
  unsigned char digest[SHA256_DIGEST_LENGTH];
  unsigned i;
  hmac(&hmac_sha256, &key_str, &data_str, digest);
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
492ce020fe2534a5789dc3848806c78f4f6711397f08e7e7a12ca5a4483c8aa6
5bdcc146bf60754e6a042426089575c75a003f089d2739839dec58b964ec3843
7dda3cc169743a6484649f94f0eda0f9f2ff496a9733fb796ed5adb40a44c3c1
#endif
