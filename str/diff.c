/* $Id$ */
/* str/diff.c - Differentiate two strings
 * Copyright (C) 2001  Bruce Guenter <bruce@untroubled.org>
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
#include <string.h>
#include "str.h"

/** Differentiate two strings.

The first non-zero difference between \c a and \c b is returned.  If \c
a is longer than \c b and no differences are found up to the length of
\c b, then the return value is positive.  If \c b is longer than \c a
and no differences are found up to the length of \c a, then the return
value is negative.
*/
int str_diff(const str* a, const str* b)
{
  return str_diffb(a, b->s, b->len);
}

/** Differentiate a string and a C string. */
int str_diffs(const str* a, const char* b)
{
  return str_diffb(a, b, strlen(b));
}

/** Differentiate a string and a binary block. */
int str_diffb(const str* a, const char* b, unsigned len)
{
  unsigned max;
  char* aptr;
  max = a->len;
  if (max > len) max = len;
  for (aptr = a->s; max; ++aptr, ++b, --max)
    if (*b - *aptr) return *b - *aptr;
  if (a->len > len) return 1;
  if (a->len < len) return -1;
  return 0;
}
