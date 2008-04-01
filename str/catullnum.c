/* str/catunumw.c - Append an unsigned long long integer
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
#include "str.h"

/** Append an unsigned long long integer, optionally padded to a minimum
    width */
int str_catullnumw(str* s, unsigned long long in, unsigned width, char pad,
		   unsigned base, const char* digits)
{
  unsigned long long tmp;
  unsigned size;
  unsigned padsize;
  unsigned i;
  
  if (in < base)
    size = 1;
  else
    for (tmp = in, size = 0; tmp; tmp /= base, ++size) ;
  padsize = (width > size) ? width - size : 0;
  if (!str_realloc(s, s->len + padsize+size)) return 0;
  while (padsize--)
    s->s[s->len++] = pad;
  for (i = size; i-- > 0; in /= base)
    s->s[s->len+i] = digits[in % base];
  s->len += size;
  s->s[s->len] = 0;
  return 1;
}
