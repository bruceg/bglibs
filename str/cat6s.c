/* $Id$ */
/* str/cat6s.c - Append six C strings to a dynamic str
 * Copyright (C) 2001  Bruce Guenter <bruce@untroubled.org>
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

/** Append 6 C strings */
int str_cat6s(str* s, const char* a, const char* b, const char* c,
	      const char* d, const char* e, const char* f)
{
  char* ptr;
  unsigned alen = strlen(a);
  unsigned blen = strlen(b);
  unsigned clen = strlen(c);
  unsigned dlen = strlen(d);
  unsigned elen = strlen(e);
  unsigned flen = strlen(f);
  if (!str_realloc(s, s->len+alen+blen+clen+dlen+elen+flen)) return 0;
  ptr = s->s + s->len;
  s->len += alen+blen+clen+dlen+elen+flen;
  memcpy(ptr, a, alen); ptr += alen;
  memcpy(ptr, b, blen); ptr += blen;
  memcpy(ptr, c, clen); ptr += clen;
  memcpy(ptr, d, dlen); ptr += dlen;
  memcpy(ptr, e, elen); ptr += elen;
  memcpy(ptr, f, flen); ptr += flen;
  *ptr = 0;
  return 1;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
MAIN
{
  static str s;
  str_copyb(&s, "abc", 3);
  debugstrfn(str_cat6s(&s, "1", "2", "3", "4", "5", "6"), &s);
}
#endif
#ifdef SELFTEST_EXP
result=1 len=9 size=16 s=abc123456
#endif
