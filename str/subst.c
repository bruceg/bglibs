/* str/subst.c - Substitute one character for another
 * Copyright (C) 2004  Bruce Guenter <bruceg@em.ca>
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
#include <ctype.h>
#include "str.h"

/** Substitute one character for another throughout the string.
    \returns the number of substitutions made.
 */
long str_subst(str* s, char from, char to)
{
  char* ptr;
  unsigned i;
  long count;
  for (count = 0, i = 0, ptr = s->s; i < s->len; ++i, ++ptr)
    if (*ptr == from) {
      *ptr = to;
      ++count;
    }
  return count;
}
