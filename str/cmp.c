/* $Id$ */
/* str/cmp.c - Compare part of two strings
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

/** Compare part of two strings.

This function compares two strings, starting at \c aoffset bytes into \c
a and \c boffset bytes into \c b.  The first non-zero difference is
returned.  If the portion of \c b is longer than the portion of \c a,
the result is positive.
*/
int str_cmp(const str* a, unsigned aoffset, const str* b, unsigned boffset)
{
  return str_cmpb(a, aoffset, b->s+boffset, b->len-boffset);
}

/** Compare a string against a C string. */
int str_cmps(const str* a, unsigned offset, const char* b)
{
  return str_cmpb(a, offset, b, strlen(b));
}

/** Compare a string against a binary block. */
int str_cmpb(const str* a, unsigned offset, const char* b, unsigned len)
{
  char* aptr;
  if (len + offset > a->len)
    return 1;
  for (aptr = a->s+offset; len; ++aptr, ++b, --len)
    if (*b - *aptr) return *b - *aptr;
  return 0;
}
