
#include <string.h>
#include "sysdeps.h"
#include "hmac.h"
#include "sha512.h"

struct midstate
{
  uint64 H[8];
};

static void extract(const void* statep, void* midstatep)
{
  const SHA512_ctx* ctx = statep;
  struct midstate* mid = midstatep;
  memcpy(mid->H, ctx->H, sizeof *mid);
}

static void inject(void* statep, const void* midstatep)
{
  SHA512_ctx* ctx = statep;
  const struct midstate* mid = midstatep;
  memcpy(ctx->H, mid->H, sizeof *mid);
  ctx->bytes = 128;
}

const struct hmac_control_block hmac_sha512 = {
  .state_size = sizeof(SHA512_ctx),
  .block_size = 128,
  .digest_size = SHA512_DIGEST_LENGTH,
  .midstate_size = sizeof(struct midstate),
  .init = (hmac_init_fn)SHA512_init,
  .update = (hmac_update_fn)SHA512_update,
  .finalize = (hmac_finalize_fn)SHA512_final,
  .extract = extract,
  .inject = inject,
};

#ifdef SELFTEST_MAIN
#include "iobuf/obuf.h"
#include "str/str.h"

static void test_hmac(const char* key, const char* data)
{
  const str key_str = { (char*)key, strlen(key), 0 };
  const str data_str = { (char*)data, strlen(data), 0 };
  unsigned char digest[SHA512_DIGEST_LENGTH];
  unsigned i;
  hmac(&hmac_sha512, &key_str, &data_str, digest);
  for (i = 0; i < sizeof digest; ++i)
    obuf_putxw(&outbuf, digest[i], 2, '0');
  obuf_endl(&outbuf);
}

int main(void)
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

  return 0;
}
#endif
#ifdef SELFTEST_EXP
7641c48a3b4aa8f887c07b3e83f96affb89c978fed8c96fcbbf4ad596eebfe496f9f16da6cd080ba393c6f365ad72b50d15c71bfb1d6b81f66a911786c6ce932
164b7a7bfcf819e2e395fbe73b56e0a387bd64222e831fd610270cd7ea2505549758bf75c05a994a6d034f65f8f0e6fdcaeab1a34d4a6b4b636e070a38bce737
ad9b5c7de72693737cd5e9d9f41170d18841fec1201c1c1b02e05cae116718009f771cad9946ddbf7e3cde3e818d9ae85d91b2badae94172d096a44a79c91e86
#endif
