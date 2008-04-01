/* Copyright (C) 2008  Bruce Guenter <bruce@untroubled.org>
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
  unsigned i = 0;
  uint32* p;
  surf(c->generated, c->counter, c->seed);
  for (p = c->counter, i = 0; i < SURF_IN_U32; ++i, ++p)
    if (++*p > 0)
      break;
  c->used = 0;
}

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
  c->used = SURF_OUT_U32;
}

uint32 surfrand_uint32(struct surfrand* c)
{
  if (c->used >= SURF_OUT_U32)
    generate(c);
  return c->generated[c->used++];
}

double surfrand_double(struct surfrand* c)
{
  const uint32 u1 = surfrand_uint32(c);
  const uint32 u2 = surfrand_uint32(c);
  return u1 * (1.0/4294967296.0) + u2 * (1.0/4294967296.0/4294967296.0);
}

void surfrand_fill(struct surfrand* c, unsigned char* buf, unsigned len)
{
  unsigned u;
  while (len >= 4) {
    u = surfrand_uint32(c);
    memcpy(buf, &c, 4);
    len -= 4;
    buf += 4;
  }
  if (len > 0) {
    u = surfrand_uint32(c);
    memcpy(buf, &c, len);
  }
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
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
 4da5cfc1 ea6787e8 23eada9c fee73aaf 8a78e27f d0e3b9ab 4a2d6392 15cd5f15
 eb027430 c47585b0 3194c1e5 94ccda9c 80a7702e 8e79aa53 6f1da938 695a7033
 33dad740 a60dbc9a 0a2ba02b 98575e80 150c83eb 9dae96f1 b9be75e2 447861ef
 269087b5 b32525a0 5c705fc5 ed0e718e 73a7f7c1 46cdc023 94216ab1 1f1279e3
 1f646f55 46a46fc1 589f20c0 7a64359d 9f7a7b8e a705e0b5 514e2c77 07506597
#endif