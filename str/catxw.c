/* str/catxw.c - Append an hexadecimal number
 * Copyright (C) 2002  Bruce Guenter <bruceg@em.ca>
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
#include "str.h"

static const char bin2hex[16] = "0123456789abcdef";

int str_catxw(str* s, unsigned long in, unsigned width, char pad)
{
  unsigned long tmp;
  unsigned size;
  unsigned padsize;
  unsigned i;
  
  if (in < 16)
    size = 1;
  else
    for (tmp = in, size = 0; tmp; tmp /= 16, ++size) ;
  padsize = (width > size) ? width - size : 0;
  if (!str_realloc(s, s->len + padsize+size)) return 0;
  while (padsize--)
    s->s[s->len++] = pad;
  for (i = size; i-- > 0; in /= 16)
    s->s[s->len+i] = bin2hex[in % 16];
  s->len += size;
  s->s[s->len] = 0;
  return 1;
}
