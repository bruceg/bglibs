/* sha384.c - SHA-384 algorithm
 * Copyright (C) 2003  Bruce Guenter <bruceg@em.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * This code was derived from the official algorithm described in
 * http://csrc.nist.gov/cryptval/shs/sha256-384-512.pdf
 */
#include <string.h>
#include "sysdeps.h"
#include "sha384.h"
#include "uint64.h"

#define min(X,Y) ((X)<(Y) ? (X) : (Y))

static const uint64 H[8] = {
  0xcbbb9d5dc1059ed8ULL,
  0x629a292a367cd507ULL,
  0x9159015a3070dd17ULL,
  0x152fecd8f70e5939ULL,
  0x67332667ffc00b31ULL,
  0x8eb44a8768581511ULL,
  0xdb0c2e0d64f98fa7ULL,
  0x47b5481dbefa4fa4ULL
};

void SHA384_init(SHA384_ctx* ctx)
{
  memcpy(ctx->H, H, sizeof H);
  ctx->lbits = 0;
  ctx->hbits = 0;
  ctx->mlen = 0;
}

void SHA384_final(SHA384_ctx* ctx, uint8* digest)
{
  uint8 tmp[SHA512_DIGEST_LENGTH];
  SHA512_final(ctx, tmp);
  memcpy(digest, tmp, SHA384_DIGEST_LENGTH);
}

#ifdef SELFTEST_MAIN
#include <stdio.h>

static void test(const char* str)
{
   SHA384_ctx ctx;
   unsigned i;
   unsigned char digest[SHA384_DIGEST_LENGTH];
   SHA384_init(&ctx);
   SHA384_update(&ctx, str, strlen(str));
   SHA384_final(&ctx, digest);
   for (i = 0; i < sizeof(digest); i++)
      printf("%02x", digest[i]);
   printf("\n");
}

int main(void)
{
  test("abc");
  test("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu");
  /* Additional test vectors found in
   * http://www.aarongifford.com/computers/sha.html */
  test("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
  test("");
  test("This is exactly 64 bytes long, not counting the terminating byte");
  test("For this sample, this 63-byte string will be used as input data");
  test("And this textual data, astonishing as it may appear, is exactly 128 bytes in length, as are both SHA-384 and SHA-512 block sizes");
  test("By hashing data that is one byte less than a multiple of a hash block length (like this 127-byte string), bugs may be revealed.");
  return 0;
}
#endif
#ifdef SELFTEST_EXP
cb00753f45a35e8bb5a03d699ac65007272c32ab0eded1631a8b605a43ff5bed8086072ba1e7cc2358baeca134c825a7
09330c33f71147e83d192fc782cd1b4753111b173b3b05d22fa08086e3b0f712fcc7c71a557e2db966c3e9fa91746039
3391fdddfc8dc7393707a65b1b4709397cf8b1d162af05abfe8f450de5f36bc6b0455a8520bc4e6f5fe95b1fe3c8452b
38b060a751ac96384cd9327eb1b1e36a21fdb71114be07434c0cc7bf63f6e1da274edebfe76f65fbd51ad2f14898b95b
e28e35e25a1874908bf0958bb088b69f3d742a753c86993e9f4b1c4c21988f958bd1fe0315b195aca7b061213ac2a9bd
37b49ef3d08de53e9bd018b0630067bd43d09c427d06b05812f48531bce7d2a698ee2d1ed1ffed46fd4c3b9f38a8a557
e3e3602f4d90c935321d788f722071a8809f4f09366f2825cd85da97ccd2955eb6b8245974402aa64789ed45293e94ba
1ca650f38480fa9dfb5729636bec4a935ebc1cd4c0055ee50cad2aa627e066871044fd8e6fdb80edf10b85df15ba7aab
#endif
