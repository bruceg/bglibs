/* sha256.c - SHA-256 algorithm
 * Copyright (C) 2003,2005  Bruce Guenter <bruce@untroubled.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#include <string.h>
#include "sha256.h"
#include "uint32.h"
#include "uint64.h"

#define S(x,n) (((x)>>n) | ((x)<<(32-n)))
#define R(x,n) ((x)>>n)
/* This optimization was found in Colin Plumb's MD5 code. */
/* #define Ch(x,y,z) (((x)&(y)) ^ (~(x)&(z)))  */
#define Ch(x,y,z) ((z) ^ ((x) & ((y) ^ (z))))
#define Maj(x,y,z) (((x)&(y)) ^ ((x)&(z)) ^ ((y)&(z)))
#define S0(x) (S(x, 2) ^ S(x,13) ^ S(x,22))
#define S1(x) (S(x, 6) ^ S(x,11) ^ S(x,25))
#define s0(x) (S(x, 7) ^ S(x,18) ^ R(x, 3))
#define s1(x) (S(x,17) ^ S(x,19) ^ R(x,10))
#define F(a,b,c,d,e,f,g,h) \
    T1 = h + S1(e) + Ch(e,f,g) + *Kp++ + *Wp++; \
    T2 = S0(a) + Maj(a,b,c); \
    d += T1; \
    h = T1 + T2; \

static const uint32 K[64] = {
  0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL,
  0x3956c25bUL, 0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL,
  0xd807aa98UL, 0x12835b01UL, 0x243185beUL, 0x550c7dc3UL,
  0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL, 0xc19bf174UL,
  0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
  0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL,
  0x983e5152UL, 0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL,
  0xc6e00bf3UL, 0xd5a79147UL, 0x06ca6351UL, 0x14292967UL,
  0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL, 0x53380d13UL,
  0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
  0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL,
  0xd192e819UL, 0xd6990624UL, 0xf40e3585UL, 0x106aa070UL,
  0x19a4c116UL, 0x1e376c08UL, 0x2748774cUL, 0x34b0bcb5UL,
  0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL, 0x682e6ff3UL,
  0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
  0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};

static const uint32 H0[8] = {
  0x6a09e667UL,
  0xbb67ae85UL,
  0x3c6ef372UL,
  0xa54ff53aUL,
  0x510e527fUL,
  0x9b05688cUL,
  0x1f83d9abUL,
  0x5be0cd19UL
};

static void SHA256_transform(uint32* H, const uint8* M)
{
  unsigned j;
  uint32 a = H[0];
  uint32 b = H[1];
  uint32 c = H[2];
  uint32 d = H[3];
  uint32 e = H[4];
  uint32 f = H[5];
  uint32 g = H[6];
  uint32 h = H[7];
  uint32 T1;
  uint32 T2;
  uint32 W[64];
  uint32* Wp;
  const uint32* Kp;

  for (Wp = W, j = 0; j < 16; ++j, ++Wp, M += 4)
    *Wp = uint32_get_msb(M);
  
  for (j = 16; j < 64; ++j, ++Wp)
    *Wp = s1(Wp[-2]) + Wp[-7] + s0(Wp[-15]) + Wp[-16];

#ifdef SHA2_NO_UNROLL
  for (Wp = W, Kp = K, j = 0; j < 64; ++j) {
    /* Straight out of the standards document. */
    T1 = h + S1(e) + Ch(e,f,g) + *Kp++ + *Wp++;
    T2 = S0(a) + Maj(a,b,c);
    h = g;
    g = f;
    f = e;
    e = d + T1;
    d = c;
    c = b;
    b = a;
    a = T1 + T2;
  }
#else
  for (Wp = W, Kp = K, j = 0; j < 8; ++j) {
    /* Unrolled loop, eliminates most of the extraneous data copies. */
    F(a,b,c,d,e,f,g,h);
    F(h,a,b,c,d,e,f,g);
    F(g,h,a,b,c,d,e,f);
    F(f,g,h,a,b,c,d,e);
    F(e,f,g,h,a,b,c,d);
    F(d,e,f,g,h,a,b,c);
    F(c,d,e,f,g,h,a,b);
    F(b,c,d,e,f,g,h,a);
  }
#endif
  
  H[0] += a;
  H[1] += b;
  H[2] += c;
  H[3] += d;
  H[4] += e;
  H[5] += f;
  H[6] += g;
  H[7] += h;
}

void SHA256_init(SHA256_ctx* ctx)
{
  memcpy(ctx->H, H0, sizeof H0);
  ctx->bytes = 0;
}

void SHA256_update(SHA256_ctx* ctx, const void* vdata, unsigned long data_len)
{
  const uint8* data = vdata;
  unsigned long use;
  unsigned mlen = ctx->bytes % 64;

  ctx->bytes += data_len;

  if (mlen && data_len >= (use = 64 - mlen)) {
    memcpy(ctx->M + mlen, data, use);
    SHA256_transform(ctx->H, ctx->M);
    mlen = 0;
    data_len -= use;
    data += use;
  }

  while (data_len >= 64) {
    SHA256_transform(ctx->H, data);
    data_len -= 64;
    data += 64;
  }

  memcpy(ctx->M + mlen, data, data_len);
}

void SHA256_final(SHA256_ctx* ctx, uint8* digest)
{
  unsigned i;
  unsigned mlen = ctx->bytes % 64;
  ctx->M[mlen++] = 0x80;
  memset(ctx->M + mlen, 0x00, 64 - mlen);
  if (mlen > 64-8) {
    SHA256_transform(ctx->H, ctx->M);
    memset(ctx->M, 0x00, 64-8);
  }

  uint64_pack_msb(ctx->bytes << 3, ctx->M+64-8);
  SHA256_transform(ctx->H, ctx->M);

  for (i = 0; i < 32/4; ++i, digest += 4)
    uint32_pack_msb(ctx->H[i], digest);
  memset(ctx, 0, sizeof *ctx);
}

#ifdef SELFTEST_MAIN
#include "obuf.h"

static void test(const char* s)
{
  SHA256_ctx ctx;
  unsigned i;
  unsigned char digest[SHA256_DIGEST_LENGTH];
  SHA256_init(&ctx);
  SHA256_update(&ctx, s, strlen(s));
  SHA256_final(&ctx, digest);
  for (i = 0; i < sizeof digest; ++i)
    obuf_putxw(&outbuf, digest[i], 2, '0');
  obuf_endl(&outbuf);
}

MAIN
{
  test("abc");
  test("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
  /* Additional test vectors found in
   * http://www.aarongifford.com/computers/sha.html */
  test("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu");
  test("");
  test("This is exactly 64 bytes long, not counting the terminating byte");
  test("For this sample, this 63-byte string will be used as input data");
  test("And this textual data, astonishing as it may appear, is exactly 128 bytes in length, as are both SHA-384 and SHA-512 block sizes");
  test("By hashing data that is one byte less than a multiple of a hash block length (like this 127-byte string), bugs may be revealed.");
}
#endif
#ifdef SELFTEST_EXP
ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad
248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1
cf5b16a778af8380036ce59e7b0492370b249b11e8f07a51afac45037afee9d1
e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855
ab64eff7e88e2e46165e29f2bce41826bd4c7b3552f6b382a9e7d3af47c245f8
f08a78cbbaee082b052ae0708f32fa1e50c5c421aa772ba5dbb406a2ea6be342
0ab803344830f92089494fb635ad00d76164ad6e57012b237722df0d7ad26896
e4326d0459653d7d3514674d713e74dc3df11ed4d30b4013fd327fdb9e394c26
#endif
