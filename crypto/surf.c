/* $Id$ */
/* surf.c - Simple Unpredictable Random Function
 * Copyright (C) 2004  Bruce Guenter <bruce@untroubled.org>
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
 * This implementation of SURF is derived directly from the description
 * given in http://cr.yp.to/papers/surf.ps
 */

#include "surf.h"

#define ROTATE(x,b) (((x) << (b)) | ((x) >> (32 - (b))))

#define h(N,R) (((tmp ^ seed[N]) + a) ^ ROTATE(tmp,R))

void surf(uint32 y[8], const uint32 p[12], const uint32 seed[32])
{
  uint32 x[12];
  uint32 a;
  int i;
  int round;
  int loop;
  uint32 tmp;
  for (i = 0; i < 12; ++i)
    x[i] = p[i] ^ seed[i + 12];
  for (i = 0; i < 8; ++i)
    y[i] = seed[i + 24];
  a = 0;
  tmp = x[11];
  for (loop = 0; loop < 2; ++loop) {
    for (round = 0; round < 16; ++round) {
      a += 2654435769UL;
      tmp = x[0]  += h(0,  5);
      tmp = x[1]  += h(1,  7);
      tmp = x[2]  += h(2,  9);
      tmp = x[3]  += h(3,  13);
      tmp = x[4]  += h(4,  5);
      tmp = x[5]  += h(5,  7);
      tmp = x[6]  += h(6,  9);
      tmp = x[7]  += h(7,  13);
      tmp = x[8]  += h(8,  5);
      tmp = x[9]  += h(9,  7);
      tmp = x[10] += h(10, 9);
      tmp = x[11] += h(11, 13);
    }
    for (i = 0; i < 8; ++i)
      y[i] ^= x[i + 4];
  }
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
static void test(const uint32 s[32], const uint32 in[32])
{
  uint32 out[8];
  int i;
  surf(out, in, s);
  for (i = 0; i < 8; ++i)
    obuf_putxw(&outbuf, out[i], 8, '0');
  obuf_endl(&outbuf);
}

MAIN
{
  const uint32 seed1[32] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 };
  const uint32 in1[12] = { 0,1,2,3,4,5,6,7,8,9,10,11 };
  const uint32 seed2[32] = { 965136454,479746425,372049829,788671599,207587135,82471478,337251274,593999156,890750140,208600969,839033205,548121119,948159788,21235830,70012315,259282220,38295658,134832975,612987741,415570019,815194940,765000498,981134726,331506518,401576148,818681223,982505486,728455333,1054946385,70995844,507840924,1012835195 };
  const uint32 in2[12] = { 298244669,133876651,331758846,714669328,243927566,996052809,632023729,791929383,677656487,649629687,448549470,506001552 };
  test(seed1, in1);
  test(seed2, in1);
  test(seed1, in2);
  test(seed2, in2);
}

#endif
/* These values were generated using DJB's code directly. */
#ifdef SELFTEST_EXP
c35ed4a7919860a94af6fa9767b18dfd1d58ff12f211e47df71b3fcf62709ad3
893a76356ca0dec34e58fcff18ffc540016d8bed7d668828e3f2a8a882266572
62e3659918b277d563bde36cbf53ce42209a1d8f57c3969c4fcd2fb13969779a
47a64ff75c94e3ad725cc2188196acd4c46eb59b58185a82f49fb92f25c598db
#endif
