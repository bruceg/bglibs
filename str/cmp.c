/* str/cmp.c - Compare part of two strings
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
#include <string.h>
#include "str.h"

int str_cmp(const str* a, unsigned aoffset, const str* b, unsigned boffset)
{
  return str_cmpb(a, aoffset, b->s+boffset, b->len-boffset);
}

int str_cmps(const str* a, unsigned offset, const char* b)
{
  return str_cmpb(a, offset, b, strlen(b));
}

int str_cmpb(const str* a, unsigned offset, const char* b, unsigned len)
{
  char* aptr;
  if (len + offset > a->len)
    return 1;
  for (aptr = a->s+offset; len; ++aptr, ++b, --len)
    if (*b - *aptr) return *b - *aptr;
  return 0;
}
