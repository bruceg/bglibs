/* str/match.c - Simple pattern matching
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

int str_match(const str* s, const char* pattern)
{
  const char* sptr;
  long slen;
  char p;
  slen = s->len;
  if (*pattern == 0) return slen == 0;
  for (sptr = s->s; (p = *pattern) != 0; ++pattern, ++sptr, --slen) {
    if (p == '*') {
      if ((p = *++pattern) == 0) return 1;
      while (slen > 0 && *sptr != p) ++sptr, --slen;
      if (slen == 0) return 0;
    }
    else
      if (slen == 0 || *sptr != p) return 0;
  }
  return slen == 0;
}
