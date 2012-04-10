/* $Id: md5.c 646 2005-12-01 22:07:11Z bruce $ */

#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#include "sysdeps.h"
#include "hmac.h"
#include "md5.h"

struct midstate
{
  uint32 A, B, C, D;
};

static void extract(const void* statep, void* midstatep)
{
  const struct md5_ctx* ctx = statep;
  struct midstate* mid = midstatep;
  mid->A = ctx->A;
  mid->B = ctx->B;
  mid->C = ctx->C;
  mid->D = ctx->D;
}

static void inject(void* statep, const void* midstatep)
{
  struct md5_ctx* ctx = statep;
  const struct midstate* mid = midstatep;
  ctx->A = mid->A;
  ctx->B = mid->B;
  ctx->C = mid->C;
  ctx->D = mid->D;
  ctx->total[0] = 64;
  ctx->total[1] = 0;
  ctx->buflen = 0;
}

static void update(void* ctx,
		   const unsigned char* bytes,
		   unsigned long len)
{
  md5_process_bytes(bytes, len, ctx);
}

const struct hmac_control_block hmac_md5 = {
  .state_size = sizeof(struct md5_ctx),
  .block_size = 64,
  .digest_size = 128/8,
  .midstate_size = sizeof(struct midstate),
  .init = (hmac_init_fn)md5_init_ctx,
  .update = update,
  .finalize = (hmac_finalize_fn)md5_finish_ctx,
  .extract = extract,
  .inject = inject,
};

#ifdef SELFTEST_MAIN
#include <stdio.h>
#include "bglibs/str.h"

static void HMACTest(const char* key, const char* data)
{
  const str key_str = { (char*)key, strlen(key), 0 };
  const str data_str = { (char*)data, strlen(data), 0 };
  unsigned char digest[128/8];
  unsigned i;
  hmac(&hmac_md5, &key_str, &data_str, digest);
  printf("HMAC (%d,%d) = ", key_str.len, data_str.len);
  for (i = 0; i < sizeof digest; ++i) printf("%02x", digest[i]);
  printf("\n");
}

MAIN
{
  /* Test vectors from RFC 2104 */
  HMACTest("\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b",
	   "Hi There");
  HMACTest("Jefe", "what do ya want for nothing?");
  HMACTest("\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa",
	   "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
	   "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
	   "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
	   "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
	   "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd");
}
#endif
#ifdef SELFTEST_EXP
HMAC (16,8) = 9294727a3638bb1c13f48ef8158bfc9d
HMAC (4,28) = 750c783e6ab0b503eaa86e310a5db738
HMAC (16,50) = 56be34521d144c88dbb8c733f0e8b3f6
#endif
