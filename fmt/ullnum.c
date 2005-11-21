/* $Id$ */
/* fmt/ullnum.c - Format an unsigned long long number.
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
#include <string.h>
#include "number.h"

static unsigned rec(char* buffer, unsigned long long num,
		    unsigned width, char pad,
		    unsigned base, const char* digits)
{
  char* s = buffer;
  if (width) --width;
  if (num >= base)
    s += rec(s, num/base, width, pad, base, digits);
  else {
    memset(s, pad, width);
    s += width;
  }
  *s++ = digits[num % base];
  return s - buffer;
}

/** Format an unsigned long long integer of arbitrary base with optional
    padding. */
unsigned fmt_ullnumw(char* buffer, unsigned long long num,
		     unsigned width, char pad,
		     unsigned base, const char* digits)
{
  char* s = buffer;
  if (buffer == 0) {
    unsigned len;
    for (len = 0; num > 0; ++len)
      num /= base;
    return len + ((width > len) ? width - len : 0);
  }
  if (num < base) {
    if (width) {
      --width;
      memset(buffer, pad, width);
      s = buffer + width;
    }
    *s++ = digits[num];
  }
  else
    s = buffer + rec(buffer, num, width, pad, base, digits);
  return s - buffer;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
static void test(unsigned (*fn)(char*, unsigned long long, unsigned, char),
		 unsigned long long num, unsigned width, char pad)
{
  char buf[FMT_ULONG_LEN];
  obuf_putu(&outbuf, fn(0, num, width, pad));
  obuf_putc(&outbuf, ':');
  buf[fn(buf, num, width, pad)] = 0;
  obuf_puts(&outbuf, buf);
  NL();
}

MAIN
{
  test(fmt_ulldecw, 10, 0,   0);
  test(fmt_ulldecw, 10, 1, ' ');
  test(fmt_ulldecw, 10, 5, ' ');
  test(fmt_ulldecw, 10, 5, '0');
  test(fmt_ullhexw, 30, 0,   0);
  test(fmt_ullhexw, 30, 5, ' ');
  test(fmt_ullhexw, 30, 5, '0');
  test(fmt_ullHexw, 30, 0,   0);
  test(fmt_ullHexw, 30, 5, ' ');
  test(fmt_ullHexw, 30, 5, '0');
}
#endif
#ifdef SELFTEST_EXP
2:10
2:10
5:   10
5:00010
2:1e
5:   1e
5:0001e
2:1E
5:   1E
5:0001E
#endif
