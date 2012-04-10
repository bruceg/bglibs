/* fmt/sllnum.c - Format a signed long long integer.
 * Copyright (C) 2004,2005  Bruce Guenter <bruce@untroubled.org>
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
#include "fmt.h"

static unsigned rec(char* buffer, long long num, int sign,
		    unsigned width, char pad,
		    unsigned base, const char* digits)
{
  char* s = buffer;
  if (width) --width;
  if (num >= (long long)base)
    s += rec(s, num/base, sign, width, pad, base, digits);
  else
    s += fmt_sign_pad(s, sign, width, pad);
  *s++ = digits[num % base];
  return s - buffer;
}

/** Format a signed long long integer of arbitrary base with optional
    padding. */
unsigned fmt_sllnumw(char* buffer, long long num, unsigned width, char pad,
		     unsigned base, const char* digits)
{
  char* s = buffer;
  int sign;

  sign = 0;
  if (num < 0) {
    sign = 1;
    num = -num;
    if (width) -- width;
  }
  if (buffer == 0) {
    unsigned len;
    for (len = 1; num >= base; ++len)
      num /= base;
    return ((width > len) ? width : len) + sign;
  }
  if (num < (long long)base) {
    s += fmt_sign_pad(s, sign, width-1, pad);
    *s++ = digits[num];
  }
  else
    s += rec(s, num, sign, width, pad, base, digits);
  return s - buffer;
}

#ifdef SELFTEST_MAIN
void test(long long num, unsigned width, char pad)
{
  char buf[FMT_ULONG_LEN];
  obuf_putu(&outbuf, fmt_slldecw(0, num, width, pad));
  obuf_putc(&outbuf, ':');
  buf[fmt_sdecw(buf, num, width, pad)] = 0;
  obuf_puts(&outbuf, buf);
  NL();
}

MAIN
{
  test(  0, 0,   0);
  test( 10, 0,   0);
  test(-10, 0,   0);
  test( 10, 1, '0');
  test(-10, 1, '0');
  test( 10, 5, ' ');
  test( 10, 5, '0');
  test(-10, 5, ' ');
  test(-10, 5, '0');
}
#endif
#ifdef SELFTEST_EXP
1:0
2:10
3:-10
2:10
3:-10
5:   10
5:00010
5:  -10
5:-0010
#endif
