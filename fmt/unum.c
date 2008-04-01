/* fmt/unum.c - Format an unsigned number.
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

static unsigned rec(char* buffer, unsigned long num,
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

/** Format an unsigned integer of arbitrary base with optional padding. */
unsigned fmt_unumw(char* buffer, unsigned long num, unsigned width, char pad,
		  unsigned base, const char* digits)
{
  char* s = buffer;
  if (buffer == 0) {
    unsigned len;
    for (len = 1; num >= base; ++len)
      num /= base;
    return (width > len) ? width : len;
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
static void test(unsigned (*fn)(char*, unsigned long, unsigned, char),
		 unsigned long num, unsigned width, char pad)
{
  char buf[FMT_ULONG_LEN];
  obuf_putu(&outbuf, fn(0, num, width, pad));
  obuf_putc(&outbuf, ':');
  buf[fn(buf, num, width, pad)] = 0;
  obuf_puts(&outbuf, buf);
  NL();
}

static void testall(unsigned (*fn)(char*, unsigned long, unsigned, char),
		    unsigned long num)
{
  test(fn, num, 0, 0);
  test(fn, num, 1, ' ');
  test(fn, num, 5, ' ');
  test(fn, num, 5, '0');
}

MAIN
{
  testall(fmt_udecw, 0);
  testall(fmt_udecw, 1);
  testall(fmt_udecw, 10);
  testall(fmt_uhexw, 30);
  testall(fmt_uHexw, 30);
}
#endif
#ifdef SELFTEST_EXP
1:0
1:0
5:    0
5:00000
1:1
1:1
5:    1
5:00001
2:10
2:10
5:   10
5:00010
2:1e
2:1e
5:   1e
5:0001e
2:1E
2:1E
5:   1E
5:0001E
#endif
