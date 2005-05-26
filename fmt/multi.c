/* fmt/multi.c - Format multiple items.
 * Copyright (C) 2005  Bruce Guenter <bruceg@em.ca>
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
#include <stdarg.h>
#include <string.h>
#include "multi.h"
#include "number.h"

/** Format multiple items

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

<dt>\c l <dd>(not implemented) The following conversion uses a \c long
integer type.

<dt>\c ll <dd>(not implemented) The following conversion uses a \c long
\c long integer type.

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

<dt>\c p <dd>The \c void* argument is converted to a hexadecimal string.

<dt>\c \\ <dd>The next character literal from the format string is
converted as with \c c conversion.

<dt>\c {string} <dd>The literal string enclosed by the parenthesis is
converted as with \c s conversion.

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

static unsigned fmt_char(char* buffer, int ch, unsigned width, char pad)
{
  unsigned i;
  if (buffer != 0) {
    for (i = width; i > 1; --i)
      *buffer++ = pad;
    *buffer = ch;
  }
  return width ? width : 1;
}

static unsigned fmt_mem(char* buffer, const char* s, unsigned length,
			unsigned width, char pad)
{
  unsigned i;
  if (length > width)
    width = length;
  if (buffer != 0) {
    for (i = width; i > length; --i)
      *buffer++ = pad;
    for (i = length; i > 0; --i)
      *buffer++ = *s++;
  }
  return width;
}

static unsigned fmt_str(char* buffer, const char* s, unsigned width, char pad)
{
  return fmt_mem(buffer, s, strlen(s), width, pad);
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
    int altfmt = 0;
    int leftadj = 0;
    unsigned width;
    unsigned ilength;

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
    if (*format == 'l') {
      ++format;
      islong = 1;
      if (*format == 'l') {
	++format;
	islong = 2;
      }
    }
    switch (*format) {
    case 'i':
    case 'd':
      ilength = fmt_snumw(buffer, va_arg(ap, int), width, pad,
			  10, fmt_lcase_digits);
      break;
    case 'o':
      ilength = fmt_unumwa(buffer, va_arg(ap, int), width, pad,
			   8, fmt_lcase_digits,
			   altfmt ? "0" : 0);
      break;
    case 'u':
      ilength = fmt_unumw(buffer, va_arg(ap, int), width, pad,
			  10, fmt_lcase_digits);
      break;
    case 'x':
      ilength = fmt_unumwa(buffer, va_arg(ap, int), width, pad,
			   16, fmt_lcase_digits,
			   altfmt ? "0x" : 0);
      break;
    case 'X':
      ilength = fmt_unumwa(buffer, va_arg(ap, int), width, pad,
			   16, fmt_ucase_digits,
			   altfmt ? "0X" : 0);
      break;
    case 'c':
      ilength = fmt_char(buffer, va_arg(ap, int), width, pad);
      break;
    case 's':
      ilength = fmt_str(buffer, va_arg(ap, const char*), width, pad);
      break;
    case 'p':
      ilength = fmt_ullnumwa(buffer, (unsigned long)va_arg(ap, void*),
			     width, pad, 16, fmt_lcase_digits, "0x");
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
#include "selftest.c"
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
}
#endif
#ifdef SELFTEST_EXP
23:23:one|two|zzz| nine|00ten
15:15:a|b|    c|0000d
9:9:000000foo
15:15:137|5f|5F|95|95
20:20:0137|0x5f|0X5F|95|95
8:8:0xbc614e
#endif
