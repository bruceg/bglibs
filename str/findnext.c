/* $Id$ */
/* str/findnext.c - Find the next instance of a single character
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

/** Find the next instance of the given character, on or after \c pos */
int str_findnext(const str* s, char ch, unsigned pos)
{
  char* p;
  if (pos >= s->len) return -1;
  p = memchr(s->s+pos, ch, s->len-pos);
  if (!p) return -1;
  return p - s->s;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
void selftest(void)
{
  str s = { "01234567890123456", 16, 0 };
  obuf_puti(&outbuf, str_findnext(&s, '4', 0)); NL();
  obuf_puti(&outbuf, str_findnext(&s, '4', 4)); NL();
  obuf_puti(&outbuf, str_findnext(&s, '4', 5)); NL();
  obuf_puti(&outbuf, str_findnext(&s, '6', 7)); NL();
}
#endif
#ifdef SELFTEST_EXP
4
4
14
-1
#endif
