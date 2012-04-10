/* fmt/chars.c - Format memory block or character string.
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
#include <string.h>
#include "bglibs/str.h"
#include "misc.h"

/* Format a block of characters */
unsigned fmt_mem(char* buffer, const char* s, unsigned length,
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

/* Format a character string terminated by ASCII \c NUL */
unsigned fmt_chars(char* buffer, const char* s, unsigned width, char pad)
{
  return fmt_mem(buffer, s, strlen(s), width, pad);
}

/* Format a dynamic string */
unsigned fmt_str(char* buffer, const str* s, unsigned width, char pad)
{
  return fmt_mem(buffer, s->s, s->len, width, pad);
}

#ifdef SELFTEST_MAIN
MAIN
{
  char buf[99];
  obuf_putu(&outbuf, fmt_chars(0, "foo", 10, ' '));
  obuf_write(&outbuf, buf, fmt_chars(buf, "foo", 10, ' '));
  obuf_endl(&outbuf);
}
#endif
#ifdef SELFTEST_EXP
10       foo
#endif
