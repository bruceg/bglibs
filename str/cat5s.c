/* str/cat5s.c - Append five C strings to a dynamic str
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
#include <string.h>
#include "str.h"

/** Append 5 C strings */
int str_cat5s(str* s, const char* a, const char* b, const char* c,
	      const char* d, const char* e)
{
  char* ptr;
  unsigned alen = strlen(a);
  unsigned blen = strlen(b);
  unsigned clen = strlen(c);
  unsigned dlen = strlen(d);
  unsigned elen = strlen(e);
  if (!str_realloc(s, s->len+alen+blen+clen+dlen+elen)) return 0;
  ptr = s->s + s->len;
  s->len += alen+blen+clen+dlen+elen;
  memcpy(ptr, a, alen); ptr += alen;
  memcpy(ptr, b, blen); ptr += blen;
  memcpy(ptr, c, clen); ptr += clen;
  memcpy(ptr, d, dlen); ptr += dlen;
  memcpy(ptr, e, elen); ptr += elen;
  *ptr = 0;
  return 1;
}

#ifdef SELFTEST_MAIN
MAIN
{
  static str s;
  str_copyb(&s, "abc", 3);
  debugstrfn(str_cat5s(&s, "1", "2", "3", "4", "5"), &s);
}
#endif
#ifdef SELFTEST_EXP
result=1 len=8 size=16 s=abc12345
#endif
