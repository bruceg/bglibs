/* $Id$ */
/* str/splice.c - Insert into the middle of a string.
 * Copyright (C) 2003,2005  Bruce Guenter <bruce@untroubled.org>
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

/* Insert one string into another */
int str_splice(str* s, unsigned start, unsigned len, const str* r)
{
  return str_spliceb(s, start, len, r->s, r->len);
}

/* Insert a C string into a string */
int str_splices(str* s, unsigned start, unsigned len, const char* r)
{
  return str_spliceb(s, start, len, r, strlen(r));
}

/* Insert a binary block into a string */
int str_spliceb(str* s, unsigned start, unsigned len,
		const char* r, unsigned rlen)
{
  if (start > s->len || start + len > s->len)
    return 0;
  if (rlen != len) {
    const long diff = rlen - len;
    if (!str_realloc(s, s->len + diff))
      return 0;
    memmove(s->s + start + rlen, s->s + start + len, s->len - start - len + 1);
    s->len += diff;
  }
  memcpy(s->s + start, r, rlen);
  return 1;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
void selftest(void)
{
  static str s;
  str_copyb(&s, "abcde", 5);
  debugstrfn(str_splices(&s, 6, 0, ""), &s); /* start is too large */
  debugstrfn(str_splices(&s, 4, 4, ""), &s); /* len is too large */
  debugstrfn(str_splices(&s, 0, 1, "fg"), &s); /* expand the front */
  debugstrfn(str_splices(&s, 1, 1, "hi"), &s); /* expand the middle */
  debugstrfn(str_splices(&s, 6, 1, "jk"), &s); /* expand the end */
  debugstrfn(str_splices(&s, 4, 2, "lm"), &s); /* replace */
  debugstrfn(str_splices(&s, 3, 3, "no"), &s); /* contract */
  debugstrfn(str_splices(&s, 7, 0, "pqrstuvwxyz"), &s); /* grow string */
}
#endif

#ifdef SELFTEST_EXP
result=0 len=5 size=16 s=abcde
result=0 len=5 size=16 s=abcde
result=1 len=6 size=16 s=fgbcde
result=1 len=7 size=16 s=fhibcde
result=1 len=8 size=16 s=fhibcdjk
result=1 len=8 size=16 s=fhiblmjk
result=1 len=7 size=16 s=fhinojk
result=1 len=18 size=32 s=fhinojkpqrstuvwxyz
#endif
