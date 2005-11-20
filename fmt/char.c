/* $Id$ */
/* fmt/char.c - Format a single character.
 * Copyright (C) 2005  Bruce Guenter <bruce@untroubled.org>
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
