/* str/copy4s.c - Copy four C strings into a dynamic str
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

/** Copy in the concatenation of 4 C strings */
int str_copy4s(str* s, const char* a, const char* b, const char* c,
	       const char* d)
{
  char* ptr;
  unsigned alen = strlen(a);
  unsigned blen = strlen(b);
  unsigned clen = strlen(c);
  unsigned dlen = strlen(d);
  unsigned total = alen + blen + clen + dlen;
  if (!str_ready(s, total)) return 0;
  s->len = total;
  ptr = s->s;
  memcpy(ptr, a, alen); ptr += alen;
  memcpy(ptr, b, blen); ptr += blen;
  memcpy(ptr, c, clen); ptr += clen;
  memcpy(ptr, d, dlen); ptr += dlen;
  *ptr = 0;
  return 1;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
MAIN
{
  static str s;
  debugstrfn(str_copy4s(&s, "1", "2", "3", "4"), &s);
}
#endif
#ifdef SELFTEST_EXP
result=1 len=4 size=16 s=1234
#endif
