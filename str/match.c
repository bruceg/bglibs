/* str/match.c - Simple pattern matching
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

/** Simple but fast (linear time) pattern matching on binary pattern. */
int str_matchb(const str* s, const char* pptr, unsigned plen)
{
  const char* sptr;
  long slen = s->len;
  if (plen == 0) return slen == 0;
  for (sptr = s->s; plen > 0; ++sptr, --slen, ++pptr, --plen) {
    char p = *pptr;
    if (p == '*') {
      ++pptr, --plen;
      if (plen == 0) return 1;
      p = *pptr;
      while (slen > 0) {
	if (*sptr == p) break;
	++sptr, --slen;
      }
      if (slen == 0) return 0;
    }
    else {
      if (slen == 0) return 0;
      if (*sptr != p) return 0;
    }
  }
  return slen == 0;
}

/** Simple pattern match on dynamic string pattern. */
int str_match(const str* s, const str* pattern)
{
  return str_matchb(s, pattern->s, pattern->len);
}

/** Simple pattern match on C string pattern. */
int str_matchs(const str* s, const char* pattern)
{
  return str_matchb(s, pattern, strlen(pattern));
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
static const str s = { "abc", 3, 0 };
void t(const char* pattern)
{
  obuf_puts(&outbuf, pattern);
  obuf_putc(&outbuf, ' ');
  debugfn(str_matchs(&s, pattern));
}
MAIN
{
  t("");
  t("*");
  t("**");
  t("abc");
  t("ABC");
  t("a");
  t("a*");
  t("*a*");
  t("*a");
  t("b*");
  t("*b*");
  t("*b");
  t("c*");
  t("*c*");
  t("*c");
  t("d*");
  t("*d*");
  t("*d");
  t("*C");
}
#endif
#ifdef SELFTEST_EXP
 result=0
* result=1
** result=0
abc result=1
ABC result=0
a result=0
a* result=1
*a* result=1
*a result=0
b* result=0
*b* result=1
*b result=0
c* result=0
*c* result=1
*c result=1
d* result=0
*d* result=0
*d result=0
*C result=0
#endif
