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
#include "uint32.h"

#define min(X,Y) ((X)<(Y) ? (X) : (Y))

#ifdef HAS_ULONG64

static const uint64 K[] = {
  0x428a2f98d728ae22, 0x7137449123ef65cd,
  0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
  0x3956c25bf348b538, 0x59f111f1b605d019,
  0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
  0xd807aa98a3030242, 0x12835b0145706fbe,
  0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
  0x72be5d74f27b896f, 0x80deb1fe3b1696b1,
  0x9bdc06a725c71235, 0xc19bf174cf692694,
  0xe49b69c19ef14ad2, 0xefbe4786384f25e3,
  0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
  0x2de92c6f592b0275, 0x4a7484aa6ea6e483,
  0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
  0x983e5152ee66dfab, 0xa831c66d2db43210,
  0xb00327c898fb213f, 0xbf597fc7beef0ee4,
  0xc6e00bf33da88fc2, 0xd5a79147930aa725,
  0x06ca6351e003826f, 0x142929670a0e6e70,
  0x27b70a8546d22ffc, 0x2e1b21385c26c926,
  0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
  0x650a73548baf63de, 0x766a0abb3c77b2a8,
  0x81c2c92e47edaee6, 0x92722c851482353b,
  0xa2bfe8a14cf10364, 0xa81a664bbc423001,
  0xc24b8b70d0f89791, 0xc76c51a30654be30,
  0xd192e819d6ef5218, 0xd69906245565a910,
  0xf40e35855771202a, 0x106aa07032bbd1b8,
  0x19a4c116b8d2d0c8, 0x1e376c085141ab53,
  0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
  0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb,
  0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
  0x748f82ee5defb2fc, 0x78a5636f43172f60,
  0x84c87814a1f0ab72, 0x8cc702081a6439ec,
  0x90befffa23631e28, 0xa4506cebde82bde9,
  0xbef9a3f7b2c67915, 0xc67178f2e372532b,
  0xca273eceea26619c, 0xd186b8c721c0c207,
  0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
  0x06f067aa72176fba, 0x0a637dc5a2c898a6,
  0x113f9804bef90dae, 0x1b710b35131c471b,
  0x28db77f523047d84, 0x32caab7b40c72493,
  0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
  0x4cc5d4becb3e42b6, 0x597f299cfc657e2a,
  0x5fcb6fab3ad6faec, 0x6c44198c4a475817
};

static const uint64 H[8] = {
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
#define Ch(x,y,z) (((x)&(y)) | (~(x)&(z)))
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

static void SHA512_transform(SHA512_ctx* ctx)
{
  uint64 a = ctx->H[0];
  uint64 b = ctx->H[1];
  uint64 c = ctx->H[2];
  uint64 d = ctx->H[3];
  uint64 e = ctx->H[4];
  uint64 f = ctx->H[5];
  uint64 g = ctx->H[6];
  uint64 h = ctx->H[7];
  uint64 W[80];
  uint64* Wp;
  const uint64* Kp;
  uint64 T1;
  uint64 T2;
  unsigned j;

  for (j = 0; j < 16; ++j)
    W[j] = uint64_get_msb(ctx->M + j*8);

  for (j = 16; j < 80; ++j)
    W[j] = s1(W[j-2]) + W[j-7] + s0(W[j-15]) + W[j-16];

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

  ctx->H[0] += a;
  ctx->H[1] += b;
  ctx->H[2] += c;
  ctx->H[3] += d;
  ctx->H[4] += e;
  ctx->H[5] += f;
  ctx->H[6] += g;
  ctx->H[7] += h;
}

#else

/*
 * Implementation of SHA-512, based on Adam Back's sha-1 implementation.
 * Uses 32 bit words.
 * Based on the implementation that was posted anonymously to
 * coderpunks@toad.com on October 14th, 2000.
 * Believed to be public domain.
 */

/* Rotate right by n bits; separate macros for high and low results */
#define Sh(xh,xl,n) ( (n<32) ? ((xh>>n)|(xl<<(32-n))) : ((xh<<(64-n))|(xl>>(n-32))) )
#define Sl(xh,xl,n) ( (n<32) ? ((xh<<(32-n))|(xl>>n)) : ((xh>>(n-32))|(xl<<(64-n))) )

/* Shift right by n bits; separate macros for high and low results */
#define Rh(xh,xl,n) ( (n<32) ? (xh>>n) : 0 )
#define Rl(xh,xl,n) ( (n<32) ? ((xh<<(32-n))|(xl>>n)) : (xh>>(n-32)) )

/* These can be done independently on high and low parts */
#define Choice(x,y,z)  ( ((x) & (y)) | (~(x) & (z)) )
#define Maj(x,y,z) ( ( (x) & (y) ) | ( (x) & (z) ) | ( (y) & (z) ) )

/* Sigma functions */
#define SIG0h(xh,xl) ( Sh(xh,xl,28) ^ Sh(xh,xl,34) ^ Sh(xh,xl,39) )
#define SIG0l(xh,xl) ( Sl(xh,xl,28) ^ Sl(xh,xl,34) ^ Sl(xh,xl,39) )
#define SIG1h(xh,xl) ( Sh(xh,xl,14) ^ Sh(xh,xl,18) ^ Sh(xh,xl,41) )
#define SIG1l(xh,xl) ( Sl(xh,xl,14) ^ Sl(xh,xl,18) ^ Sl(xh,xl,41) )
#define sig0h(xh,xl) ( Sh(xh,xl, 1) ^ Sh(xh,xl, 8) ^ Rh(xh,xl, 7) )
#define sig0l(xh,xl) ( Sl(xh,xl, 1) ^ Sl(xh,xl, 8) ^ Rl(xh,xl, 7) )
#define sig1h(xh,xl) ( Sh(xh,xl,19) ^ Sh(xh,xl,61) ^ Rh(xh,xl, 6) )
#define sig1l(xh,xl) ( Sl(xh,xl,19) ^ Sl(xh,xl,61) ^ Rl(xh,xl, 6) )

/* Add with carry, x += y */
#define ADDC(xh,xl,yh,yl) ( xl+=(yl), xh+=(yh)+(xl<(yl)) )

/* Stored as high, then low words */
static const uint32 K[] = {
        0x428a2f98, 0xd728ae22, 0x71374491, 0x23ef65cd,
        0xb5c0fbcf, 0xec4d3b2f, 0xe9b5dba5, 0x8189dbbc,
        0x3956c25b, 0xf348b538, 0x59f111f1, 0xb605d019,
        0x923f82a4, 0xaf194f9b, 0xab1c5ed5, 0xda6d8118,
        0xd807aa98, 0xa3030242, 0x12835b01, 0x45706fbe,
        0x243185be, 0x4ee4b28c, 0x550c7dc3, 0xd5ffb4e2,
        0x72be5d74, 0xf27b896f, 0x80deb1fe, 0x3b1696b1,
        0x9bdc06a7, 0x25c71235, 0xc19bf174, 0xcf692694,
        0xe49b69c1, 0x9ef14ad2, 0xefbe4786, 0x384f25e3,
        0x0fc19dc6, 0x8b8cd5b5, 0x240ca1cc, 0x77ac9c65,
        0x2de92c6f, 0x592b0275, 0x4a7484aa, 0x6ea6e483,
        0x5cb0a9dc, 0xbd41fbd4, 0x76f988da, 0x831153b5,
        0x983e5152, 0xee66dfab, 0xa831c66d, 0x2db43210,
        0xb00327c8, 0x98fb213f, 0xbf597fc7, 0xbeef0ee4,
        0xc6e00bf3, 0x3da88fc2, 0xd5a79147, 0x930aa725,
        0x06ca6351, 0xe003826f, 0x14292967, 0x0a0e6e70,
        0x27b70a85, 0x46d22ffc, 0x2e1b2138, 0x5c26c926,
        0x4d2c6dfc, 0x5ac42aed, 0x53380d13, 0x9d95b3df,
        0x650a7354, 0x8baf63de, 0x766a0abb, 0x3c77b2a8,
        0x81c2c92e, 0x47edaee6, 0x92722c85, 0x1482353b,
        0xa2bfe8a1, 0x4cf10364, 0xa81a664b, 0xbc423001,
        0xc24b8b70, 0xd0f89791, 0xc76c51a3, 0x0654be30,
        0xd192e819, 0xd6ef5218, 0xd6990624, 0x5565a910,
        0xf40e3585, 0x5771202a, 0x106aa070, 0x32bbd1b8,
        0x19a4c116, 0xb8d2d0c8, 0x1e376c08, 0x5141ab53,
        0x2748774c, 0xdf8eeb99, 0x34b0bcb5, 0xe19b48a8,
        0x391c0cb3, 0xc5c95a63, 0x4ed8aa4a, 0xe3418acb,
        0x5b9cca4f, 0x7763e373, 0x682e6ff3, 0xd6b2b8a3,
        0x748f82ee, 0x5defb2fc, 0x78a5636f, 0x43172f60,
        0x84c87814, 0xa1f0ab72, 0x8cc70208, 0x1a6439ec,
        0x90befffa, 0x23631e28, 0xa4506ceb, 0xde82bde9,
        0xbef9a3f7, 0xb2c67915, 0xc67178f2, 0xe372532b,
        0xca273ece, 0xea26619c, 0xd186b8c7, 0x21c0c207,
        0xeada7dd6, 0xcde0eb1e, 0xf57d4f7f, 0xee6ed178,
        0x06f067aa, 0x72176fba, 0x0a637dc5, 0xa2c898a6,
        0x113f9804, 0xbef90dae, 0x1b710b35, 0x131c471b,
        0x28db77f5, 0x23047d84, 0x32caab7b, 0x40c72493,
        0x3c9ebe0a, 0x15c9bebc, 0x431d67c4, 0x9c100d4c,
        0x4cc5d4be, 0xcb3e42b6, 0x597f299c, 0xfc657e2a,
        0x5fcb6fab, 0x3ad6faec, 0x6c44198c, 0x4a475817
};

static const uint32 H[16] = {
  0x6a09e667UL, 0xf3bcc908UL,
  0xbb67ae85UL, 0x84caa73bUL,
  0x3c6ef372UL, 0xfe94f82bUL,
  0xa54ff53aUL, 0x5f1d36f1UL,
  0x510e527fUL, 0xade682d1UL,
  0x9b05688cUL, 0x2b3e6c1fUL,
  0x1f83d9abUL, 0xfb41bd6bUL,
  0x5be0cd19UL, 0x137e2179UL
};

static void SHA512_transform( SHA512_ctx* ctx )
{
   int t;
   uint32 Ah = ctx->H[ 0 ];
   uint32 Al = ctx->H[ 1 ];
   uint32 Bh = ctx->H[ 2 ];
   uint32 Bl = ctx->H[ 3 ];
   uint32 Ch = ctx->H[ 4 ];
   uint32 Cl = ctx->H[ 5 ];
   uint32 Dh = ctx->H[ 6 ];
   uint32 Dl = ctx->H[ 7 ];
   uint32 Eh = ctx->H[ 8 ];
   uint32 El = ctx->H[ 9 ];
   uint32 Fh = ctx->H[ 10 ];
   uint32 Fl = ctx->H[ 11 ];
   uint32 Gh = ctx->H[ 12 ];
   uint32 Gl = ctx->H[ 13 ];
   uint32 Hh = ctx->H[ 14 ];
   uint32 Hl = ctx->H[ 15 ];
   uint32 T1h, T1l, T2h, T2l;
   uint32 W[ 160 ];		/* Stored as hi, lo */

   for (t = 0; t < 32; ++t)
     W[t] = uint32_get_msb(ctx->M + t*4);

   for ( t = 16; t < 80; t++ )
   {
//    W[ t ] = sig1(W[t-2]) + W[t-7] + sig0(W[t-15]) + W[t-16];
          T1h = sig1h(W[2*t-4],W[2*t-3]); T1l = sig1l(W[2*t-4],W[2*t-3]);
          ADDC( T1h, T1l, W[2*t-14], W[2*t-13] );
          ADDC( T1h, T1l, sig0h(W[2*t-30],W[2*t-29]), sig0l(W[2*t-30],W[2*t-29]) );
          ADDC( T1h, T1l, W[2*t-32], W[2*t-31] );
          W[2*t] = T1h; W[2*t+1] = T1l;
   }

   for ( t = 0; t < 80; t++ )
   {
//	  T1 = H + SIG1(E) + Ch(E,F,G) + K[t] + W[t]; 
          T1h = Hh; T1l = Hl;
          ADDC( T1h, T1l, SIG1h(Eh,El), SIG1l(Eh,El) );
          ADDC( T1h, T1l, Choice(Eh,Fh,Gh), Choice(El,Fl,Gl));
          ADDC( T1h, T1l, K[2*t], K[2*t+1] );
          ADDC( T1h, T1l, W[2*t], W[2*t+1] );
//	  T2 = SIG0(A) + Maj(A,B,C);
          T2h = SIG0h(Ah,Al); T2l = SIG0l(Ah,Al);
          ADDC( T2h, T2l, Maj(Ah,Bh,Ch), Maj(Al,Bl,Cl) );
          Hh = Gh; Hl = Gl;
          Gh = Fh; Gl = Fl;
          Fh = Eh; Fl = El;
//    E = D + T1;
          Eh = Dh; El = Dl;
          ADDC( Eh, El, T1h, T1l );
          Dh = Ch; Dl = Cl;
          Ch = Bh; Cl = Bl;
          Bh = Ah; Bl = Al;
//    A = T1 + T2;
          Ah = T1h; Al = T1l;
          ADDC( Ah, Al, T2h, T2l );
   }

   ADDC( ctx->H[ 0], ctx->H[ 1], Ah, Al );
   ADDC( ctx->H[ 2], ctx->H[ 3], Bh, Bl );
   ADDC( ctx->H[ 4], ctx->H[ 5], Ch, Cl );
   ADDC( ctx->H[ 6], ctx->H[ 7], Dh, Dl );
   ADDC( ctx->H[ 8], ctx->H[ 9], Eh, El );
   ADDC( ctx->H[10], ctx->H[11], Fh, Fl );
   ADDC( ctx->H[12], ctx->H[13], Gh, Gl );
   ADDC( ctx->H[14], ctx->H[15], Hh, Hl );
}

#endif

void SHA512_init(SHA512_ctx* ctx)
{
  memcpy(ctx->H, H, sizeof H);
  ctx->lbits = 0;
  ctx->hbits = 0;
  ctx->mlen = 0;
}

void SHA512_update(SHA512_ctx* ctx, const void* vdata, unsigned long data_len)
{
   const uint8* data = vdata;
   unsigned long use;
   uint64 low_bits;
   
   /* convert data_len to bits and add to the 128 bit word formed by lbits
      and hbits */
   ctx->hbits += data_len >> 61;
   low_bits = data_len << 3;
   ctx->lbits += low_bits;
   if (ctx->lbits < low_bits) ++ctx->hbits;

   while (data_len >= (use = 128 - ctx->mlen)) {
     memcpy(ctx->M + ctx->mlen, data, use);
     SHA512_transform(ctx);
     ctx->mlen = 0;
     data_len -= use;
     data += use;
   }
   memcpy(ctx->M + ctx->mlen, data, data_len);
   ctx->mlen += data_len;
}

void SHA512_final(SHA512_ctx* ctx, uint8* digest)
{
  unsigned i;
  ctx->M[ctx->mlen] = 0x80;
  ++ctx->mlen;
  if (ctx->mlen <= 128-16) {
    memset(ctx->M + ctx->mlen, 0x00, 128-8 - ctx->mlen);
  }
  else {
    memset(ctx->M + ctx->mlen, 0x00, 128 - ctx->mlen);
    SHA512_transform( ctx );
    memset( ctx->M, 0x00, 128-8 );
  }

  uint64_pack_msb(ctx->hbits, ctx->M+112);
  uint64_pack_msb(ctx->lbits, ctx->M+120);
  SHA512_transform( ctx );

#ifdef HAS_ULONG64
  for (i = 0; i < 8; ++i, digest += 8)
    uint64_pack_msb(ctx->H[i], digest);
#else
  for (i = 0; i < 16; ++i, digest += 4)
    uint32_pack_msb(ctx->H[i], digest);
#endif
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
  return 0;
}
#endif
#ifdef SELFTEST_EXP
ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f
8e959b75dae313da8cf4f72814fc143f8f7779c6eb9f7fa17299aeadb6889018501d289e4900f7e4331b99dec4b5433ac7d329eeb6dd26545e96e55b874be909
#endif
