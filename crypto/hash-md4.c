/* $Id$ */

/* hash-md4.c - Functions to compute MD4 message digest
   according to the definition of MD4 in RFC 1320 from April 1992.
   Copyright (C) 2000,2003,2005,2006 Bruce Guenter

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
  
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
  
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
*/

/* Based on the GNU C Library MD5 source code,
   Written by Ulrich Drepper <drepper@gnu.ai.mit.edu>, 1995.  */

#include "uint32.h"
#include "hash.h"

struct hash_state
{
  uint32 A;
  uint32 B;
  uint32 C;
  uint32 D;
};

const struct hash_state md4_init = {
  0x67452301,
  0xefcdab89,
  0x98badcfe,
  0x10325476
};

/* These are the four functions used in the four steps of the MD4 algorithm
   and defined in the RFC 1320.  The first function is a little bit optimized
   (as found in Colin Plumbs public domain MD5 implementation).  */
/* #define FF(X,Y,Z) ((X & Y) | (~X & Z)) */
#define FF(X,Y,Z) (Z ^ (X & (Y ^ Z)))
#define FG(X,Y,Z) ((X & Y) | (X & Z) | (Y & Z))
#define FH(X,Y,Z) (X ^ Y ^ Z)

void md4_compress(struct hash_state* state, const unsigned char* block)
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

#define OP(a, b, c, d, s, i)						\
      do {								\
	  a += FF(b, c, d) + (correct_words[i]=uint32_get_lsb(block+i*4));\
	  CYCLIC(a, s);							\
      } while (0)

  /* It is unfortunate that C does not provide an operator for
     cyclic rotation.  Hope the C compiler is smart enough.  */
#define CYCLIC(w, s) (w = (w << s) | (w >> (32 - s)))

  /* Round 1.  */
  OP(A,B,C,D, 3, 0); OP(D,A,B,C, 7, 1); OP(C,D,A,B,11, 2); OP(B,C,D,A,19, 3);
  OP(A,B,C,D, 3, 4); OP(D,A,B,C, 7, 5); OP(C,D,A,B,11, 6); OP(B,C,D,A,19, 7);
  OP(A,B,C,D, 3, 8); OP(D,A,B,C, 7, 9); OP(C,D,A,B,11,10); OP(B,C,D,A,19,11);
  OP(A,B,C,D, 3,12); OP(D,A,B,C, 7,13); OP(C,D,A,B,11,14); OP(B,C,D,A,19,15);
      
  /* For the second to fourth round we have the possibly swapped words
     in CORRECT_WORDS.  Redefine the macro to take an additional first
     argument specifying the function to use.  */
#undef OP
#define OP(f, a, b, c, d, k, s, T)					\
      do {								\
	  a += f (b, c, d) + correct_words[k] + T;			\
	  CYCLIC (a, s);						\
      } while (0)

  /* Round 2.  */
#define OP2(a,b,c,d,k,s) OP(FG,a,b,c,d,k,s,0x5A827999)
  OP2 (A, B, C, D,  0,  3);
  OP2 (D, A, B, C,  4,  5);
  OP2 (C, D, A, B,  8,  9);
  OP2 (B, C, D, A, 12, 13);
  OP2 (A, B, C, D,  1,  3);
  OP2 (D, A, B, C,  5,  5);
  OP2 (C, D, A, B,  9,  9);
  OP2 (B, C, D, A, 13, 13);
  OP2 (A, B, C, D,  2,  3);
  OP2 (D, A, B, C,  6,  5);
  OP2 (C, D, A, B, 10,  9);
  OP2 (B, C, D, A, 14, 13);
  OP2 (A, B, C, D,  3,  3);
  OP2 (D, A, B, C,  7,  5);
  OP2 (C, D, A, B, 11,  9);
  OP2 (B, C, D, A, 15, 13);

  /* Round 3.  */
#define OP3(a,b,c,d,k,s) OP(FH,a,b,c,d,k,s,0x6ED9EBA1)
  OP3 (A, B, C, D,  0,  3);
  OP3 (D, A, B, C,  8,  9);
  OP3 (C, D, A, B,  4, 11);
  OP3 (B, C, D, A, 12, 15);
  OP3 (A, B, C, D,  2,  3);
  OP3 (D, A, B, C, 10,  9);
  OP3 (C, D, A, B,  6, 11);
  OP3 (B, C, D, A, 14, 15);
  OP3 (A, B, C, D,  1,  3);
  OP3 (D, A, B, C,  9,  9);
  OP3 (C, D, A, B,  5, 11);
  OP3 (B, C, D, A, 13, 15);
  OP3 (A, B, C, D,  3,  3);
  OP3 (D, A, B, C, 11,  9);
  OP3 (C, D, A, B,  7, 11);
  OP3 (B, C, D, A, 15, 15);

  /* Put checksum in context given as argument.  */
  state->A += A;
  state->B += B;
  state->C += C;
  state->D += D;
}

void md4_extract(const struct hash_state* state, unsigned char* digest)
{
  uint32_pack_lsb(state->A, digest);
  uint32_pack_lsb(state->B, digest+4);
  uint32_pack_lsb(state->C, digest+8);
  uint32_pack_lsb(state->D, digest+12);
}

const struct hash_control_block hash_md4 = {
  .state_size = sizeof(struct hash_state),
  .block_shift = 6,
  .block_mask = 0x3f,
  .length_size = 8,
  .digest_size = 16,
  .init = &md4_init,
  .compress = md4_compress,
  .packlength = hash_pack_uint64_lsb,
  .extract = md4_extract,
};

#ifdef SELFTEST_MAIN
#include <stdio.h>
#include <string.h>

static void MDString(const char* str)
{
  unsigned char digest[16];
  unsigned int len = strlen(str);
  unsigned i;

  hash_block(&hash_md4, str, len, digest);
  printf("MD4 (\"%s\") = ", str);
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
MD4 ("") = 31d6cfe0d16ae931b73c59d7e0c089c0
MD4 ("a") = bde52cb31de33e46245e05fbdbd6fb24
MD4 ("abc") = a448017aaf21d8525fc10ae87aa6729d
MD4 ("message digest") = d9130a8164549fe818874806e1c7014b
MD4 ("abcdefghijklmnopqrstuvwxyz") = d79e1c308aa5bbcdeea8ed63df412da9
MD4 ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") = 043f8582f241db351ce627e153e7f0e4
MD4 ("12345678901234567890123456789012345678901234567890123456789012345678901234567890") = e33b4ddc9c38f2199c3e7b164fcc0536
#endif
