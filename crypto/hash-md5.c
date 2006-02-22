/* $Id$ */

/* md5.c - Functions to compute MD5 message digest of files or memory blocks
   according to the definition of MD5 in RFC 1321 from April 1992.
   Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.

   Written by Ulrich Drepper <drepper@gnu.ai.mit.edu>, 1995.
   Self-tests added by Bruce Guenter <bruce@untroubled.org>, 2003.
*/

#include "hash.h"
#include "md5.h"
#include "uint32.h"

struct hash_state
{
  uint32 A;
  uint32 B;
  uint32 C;
  uint32 D;
};

const struct hash_state md5_init = {
  0x67452301,
  0xefcdab89,
  0x98badcfe,
  0x10325476
};

/* These are the four functions used in the four steps of the MD5 algorithm
   and defined in the RFC 1321.  The first function is a little bit optimized
   (as found in Colin Plumbs public domain implementation).  */
/* #define FF(b, c, d) ((b & c) | (~b & d)) */
#define FF(b, c, d) (d ^ (b & (c ^ d)))
#define FG(b, c, d) FF (d, b, c)
#define FH(b, c, d) (b ^ c ^ d)
#define FI(b, c, d) (c ^ (b | ~d))

#define ROTATE_LEFT(w, s) (w = (w << s) | (w >> (32 - s)))

void md5_compress(struct hash_state* state, const unsigned char* block)
{
  uint32 A = state->A;
  uint32 B = state->B;
  uint32 C = state->C;
  uint32 D = state->D;
  uint32 correct_words[16];

  /* First round: using the given function, the context and a constant
     the next context is computed.  Because the algorithms processing
     unit is a 32-bit word and it is determined to work on words in
     little endian byte order we perhaps have to change the byte order
     before the computation.  To reduce the work for the next steps
     we store the swapped words in the array CORRECT_WORDS.  */

#define OP(a, b, c, d, k, s, T)						\
      do {								\
	  a += FF(b,c,d) + (correct_words[k]=uint32_get_lsb(block+k*4)) + T;\
	  ROTATE_LEFT(a, s);						\
	  a += b;							\
      } while (0)

  /* The strange looking constants are defined in RFC 1321 as:
   * T[i] = (int) (4294967296.0 * fabs (sin (i))), i=1..64 */

  /* Round 1.  */
  OP (A, B, C, D,  0,  7, 0xd76aa478);
  OP (D, A, B, C,  1, 12, 0xe8c7b756);
  OP (C, D, A, B,  2, 17, 0x242070db);
  OP (B, C, D, A,  3, 22, 0xc1bdceee);
  OP (A, B, C, D,  4,  7, 0xf57c0faf);
  OP (D, A, B, C,  5, 12, 0x4787c62a);
  OP (C, D, A, B,  6, 17, 0xa8304613);
  OP (B, C, D, A,  7, 22, 0xfd469501);
  OP (A, B, C, D,  8,  7, 0x698098d8);
  OP (D, A, B, C,  9, 12, 0x8b44f7af);
  OP (C, D, A, B, 10, 17, 0xffff5bb1);
  OP (B, C, D, A, 11, 22, 0x895cd7be);
  OP (A, B, C, D, 12,  7, 0x6b901122);
  OP (D, A, B, C, 13, 12, 0xfd987193);
  OP (C, D, A, B, 14, 17, 0xa679438e);
  OP (B, C, D, A, 15, 22, 0x49b40821);
      
  /* For the second to fourth round we have the possibly swapped words
     in CORRECT_WORDS.  Redefine the macro to take an additional first
     argument specifying the function to use.  */
#undef OP
#define OP(f, a, b, c, d, k, s, T)				\
  do {								\
    a += f (b, c, d) + correct_words[k] + T;			\
    ROTATE_LEFT (a, s);						\
    a += b;							\
  } while (0)

  /* Round 2.  */
  OP (FG, A, B, C, D,  1,  5, 0xf61e2562);
  OP (FG, D, A, B, C,  6,  9, 0xc040b340);
  OP (FG, C, D, A, B, 11, 14, 0x265e5a51);
  OP (FG, B, C, D, A,  0, 20, 0xe9b6c7aa);
  OP (FG, A, B, C, D,  5,  5, 0xd62f105d);
  OP (FG, D, A, B, C, 10,  9, 0x02441453);
  OP (FG, C, D, A, B, 15, 14, 0xd8a1e681);
  OP (FG, B, C, D, A,  4, 20, 0xe7d3fbc8);
  OP (FG, A, B, C, D,  9,  5, 0x21e1cde6);
  OP (FG, D, A, B, C, 14,  9, 0xc33707d6);
  OP (FG, C, D, A, B,  3, 14, 0xf4d50d87);
  OP (FG, B, C, D, A,  8, 20, 0x455a14ed);
  OP (FG, A, B, C, D, 13,  5, 0xa9e3e905);
  OP (FG, D, A, B, C,  2,  9, 0xfcefa3f8);
  OP (FG, C, D, A, B,  7, 14, 0x676f02d9);
  OP (FG, B, C, D, A, 12, 20, 0x8d2a4c8a);

  /* Round 3.  */
  OP (FH, A, B, C, D,  5,  4, 0xfffa3942);
  OP (FH, D, A, B, C,  8, 11, 0x8771f681);
  OP (FH, C, D, A, B, 11, 16, 0x6d9d6122);
  OP (FH, B, C, D, A, 14, 23, 0xfde5380c);
  OP (FH, A, B, C, D,  1,  4, 0xa4beea44);
  OP (FH, D, A, B, C,  4, 11, 0x4bdecfa9);
  OP (FH, C, D, A, B,  7, 16, 0xf6bb4b60);
  OP (FH, B, C, D, A, 10, 23, 0xbebfbc70);
  OP (FH, A, B, C, D, 13,  4, 0x289b7ec6);
  OP (FH, D, A, B, C,  0, 11, 0xeaa127fa);
  OP (FH, C, D, A, B,  3, 16, 0xd4ef3085);
  OP (FH, B, C, D, A,  6, 23, 0x04881d05);
  OP (FH, A, B, C, D,  9,  4, 0xd9d4d039);
  OP (FH, D, A, B, C, 12, 11, 0xe6db99e5);
  OP (FH, C, D, A, B, 15, 16, 0x1fa27cf8);
  OP (FH, B, C, D, A,  2, 23, 0xc4ac5665);

  /* Round 4.  */
  OP (FI, A, B, C, D,  0,  6, 0xf4292244);
  OP (FI, D, A, B, C,  7, 10, 0x432aff97);
  OP (FI, C, D, A, B, 14, 15, 0xab9423a7);
  OP (FI, B, C, D, A,  5, 21, 0xfc93a039);
  OP (FI, A, B, C, D, 12,  6, 0x655b59c3);
  OP (FI, D, A, B, C,  3, 10, 0x8f0ccc92);
  OP (FI, C, D, A, B, 10, 15, 0xffeff47d);
  OP (FI, B, C, D, A,  1, 21, 0x85845dd1);
  OP (FI, A, B, C, D,  8,  6, 0x6fa87e4f);
  OP (FI, D, A, B, C, 15, 10, 0xfe2ce6e0);
  OP (FI, C, D, A, B,  6, 15, 0xa3014314);
  OP (FI, B, C, D, A, 13, 21, 0x4e0811a1);
  OP (FI, A, B, C, D,  4,  6, 0xf7537e82);
  OP (FI, D, A, B, C, 11, 10, 0xbd3af235);
  OP (FI, C, D, A, B,  2, 15, 0x2ad7d2bb);
  OP (FI, B, C, D, A,  9, 21, 0xeb86d391);

  /* Add the starting values of the context.  */
  state->A += A;
  state->B += B;
  state->C += C;
  state->D += D;
}

void md5_extract(const struct hash_state* state, unsigned char* digest)
{
  uint32_pack_lsb(state->A, digest);
  uint32_pack_lsb(state->B, digest+4);
  uint32_pack_lsb(state->C, digest+8);
  uint32_pack_lsb(state->D, digest+12);
}

const struct hash_control_block hash_md5 = {
  .state_size = sizeof(struct hash_state),
  .block_shift = 6,
  .block_mask = 0x3f,
  .length_size = 8,
  .digest_size = 16,
  .init = &md5_init,
  .compress = md5_compress,
  .packlength = hash_pack_uint64_lsb,
  .extract = md5_extract,
  .name = "MD5",
};


#ifdef SELFTEST_MAIN
#include <stdio.h>
#include <string.h>

static void MDString(const char* str)
{
  unsigned char digest[16];
  unsigned int len = strlen(str);
  unsigned i;

  hash_block(&hash_md5, str, len, digest);
  printf("MD5 (\"%s\") = ", str);
  for (i = 0; i < 16; i++) printf("%02x", digest[i]);
  printf("\n");
}

int main(void)
{
  MDString("");
  MDString("a");
  MDString("abc");
  MDString("message digest");
  MDString("abcdefghijklmnopqrstuvwxyz");
  MDString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
  
  MDString("1234567890123456789012345678901234567890"
	   "1234567890123456789012345678901234567890");
  return 0;
}
#endif
#ifdef SELFTEST_EXP
MD5 ("") = d41d8cd98f00b204e9800998ecf8427e
MD5 ("a") = 0cc175b9c0f1b6a831c399e269772661
MD5 ("abc") = 900150983cd24fb0d6963f7d28e17f72
MD5 ("message digest") = f96b697d7cb7938d525a2f31aaf161d0
MD5 ("abcdefghijklmnopqrstuvwxyz") = c3fcd3d76192e4007dfb496cca67e13b
MD5 ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") = d174ab98d277d9f5a5611c2c9f419d9f
MD5 ("12345678901234567890123456789012345678901234567890123456789012345678901234567890") = 57edf4a22be3c955ac49da2e2107b67a
#endif
