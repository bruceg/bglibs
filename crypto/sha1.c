/*
 SHA-1 in C

 By Steve Reid <steve@edmweb.com>, with small changes to make it
 fit by Bruce Guenter <bruceg@em.ca>

 100% Public Domain.

 Test Vectors (from FIPS PUB 180-1)
 "abc"
 A9993E36 4706816A BA3E2571 7850C26C 9CD0D89D
 "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"
 84983E44 1C3BD26E BAAE4AA1 F95129E5 E54670F1
 A million repetitions of "a"
 34AA973C D4C4DAA4 F61EEB2B DBAD2731 6534016F
*/

#include <string.h>

#include "sysdeps.h"
#include "sha1.h"

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


/* Hash a single 512-bit block. This is the core of the algorithm. */

void SHA1Transform(uint32 state[5], const unsigned char buffer[SHA1_BLOCK_LENGTH])
{
uint32 a, b, c, d, e;
typedef union {
    unsigned char c[64];
    uint32 l[16];
} CHAR64LONG16;
CHAR64LONG16 block[1];  /* use array to appear as a pointer */
    memcpy(block, buffer, SHA1_BLOCK_LENGTH);

    /* Copy context->state[] to working vars */
    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];

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
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    /* Wipe variables */
    a = b = c = d = e = 0;
#ifdef SHA1HANDSOFF
    memset(block, '\0', sizeof(block));
#endif
}


/* SHA1Init - Initialize new context */

void SHA1Init(SHA1_CTX* context)
{
    /* SHA1 initialization constants */
    context->state[0] = 0x67452301;
    context->state[1] = 0xEFCDAB89;
    context->state[2] = 0x98BADCFE;
    context->state[3] = 0x10325476;
    context->state[4] = 0xC3D2E1F0;
    context->countlo = context->counthi = 0;
}


/* Run your data through this. */

void SHA1Update(SHA1_CTX* context, const unsigned char* data, uint32 len)
{
uint32 i;
uint32 j;

    j = context->countlo;
    if ((context->countlo += len << 3) < j)
	context->counthi++;
    context->counthi += (len>>29);
    j = (j >> 3) % SHA1_BLOCK_LENGTH;
    if ((j + len) >= SHA1_BLOCK_LENGTH) {
	i = SHA1_BLOCK_LENGTH - j;
        memcpy(&context->buffer[j], data, i);
        SHA1Transform(context->state, context->buffer);
        for ( ; i + SHA1_BLOCK_LENGTH <= len; i += SHA1_BLOCK_LENGTH) {
            SHA1Transform(context->state, &data[i]);
        }
        j = 0;
    }
    else
	i = 0;
    memcpy(&context->buffer[j], &data[i], len - i);
}


/* Add padding and return the message digest. */

void SHA1Final(SHA1_CTX* context, unsigned char digest[SHA1_DIGEST_LENGTH])
{
unsigned i;
unsigned j;
unsigned char finalcount[8];
unsigned char c;

    finalcount[0] = (context->counthi >> 24) & 0xff;
    finalcount[1] = (context->counthi >> 16) & 0xff;
    finalcount[2] = (context->counthi >> 8) & 0xff;
    finalcount[3] = context->counthi & 0xff;
    finalcount[4] = (context->countlo >> 24) & 0xff;
    finalcount[5] = (context->countlo >> 16) & 0xff;
    finalcount[6] = (context->countlo >> 8) & 0xff;
    finalcount[7] = context->countlo & 0xff;
    c = 0x80;
    SHA1Update(context, &c, 1);
    c = 0x00;
    while ((context->countlo%(SHA1_BLOCK_LENGTH*8)) != (SHA1_BLOCK_LENGTH*8-8*8))
        SHA1Update(context, &c, 1);
    SHA1Update(context, finalcount, 8);  /* Should cause a SHA1Transform() */
    for (i = j = 0; i < SHA1_DIGEST_LENGTH; i += 4, j++) {
	digest[i+0] = (unsigned char)(context->state[j] >> 24);
	digest[i+1] = (unsigned char)(context->state[j] >> 16);
	digest[i+2] = (unsigned char)(context->state[j] >>  8);
	digest[i+3] = (unsigned char)(context->state[j] >>  0);
    }
    /* Wipe variables */
    memset(context, '\0', sizeof(*context));
    memset(&finalcount, '\0', sizeof(finalcount));
}

#ifdef SHA1_SELF_TEST
#include <stdio.h>

void dc(int test, const unsigned char* d1, const unsigned char* d2)
{
  int i;
  if (memcmp(d1, d2, SHA1_DIGEST_LENGTH) != 0) {
    printf("Test %d failed:\ncomputed = ", test);
    for (i = 0; i < SHA1_DIGEST_LENGTH; i++) printf("%02x", d1[i]);
    printf("\nactual  = ");
    for (i = 0; i < SHA1_DIGEST_LENGTH; i++) printf("%02x", d2[i]);
    printf("\n");
  }
  else
    printf("Test %d succeeded.\n", test);
}

int main(void) {
  SHA1_CTX ctx;
  long i;
  unsigned char digest[SHA1_DIGEST_LENGTH];

  SHA1Init(&ctx);
  SHA1Update(&ctx, "abc", 3);
  SHA1Final(&ctx, digest);
  dc(1, digest, "\xA9\x99\x3E\x36\x47\x06\x81\x6A\xBA\x3E\x25\x71\x78\x50\xC2\x6C\x9C\xD0\xD8\x9D");

  SHA1Init(&ctx);
  SHA1Update(&ctx, "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", 56);
  SHA1Final(&ctx, digest);
  dc(2, digest, "\x84\x98\x3E\x44\x1C\x3B\xD2\x6E\xBA\xAE\x4A\xA1\xF9\x51\x29\xE5\xE5\x46\x70\xF1");

  SHA1Init(&ctx);
  for (i = 0; i < 1000000; i += 50)
    SHA1Update(&ctx, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 50);
  SHA1Final(&ctx, digest);
  dc(3, digest, "\x34\xAA\x97\x3C\xD4\xC4\xDA\xA4\xF6\x1E\xEB\x2B\xDB\xAD\x27\x31\x65\x34\x01\x6F");
  return 0;
}
#endif
