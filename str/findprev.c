/* $Id$ */
/* str/findprev.c - Find the previous instance of a character
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
#include "str.h"

/** Find the previous instance of the given character on or before \c pos */
int str_findprev(const str* s, char ch, unsigned pos)
{
  char* p;
  if (s->len > 0) {
    if (pos >= s->len)
      pos = s->len - 1;
    for (p = s->s + pos; p >= s->s; --p)
      if (*p == ch)
	return p - s->s;
  }
  return -1;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
void selftest(void)
{
  str s = { "01234567890123456", 16, 0 };
  str e = { 0, 0, 0 };
  obuf_puti(&outbuf, str_findprev(&s, '6', 10)); NL();
  obuf_puti(&outbuf, str_findprev(&s, '6', 6)); NL();
  obuf_puti(&outbuf, str_findprev(&s, '6', 5)); NL();
  obuf_puti(&outbuf, str_findprev(&s, '4', -1)); NL();
  obuf_puti(&outbuf, str_findprev(&s, '6', -1)); NL();
  obuf_puti(&outbuf, str_findprev(&e, '6', -1)); NL();
}
#endif
#ifdef SELFTEST_EXP
6
6
-1
14
6
-1
#endif
