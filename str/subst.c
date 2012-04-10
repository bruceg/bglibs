/* str/subst.c - Substitute one character for another
 * Copyright (C) 2004,2005  Bruce Guenter <bruce@untroubled.org>
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
#include <ctype.h>
#include "bglibs/str.h"

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
