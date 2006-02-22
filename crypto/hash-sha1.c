/* $Id$ */

/*
 SHA-1 in C

 Test Vectors (from FIPS PUB 180-1)
 "abc"
 A9993E36 4706816A BA3E2571 7850C26C 9CD0D89D
 "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"
 84983E44 1C3BD26E BAAE4AA1 F95129E5 E54670F1
 A million repetitions of "a"
 34AA973C D4C4DAA4 F61EEB2B DBAD2731 6534016F
*/

#include <string.h>

#include "hash.h"
#include "sha1.h"
#include "uint32.h"

#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

/* blk0() and blk() perform the initial expand. */
/* I got the idea of expanding during the round function from SSLeay */
#ifdef ENDIAN_MSB
#  define blk0(i) block->l[i]
#else
#  define blk0(i) (block->l[i] = (rol(block->l[i],24)&0xFF00FF00) \
		    |(rol(block->l[i],8)&0x00FF00FF))
#endif

#define blk(i) (block->l[i&15] = rol(block->l[(i+13)&15]^block->l[(i+8)&15] \
    ^block->l[(i+2)&15]^block->l[i&15],1))

/* (R0+R1), R2, R3, R4 are the different operations used in SHA1 */
#define R0(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk0(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R1(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R2(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0x6ED9EBA1+rol(v,5);w=rol(w,30);
#define R3(v,w,x,y,z,i) z+=(((w|x)&y)|(w&x))+blk(i)+0x8F1BBCDC+rol(v,5);w=rol(w,30);
#define R4(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0xCA62C1D6+rol(v,5);w=rol(w,30);

struct hash_state
{
  uint32 H[5];
};

const struct hash_state sha1_init = {
  { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0 }
};

typedef union {
    unsigned char c[64];
    uint32 l[16];
} CHAR64LONG16;

void sha1_compress(struct hash_state* state, const unsigned char* blockp)
{
  uint32 a, b, c, d, e;
  CHAR64LONG16 block[1];  /* use array to appear as a pointer */
  memcpy(block, blockp, SHA1_BLOCK_LENGTH);

  /* Copy context->state[] to working vars */
  a = state->H[0];
  b = state->H[1];
  c = state->H[2];
  d = state->H[3];
  e = state->H[4];

  /* 4 rounds of 20 operations each. Loop unrolled. */
  R0(a,b,c,d,e, 0); R0(e,a,b,c,d, 1); R0(d,e,a,b,c, 2); R0(c,d,e,a,b, 3);
  R0(b,c,d,e,a, 4); R0(a,b,c,d,e, 5); R0(e,a,b,c,d, 6); R0(d,e,a,b,c, 7);
  R0(c,d,e,a,b, 8); R0(b,c,d,e,a, 9); R0(a,b,c,d,e,10); R0(e,a,b,c,d,11);
  R0(d,e,a,b,c,12); R0(c,d,e,a,b,13); R0(b,c,d,e,a,14); R0(a,b,c,d,e,15);
  R1(e,a,b,c,d,16); R1(d,e,a,b,c,17); R1(c,d,e,a,b,18); R1(b,c,d,e,a,19);
  R2(a,b,c,d,e,20); R2(e,a,b,c,d,21); R2(d,e,a,b,c,22); R2(c,d,e,a,b,23);
  R2(b,c,d,e,a,24); R2(a,b,c,d,e,25); R2(e,a,b,c,d,26); R2(d,e,a,b,c,27);
  R2(c,d,e,a,b,28); R2(b,c,d,e,a,29); R2(a,b,c,d,e,30); R2(e,a,b,c,d,31);
  R2(d,e,a,b,c,32); R2(c,d,e,a,b,33); R2(b,c,d,e,a,34); R2(a,b,c,d,e,35);
  R2(e,a,b,c,d,36); R2(d,e,a,b,c,37); R2(c,d,e,a,b,38); R2(b,c,d,e,a,39);
  R3(a,b,c,d,e,40); R3(e,a,b,c,d,41); R3(d,e,a,b,c,42); R3(c,d,e,a,b,43);
  R3(b,c,d,e,a,44); R3(a,b,c,d,e,45); R3(e,a,b,c,d,46); R3(d,e,a,b,c,47);
  R3(c,d,e,a,b,48); R3(b,c,d,e,a,49); R3(a,b,c,d,e,50); R3(e,a,b,c,d,51);
  R3(d,e,a,b,c,52); R3(c,d,e,a,b,53); R3(b,c,d,e,a,54); R3(a,b,c,d,e,55);
  R3(e,a,b,c,d,56); R3(d,e,a,b,c,57); R3(c,d,e,a,b,58); R3(b,c,d,e,a,59);
  R4(a,b,c,d,e,60); R4(e,a,b,c,d,61); R4(d,e,a,b,c,62); R4(c,d,e,a,b,63);
  R4(b,c,d,e,a,64); R4(a,b,c,d,e,65); R4(e,a,b,c,d,66); R4(d,e,a,b,c,67);
  R4(c,d,e,a,b,68); R4(b,c,d,e,a,69); R4(a,b,c,d,e,70); R4(e,a,b,c,d,71);
  R4(d,e,a,b,c,72); R4(c,d,e,a,b,73); R4(b,c,d,e,a,74); R4(a,b,c,d,e,75);
  R4(e,a,b,c,d,76); R4(d,e,a,b,c,77); R4(c,d,e,a,b,78); R4(b,c,d,e,a,79);

  /* Add the working vars back into context.state[] */
  state->H[0] += a;
  state->H[1] += b;
  state->H[2] += c;
  state->H[3] += d;
  state->H[4] += e;

  /* Wipe variables */
  a = b = c = d = e = 0;
}

void sha1_extract(const struct hash_state* state, unsigned char* digest)
{
  uint32_pack_msb(state->H[0], digest);
  uint32_pack_msb(state->H[1], digest+4);
  uint32_pack_msb(state->H[2], digest+8);
  uint32_pack_msb(state->H[3], digest+12);
  uint32_pack_msb(state->H[4], digest+16);
}

const struct hash_control_block hash_sha1 = {
  .state_size = sizeof(struct hash_state),
  .block_shift = 6,
  .block_mask = 0x3f,
  .length_size = 8,
  .digest_size = SHA1_DIGEST_LENGTH,
  .init = &sha1_init,
  .compress = sha1_compress,
  .packlength = hash_pack_uint64_msb,
  .extract = sha1_extract,
  .name = "SHA1",
};

#ifdef SELFTEST_MAIN
#include "selftest.c"
#include "str/str.h"

static void dump(unsigned char digest[SHA1_DIGEST_LENGTH])
{
  unsigned i;
  for (i = 0; i < SHA1_DIGEST_LENGTH; ++i)
    obuf_putXw(&outbuf, digest[i], 2, '0');
  NL();
}

static void test(const char* ptr, long len)
{
  unsigned char digest[SHA1_DIGEST_LENGTH];
  hash_block(&hash_sha1, ptr, len, digest);
  dump(digest);
}

static void test_1000000a(void)
{
  unsigned i;
  struct hash_context* c;
  unsigned char digest[SHA1_DIGEST_LENGTH];
  c = hash_alloc(&hash_sha1);
  for (i = 0; i < 1000000/64; ++i)
    hash_update(c, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 64);
  hash_finish(c, digest);
  free(c);
  dump(digest);
}

MAIN
{
  /* Test Vectors (from FIPS PUB 180-1) */
  test("abc", 3);
  test("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", 56);
  test_1000000a();
}
#endif
#ifdef SELFTEST_EXP
A9993E364706816ABA3E25717850C26C9CD0D89D
84983E441C3BD26EBAAE4AA1F95129E5E54670F1
34AA973CD4C4DAA4F61EEB2BDBAD27316534016F
#endif
