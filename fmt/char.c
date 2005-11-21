/* $Id$ */
/* fmt/char.c - Format a single character.
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
#include "misc.h"

/** Format a single character */
unsigned fmt_char(char* buffer, int ch, unsigned width, char pad)
{
  unsigned i;
  if (buffer != 0) {
    for (i = width; i > 1; --i)
      *buffer++ = pad;
    *buffer = ch;
  }
  return width ? width : 1;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
MAIN
{
  char buf[99];
  obuf_putu(&outbuf, fmt_char(0, 'f', 5, ' '));
  obuf_write(&outbuf, buf, fmt_char(buf, 'f', 5, ' '));
  obuf_endl(&outbuf);
}
#endif
#ifdef SELFTEST_EXP
5    f
#endif
