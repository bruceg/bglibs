/* fmt/multi.c - Format multiple items.
 * Copyright (C) 2005  Bruce Guenter <bruce@untroubled.org>
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
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include "misc.h"
#include "multi.h"
#include "number.h"
#include "str/str.h"

/** Format multiple items.

The \c format string used to describe the multiple items is related to
what is used with printf and related functions, but has one critical
difference: instead of formatted items being escaped, literal text must
be escaped.  This solves the largest security issue with using
printf-style format strings, which is the possibility of accidentally
treating untrusted text as the format string.

The format string is composed of zero or more format items.  Each item
is composed of the following parts:

<b>Zero or more flag characters:</b>

<dl>

<dt>\c # <dd>Use an "alternate form" to convert the value.  For octal
conversion, the result is prefixed with a \c 0 .  For hexadecimal
conversion, the result is prefixed with either \c 0x or \c 0X depending
on the conversion type.

<dt>\c - <dd>(not implemented) Pad on the right (left justified) instead
of on the left (default right justified).

<dt>\c 0 <dd>Pad the result with zeros instead of spaces.

</dl>

<b>Field width.</b>

The option field width is a decimal digit string specifying the minimum
field width.  If the converted value has fewer characters than the field
width, it will be padded out to the field width.

<b>Length modifier:</b>

<dl>

<dt>\c l <dd>The following integer conversion uses a \c long type.

<dt>\c ll <dd>The following integer conversion uses a \c long \c long
type.

</dl>

<b>Conversion specifier.</b>

<dl>

<dt>\c d <dt>\c i <dd>The \c int argument is converted to a signed
decimal string.

<dt>\c o <dt>\c u <dt>\c x <dt>\c X <dd>The \c unsigned \c int argument
is converted to a unsigned octal, unsigned decimal, lowercase unsigned
hexadecimal, or uppercase unsigned hexadecimal string respectively.

<dt>\c c <dd>The \c int argument is converted to an unsigned char.

<dt>\c s <dd>The \c const \c char* argument is converted.

<dt>\c S <dd>The \c const \c str* argument is converted.

<dt>\c p <dd>The \c void* argument is converted to a hexadecimal string.

<dt>\c m <dd>The result of \c strerror(errno) is formatted.

<dt>\c \\ <dd>The next character literal from the format string is
converted as with \c c conversion.

<dt>\c {string} <dd>The literal string enclosed by the parenthesis is
converted as with \c s conversion.

<dt>\c @ <dd>Formats an arbitrary object using two arguments: The first
::fmt_function argument is used to format the following \c void*
argument.  The ::fmt_function is passed the output buffer, the data
pointer, width, and pad character, and is expected to return the number
of bytes formatted.

</dl>
*/
unsigned fmt_multi(char* buffer, const char* format, ...)
{
  va_list ap;
  unsigned i;
  va_start(ap, format);
  i = fmt_multiv(buffer, format, ap);
  va_end(ap);
  return i;
}

static unsigned fmt_ullnumwa(char* buffer, unsigned long long u,
			     unsigned width, char pad,
			     unsigned base, const char* digits,
			     const char* prefix)
{
  unsigned prefixlen = 0;
  if (prefix != 0) {
    prefixlen = strlen(prefix);
    width = (width > prefixlen) ? width - prefixlen : 0;
    if (buffer != 0)
      for (; *prefix != 0; ++prefix, ++buffer)
	*buffer = *prefix;
  }
  return prefixlen + fmt_ullnumw(buffer, u, width, pad, base, digits);
}

static unsigned fmt_unumwa(char* buffer, unsigned long u,
			   unsigned width, char pad,
			   unsigned base, const char* digits,
			   const char* prefix)
{
  unsigned prefixlen = 0;
  if (prefix != 0) {
    prefixlen = strlen(prefix);
    width = (width > prefixlen) ? width - prefixlen : 0;
    if (buffer != 0)
      for (; *prefix != 0; ++prefix, ++buffer)
	*buffer = *prefix;
  }
  return prefixlen + fmt_unumw(buffer, u, width, pad, base, digits);
}

/** Format multiple items, using a va_list.

This is the core function used to format multiple items.
*/
unsigned fmt_multiv(char* buffer, const char* format, va_list ap)
{
  unsigned length;

  for (length = 0; *format != 0; ++format) {
    int pad = ' ';
    int islong = 0;
    int islonglong = 0;
    int altfmt = 0;
    int leftadj = 0;
    unsigned width;
    unsigned ilength;
    long long value = 0;
    const char* altstr;
    char conv;
    const str* strp;
    const void* voidp;
    fmt_function fn;

    for (; *format != 0; ++format) {
      switch (*format) {
      case '#': altfmt = 1; continue;
      case '-': leftadj = 1; continue;
      case '0': pad = '0'; continue;
      }
      break;
    }
    for (width = 0; *format >= '0' && *format <= '9'; ++format)
      width = (width * 10) + (*format - '0');
    while (*format == 'l') {
      ++format;
      ++islong;
    }
    islonglong = islong >= 2;
    conv = *format;
    switch (conv) {
    case 'i': case 'd': case 'o': case 'u': case 'x': case 'X':
      value = islonglong
	? va_arg(ap, long long)
	: islong
	? va_arg(ap, long)
	: va_arg(ap, int);
    }
    switch (conv) {
    case 'i':
    case 'd':
      ilength = islonglong
	? fmt_sllnumw(buffer, value, width, pad, 10, fmt_lcase_digits)
	: fmt_snumw(buffer, value, width, pad, 10, fmt_lcase_digits);
      break;
    case 'o':
      altstr = altfmt ? "0" : 0;
      ilength = islonglong
	? fmt_ullnumwa(buffer, value, width, pad, 8, fmt_lcase_digits, altstr)
	: fmt_unumwa(buffer, value, width, pad, 8, fmt_lcase_digits, altstr);
      break;
    case 'u':
      ilength = islonglong
	? fmt_ullnumw(buffer, value, width, pad, 10, fmt_lcase_digits)
	: fmt_unumw(buffer, value, width, pad, 10, fmt_lcase_digits);
      break;
    case 'x':
      altstr = altfmt ? "0x" : 0;
      ilength = islonglong
	? fmt_ullnumwa(buffer, value, width, pad, 16, fmt_lcase_digits, altstr)
	: fmt_unumwa(buffer, value, width, pad, 16, fmt_lcase_digits, altstr);
      break;
    case 'X':
      altstr = altfmt ? "0X" : 0;
      ilength = islonglong
	? fmt_ullnumwa(buffer, value, width, pad, 16, fmt_ucase_digits, altstr)
	: fmt_unumwa(buffer, value, width, pad, 16, fmt_ucase_digits, altstr);
      break;
    case 'c':
      ilength = fmt_char(buffer, va_arg(ap, int), width, pad);
      break;
    case 's':
      ilength = fmt_chars(buffer, va_arg(ap, const char*), width, pad);
      break;
    case 'S':
      strp = va_arg(ap, const str*);
      ilength = fmt_mem(buffer, strp->s, strp->len, width, pad);
      break;
    case 'p':
      ilength = fmt_unumwa(buffer, (unsigned long)va_arg(ap, void*),
			   width, pad, 16, fmt_lcase_digits, "0x");
      break;
    case 'm':
      ilength = fmt_chars(buffer, strerror(errno), width, pad);
      break;
    case '@':
      fn = va_arg(ap, fmt_function);
      voidp = va_arg(ap, const void*);
      ilength = fn(buffer, voidp, width, pad);
      break;
    case '\\':
      ilength = fmt_char(buffer, *++format, width, pad);
      break;
    case '{': 
      {
	const char* start = ++format;
	for (; *format != 0 && *format != '}'; ++format)
	  ;
	ilength = fmt_mem(buffer, start, format-start, width, pad);
      }
      break;
    default:
      ilength = 0;
    }
    if (buffer != 0)
      buffer += ilength;
    length += ilength;
  }
  return length;
}

#ifdef SELFTEST_MAIN
unsigned fmt_bool(char* buffer, const void* data, unsigned width, char pad)
{
  return fmt_chars(buffer, data ? "true" : "false", width, pad);
}

void testit(const char* format, ...)
{
  char buffer[100];
  unsigned length;
  va_list ap;

  va_start(ap, format);
  length = fmt_multiv(0, format, ap);
  va_end(ap);
  obuf_putu(&outbuf, length);
  obuf_putc(&outbuf, ':');

  va_start(ap, format);
  length = fmt_multiv(buffer, format, ap);
  va_end(ap);
  obuf_putu(&outbuf, length);
  obuf_putc(&outbuf, ':');
  obuf_write(&outbuf, buffer, length);
  obuf_endl(&outbuf);
}

MAIN
{
  testit("s{|}1s{|zzz|}5s\\|05s", "one", "two", "nine", "ten");
  testit("c\\|1c\\|5c\\|05c", 'a', 'b', 'c', 'd');
  testit("09{foo}");
  testit("o\\|x\\|X\\|i\\|d", 95, 95, 95, 95, 95);
  testit("#o\\|#x\\|#X\\|#i\\|#d", 95, 95, 95, 95, 95);
  testit("p", (void*)12345678);
  testit("o\\|lo\\|llo", 505050505U, 505050505UL, 5050505050505050505ULL);
  testit("u\\|lu\\|llu", 505050505U, 505050505UL, 5050505050505050505ULL);
  testit("x\\|lx\\|llx", 505050505U, 505050505UL, 5050505050505050505ULL);
  testit("X\\|lX\\|llX", 505050505U, 505050505UL, 5050505050505050505ULL);
  testit("i\\|li\\|lli", 505050505U, 505050505UL, 5050505050505050505ULL);
  testit("i\\|li\\|lli", -505050505U, -505050505UL, -5050505050505050505ULL);
  testit("10@\\|010@", fmt_bool, (void*)1, fmt_bool, (void*)0);
}
#endif
#ifdef SELFTEST_EXP
23:23:one|two|zzz| nine|00ten
15:15:a|b|    c|0000d
9:9:000000foo
15:15:137|5f|5F|95|95
20:20:0137|0x5f|0X5F|95|95
8:8:0xbc614e
43:43:3606472611|3606472611|430267771265752674611
39:39:505050505|505050505|5050505050505050505
34:34:1e1a7589|1e1a7589|4616ff95afab7989
34:34:1E1A7589|1E1A7589|4616FF95AFAB7989
39:39:505050505|505050505|5050505050505050505
42:42:-505050505|-505050505|-5050505050505050505
21:21:      true|00000false
#endif
