/* sha512.c - SHA-512 algorithm
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
#include "sha512.h"
#include "uint64.h"

static const uint64 K[] = {
  0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL,
  0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL,
  0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL,
  0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL,
  0xd807aa98a3030242ULL, 0x12835b0145706fbeULL,
  0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
  0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL,
  0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL,
  0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL,
  0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
  0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL,
  0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
  0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL,
  0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL,
  0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL,
  0x06ca6351e003826fULL, 0x142929670a0e6e70ULL,
  0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL,
  0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
  0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL,
  0x81c2c92e47edaee6ULL, 0x92722c851482353bULL,
  0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL,
  0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL,
  0xd192e819d6ef5218ULL, 0xd69906245565a910ULL,
  0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
  0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL,
  0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL,
  0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL,
  0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL,
  0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL,
  0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
  0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL,
  0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL,
  0xca273eceea26619cULL, 0xd186b8c721c0c207ULL,
  0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL,
  0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL,
  0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
  0x28db77f523047d84ULL, 0x32caab7b40c72493ULL,
  0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL,
  0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL,
  0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817
};

static const uint64 H0[8] = {
  0x6a09e667f3bcc908ULL,
  0xbb67ae8584caa73bULL,
  0x3c6ef372fe94f82bULL,
  0xa54ff53a5f1d36f1ULL,
  0x510e527fade682d1ULL,
  0x9b05688c2b3e6c1fULL,
  0x1f83d9abfb41bd6bULL,
  0x5be0cd19137e2179ULL
};

#define S(X,N) ((X>>N)|(X<<(64-N)))
#define R(X,N) (X>>N)
/* This optimization was found in Colin Plumb's MD5 code. */
/* #define Ch(x,y,z) (((x)&(y)) ^ (~(x)&(z)))  */
#define Ch(x,y,z) ((z) ^ ((x) & ((y) ^ (z))))
#define Maj(x,y,z) (((x)&(y)) | ((x)&(z)) | ((y)&(z)))
#define S0(x) (S(x,28) ^ S(x,34) ^ S(x,39))
#define S1(x) (S(x,14) ^ S(x,18) ^ S(x,41))
#define s0(x) (S(x, 1) ^ S(x, 8) ^ R(x, 7))
#define s1(x) (S(x,19) ^ S(x,61) ^ R(x, 6))
#define F(a,b,c,d,e,f,g,h) \
    T1 = h + S1(e) + Ch(e,f,g) + *Kp++ + *Wp++; \
    T2 = S0(a) + Maj(a,b,c); \
    d += T1; \
    h = T1 + T2; \

static void SHA512_transform(uint64 H[8], const uint8 M[128])
{
  uint64 a = H[0];
  uint64 b = H[1];
  uint64 c = H[2];
  uint64 d = H[3];
  uint64 e = H[4];
  uint64 f = H[5];
  uint64 g = H[6];
  uint64 h = H[7];
  uint64 W[80];
  uint64* Wp;
  const uint64* Kp;
  uint64 T1;
  uint64 T2;
  unsigned j;

  for (Wp = W, j = 0; j < 16; ++j, ++Wp, M += 8)
    *Wp = uint64_get_msb(M);
  
  for (j = 16; j < 80; ++j, ++Wp)
    *Wp = s1(Wp[-2]) + Wp[-7] + s0(Wp[-15]) + Wp[-16];

#ifdef SHA2_NO_UNROLL
  for (Wp = W, Kp = K, j = 0; j < 80; ++j) {
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
  for (Wp = W, Kp = K, j = 0; j < 10; ++j) {
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

void SHA512_init(SHA512_ctx* ctx)
{
  memcpy(ctx->H, H0, sizeof H0);
  ctx->bytes = 0;
}

void SHA512_update(SHA512_ctx* ctx, const void* vdata, unsigned long data_len)
{
  const uint8* data = vdata;
  unsigned use;
  unsigned mlen = ctx->bytes % 128;
   
  ctx->bytes += data_len;
  if (mlen > 0 && data_len >= (use = 128 - mlen)) {
    memcpy(ctx->M + mlen, data, use);
    SHA512_transform(ctx->H, ctx->M);
    mlen = 0;
    data_len -= use;
    data += use;
  }

  while (data_len >= 128) {
    SHA512_transform(ctx->H, data);
    data_len -= 128;
    data += 128;
  }
  
  memcpy(ctx->M + mlen, data, data_len);
}

void SHA512_final_transform(SHA512_ctx* ctx)
{
  unsigned mlen = ctx->bytes % 128;
  ctx->M[mlen++] = 0x80;
  memset(ctx->M + mlen, 0x00, 128 - mlen);
  if (mlen > 128-16) {
    SHA512_transform(ctx->H, ctx->M);
    memset(ctx->M, 0x00, 128-16);
  }

  uint64_pack_msb(ctx->bytes >> 61, ctx->M+112);
  uint64_pack_msb(ctx->bytes <<  3, ctx->M+120);
  SHA512_transform(ctx->H, ctx->M);
}

void SHA512_final(SHA512_ctx* ctx, uint8* digest)
{
  unsigned i;
  SHA512_final_transform(ctx);
  for (i = 0; i < 8; ++i, digest += 8)
    uint64_pack_msb(ctx->H[i], digest);
  memset(ctx, 0, sizeof *ctx);
}

#ifdef SELFTEST_MAIN
#include <stdio.h>

static void test(const char* str)
{
   SHA512_ctx ctx;
   unsigned i;
   unsigned char digest[SHA512_DIGEST_LENGTH];
   SHA512_init(&ctx);
   SHA512_update(&ctx, str, strlen(str));
   SHA512_final(&ctx, digest);
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
ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f
8e959b75dae313da8cf4f72814fc143f8f7779c6eb9f7fa17299aeadb6889018501d289e4900f7e4331b99dec4b5433ac7d329eeb6dd26545e96e55b874be909
204a8fc6dda82f0a0ced7beb8e08a41657c16ef468b228a8279be331a703c33596fd15c13b1b07f9aa1d3bea57789ca031ad85c7a71dd70354ec631238ca3445
cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e
70aefeaa0e7ac4f8fe17532d7185a289bee3b428d950c14fa8b713ca09814a387d245870e007a80ad97c369d193e41701aa07f3221d15f0e65a1ff970cedf030
b3de4afbc516d2478fe9b518d063bda6c8dd65fc38402dd81d1eb7364e72fb6e6663cf6d2771c8f5a6da09601712fb3d2a36c6ffea3e28b0818b05b0a8660766
97fb4ec472f3cb698b9c3c12a12768483e5b62bcdad934280750b4fa4701e5e0550a80bb0828342c19631ba55a55e1cee5de2fda91fc5d40e7bee1d4e6d415b3
d399507bbf5f2d0da51db1ff1fc51c1c9ff1de0937e00d01693b240e84fcc3400601429f45c297acc6e8fcf1e4e4abe9ff21a54a0d3d88888f298971bd206cd5
#endif
