/* str/catuw.c - Append an unsigned integer
 * Copyright (C) 2001  Bruce Guenter <bruceg@em.ca>
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

int str_catuw(str* s, unsigned long in, unsigned width, char pad)
{
  unsigned long tmp;
  unsigned size;
  unsigned padsize;
  unsigned i;
  
  if (in < 10)
    size = 1;
  else
    for (tmp = in, size = 0; tmp; tmp /= 10, ++size) ;
  padsize = (width > size) ? width - size : 0;
  if (!str_realloc(s, padsize+size)) return 0;
  while (padsize--)
    s->s[s->len++] = pad;
  for (i = size; i-- > 0; in /= 10)
    s->s[s->len+i] = (in % 10) + '0';
  s->len += size;
  s->s[s->len] = 0;
  return 1;
}
