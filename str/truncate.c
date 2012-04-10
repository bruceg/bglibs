/* str/truncate.c - Truncate a string to a particular length
 * Copyright (C) 2001,2005  Bruce Guenter <bruce@untroubled.org>
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

/** Truncate a string.
 *
 * If \c len is larger than the current size, the string is reallocated.
 * If \c len is less than the existing length (not size), the length of
 * the string is reduced to \c len.  A \c NUL marker is placed at \c
 * len.
 */
int str_truncate(str* s, unsigned len)
{
  if (!str_realloc(s, len)) return 0;
  if (len < s->len)
    s->len = len;
  s->s[len] = 0;
  return 1;
}
