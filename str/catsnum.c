/* str/catsllnum.c - Append an signed integer.
 * Copyright (C) 2003,2005  Bruce Guenter <bruce@untroubled.org>
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
#include "bglibs/str.h"

/** Append a signed integer, optionally padded to a minimum width */
int str_catsnumw(str* s, long in, unsigned width, char pad,
		 unsigned base, const char* digits)
{
  long tmp;
  unsigned size;
  unsigned i;
  unsigned sign;
  unsigned padsize;
  
  sign = 0;
  if (in < 0) {
    sign = 1;
    in = -in;
  }
  if (in < (long)base)
    size = 1;
  else
    for (tmp = in, size = 0; tmp; tmp /= base, ++size) ;
  padsize = (width > sign+size) ? width - sign+size : 0;
  if (!str_realloc(s, s->len + padsize+sign+size)) return 0;

  /* If the padding is a zero, put it after the sign, otherwise before */
  if (pad != '0')
    while (padsize--) s->s[s->len++] = pad;
  if (sign) s->s[s->len++] = pad;
  if (pad == '0')
    while (padsize--) s->s[s->len++] = pad;
  for (i = size; i-- > 0; in /= base)
    s->s[s->len+i] = digits[in % base];
  s->len += size;
  s->s[s->len] = 0;
  return 1;
}
