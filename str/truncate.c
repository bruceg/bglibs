/* str/truncate.c - Truncate a string to a particular length
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
