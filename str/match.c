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
#include <ctype.h>
#include <string.h>

#include "str.h"

/** Simple but fast (linear time) pattern matching on binary pattern. */
int str_matchb(const str* s, const char* pptr, unsigned plen, int ncase)
{
  const char* sptr;
  long slen = s->len;
  if (plen == 0) return slen == 0;
  for (sptr = s->s; plen > 0; ++sptr, --slen, ++pptr, --plen) {
    char p = *pptr;
    char c;
    if (p == '*') {
      ++pptr, --plen;
      if (plen == 0) return 1;
      p = *pptr;
      if (ncase && isupper(p)) p = tolower(p);
      while (slen > 0) {
	c = *sptr;
	if (ncase && isupper(c)) c = tolower(c);
	if (c == p) break;
	++sptr, --slen;
      }
      if (slen == 0) return 0;
    }
    else {
      if (slen == 0) return 0;
      c = *sptr;
      if (ncase) {
	if (isupper(c)) c = tolower(c);
	if (isupper(p)) p = tolower(p);
      }
      if (c != p) return 0;
    }
  }
  return slen == 0;
}

/** Simple pattern match on dynamic string pattern. */
int str_match(const str* s, const str* pattern, int ncase)
{
  return str_matchb(s, pattern->s, pattern->len, ncase);
}

/** Simple pattern match on C string pattern. */
int str_matchs(const str* s, const char* pattern, int ncase)
{
  return str_matchb(s, pattern, strlen(pattern), ncase);
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
static const str s = { "abc", 3, 0 };
void t(const char* pattern, int ncase)
{
  obuf_puts(&outbuf, pattern);
  obuf_putc(&outbuf, ' ');
  obuf_puti(&outbuf, ncase);
  obuf_putc(&outbuf, ' ');
  debugfn(str_matchs(&s, pattern, ncase));
}
MAIN
{
  t("", 0);
  t("*", 0);
  t("**", 0);
  t("abc", 0);
  t("ABC", 0);
  t("abc", 1);
  t("ABC", 1);
  t("a", 0);
  t("a*", 0);
  t("*a*", 0);
  t("*a", 0);
  t("b*", 0);
  t("*b*", 0);
  t("*b", 0);
  t("c*", 0);
  t("*c*", 0);
  t("*c", 0);
  t("d*", 0);
  t("*d*", 0);
  t("*d", 0);
  t("*C", 0);
  t("*c", 1);
  t("*C", 1);
}
#endif
#ifdef SELFTEST_EXP
 0 result=0
* 0 result=1
** 0 result=0
abc 0 result=1
ABC 0 result=0
abc 1 result=1
ABC 1 result=1
a 0 result=0
a* 0 result=1
*a* 0 result=1
*a 0 result=0
b* 0 result=0
*b* 0 result=1
*b 0 result=0
c* 0 result=0
*c* 0 result=1
*c 0 result=1
d* 0 result=0
*d* 0 result=0
*d 0 result=0
*C 0 result=0
*c 1 result=1
*C 1 result=1
#endif
