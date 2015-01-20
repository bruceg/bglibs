/* surfrand.c - Random number generator based on SURF
 * Copyright (C) 2008  Bruce Guenter <bruce@untroubled.org>
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
 */
#include <string.h>

#include "surfrand.h"

static void generate(struct surfrand* c)
{
  surf(c->generated, c->counter, c->seed);
  if (!++c->counter[0])
    if (!++c->counter[1])
      if (!++c->counter[2])
	if (!++c->counter[3])
	  if (!++c->counter[4])
	    if (!++c->counter[5])
	      if (!++c->counter[6])
		if (!++c->counter[7])
		  if (!++c->counter[8])
		    if (!++c->counter[9])
		      if (!++c->counter[10])
			++c->counter[11];
  c->left = SURF_OUT_U32;
}

/** Initialize the \c surfrand structure.
 *
 * Initializes the seed in \c c from the input \c data, and sets the
 * counter to zero.  If more than \c SURF_SEED_U32 bytes of input data
 * is given, the extra data is merged into the seed.  If less, the given
 * data is repeated until the seed is filled.
 *
 * The counter is treated as a giant multi-word integer, and is
 * incremented once each time data is generated.  This makes the
 * theoretical period of this generator 8*2^32^12 or >10^116.
 */
void surfrand_init(struct surfrand* c, const uint32* data, unsigned words)
{
  uint32* ptr;
  unsigned i;
  /* If there are more bytes to use than the size of the seed,
   * add bytes together where they overlap. */
  if (words > SURF_SEED_U32) {
    memcpy(c->seed, data, sizeof c->seed);
    data += SURF_SEED_U32;
    words -= SURF_SEED_U32;
    while (words > 0) {
      for (i = 0, ptr = c->seed; words > 0 && i < SURF_SEED_U32; ++i, --words)
	*ptr++ += *data++;
    }
  }
  /* Otherwise, repeat the given bytes until the seed is filled. */
  else {
    for (i = 0, ptr = c->seed;
	 i + words < SURF_SEED_U32;
	 i += words, ptr += words)
      memcpy(ptr, data, words * 4);
    memcpy(ptr, data, sizeof c->seed - i * 4);
  }
  memset(c->counter, 0, sizeof c->counter);
  c->left = 0;
}

/** Output an random unsigned 32-bit integer.
 *
 * All the bits in the output integer are equally random. */
uint32 surfrand_uint32(struct surfrand* c)
{
  if (c->left == 0)
    generate(c);
  return c->generated[--c->left];
}

/** Output a random double precision floating-point number in the range 0-1.
 *
 * This routine uses two calls to surfrand_uint32 to fill all the
 * precision of a double precision floating-point with randomness.
 */
double surfrand_double(struct surfrand* c)
{
  const uint32 u1 = surfrand_uint32(c);
  const uint32 u2 = surfrand_uint32(c);
  return u1 * (1.0/4294967296.0) + u2 * (1.0/4294967296.0/4294967296.0);
}

/** Fill the buffer with random data */
void surfrand_fill(struct surfrand* c, unsigned char* buf, unsigned len)
{
  uint32 u;
  while (len >= 4) {
    u = surfrand_uint32(c);
    memcpy(buf, &u, 4);
    len -= 4;
    buf += 4;
  }
  if (len > 0) {
    u = surfrand_uint32(c);
    memcpy(buf, &u, len);
  }
}

/** Generate a uniformly distributed random number less than \c bound.

    The result is masked against bias by regenerating the result if it
    would cause bias.
*/
uint32 surfrand_uniform(struct surfrand* c, uint32 bound)
{
  uint32 num;
  /* 2**BITS % x == (2**BITS - x) % x == -x % x */
  uint32 min = - bound % bound;

  /* Avoid bias by keeping generating random numbers until one is within
   * [min..UINT32_MAX]. This could theoretically loop forever, but it
   * has a worst-case probability of 0.5 of success on each iteration,
   * so it should rarely need more than one iteration. */
  do {
    num = surfrand_uint32(c);
  } while (num < min);

  return num % bound;
}

#ifdef SELFTEST_MAIN
static void dumpn(struct surfrand* c, unsigned count)
{
  while (count-- > 0) {
    obuf_putc(&outbuf, ' ');
    obuf_putxw(&outbuf, surfrand_uint32(c), 8, '0');
  }
  NL();
}
MAIN
{
  uint32 seed[37] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,32,32,32,32,32 };
  struct surfrand c;

  surfrand_init(&c, seed, 32);
  dumpn(&c, 8);
  dumpn(&c, 8);
  dumpn(&c, 8);

  surfrand_init(&c, seed, 1);
  dumpn(&c, 8);

  surfrand_init(&c, seed, 37);
  dumpn(&c, 8);
}
#endif
#ifdef SELFTEST_EXP
 15cd5f15 4a2d6392 d0e3b9ab 8a78e27f fee73aaf 23eada9c ea6787e8 4da5cfc1
 695a7033 6f1da938 8e79aa53 80a7702e 94ccda9c 3194c1e5 c47585b0 eb027430
 447861ef b9be75e2 9dae96f1 150c83eb 98575e80 0a2ba02b a60dbc9a 33dad740
 1f1279e3 94216ab1 46cdc023 73a7f7c1 ed0e718e 5c705fc5 b32525a0 269087b5
 07506597 514e2c77 a705e0b5 9f7a7b8e 7a64359d 589f20c0 46a46fc1 1f646f55
#endif
