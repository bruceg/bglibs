/* fmt/sllnum.c - Format a signed long long integer.
 * Copyright (C) 2004  Bruce Guenter <bruceg@em.ca>
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
#include "number.h"

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
    for (len = 0; num > 0; ++len)
      num /= base;
    return len + ((width > len) ? width - len : 0) + sign;
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
#include "selftest.c"
void test(long long num, unsigned width, char pad)
{
  char buf[FMT_ULONG_LEN];
  obuf_putu(&outbuf, fmt_sdecw(0, num, width, pad));
  obuf_putc(&outbuf, ':');
  buf[fmt_sdecw(buf, num, width, pad)] = 0;
  obuf_puts(&outbuf, buf);
  NL();
}

MAIN
{
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
2:10
3:-10
2:10
3:-10
5:   10
5:00010
5:  -10
5:-0010
#endif