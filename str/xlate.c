/* $Id$ */
/* str/xlate.c - Perform 1-to-1 character translation on a string.
 * Copyright (C) 2004  Bruce Guenter <bruce@untroubled.org>
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
#include <ctype.h>
#include "str.h"

/** Substitute one character for another throughout the string.
    \returns the number of substitutions made.
 */
long str_xlate(str* s, const char* from, const char* to, unsigned nchars)
{
  int map[256];
  unsigned i;
  char* ptr;
  long count;
  int ch;
  for (i = 0; i < 256; ++i)
    map[i] = -1;
  for (i = 0; i < nchars; ++i)
    map[(unsigned char)from[i]] = (unsigned char)to[i];
  for (count = 0, i = 0, ptr = s->s; i < s->len; ++i, ++ptr)
    if ((ch = map[(unsigned char)*ptr]) != -1) {
      *ptr = ch;
      ++count;
    }
  return count;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
static str s;
MAIN 
{
  str_copys(&s, "Test string goes here.");
  obuf_putu(&outbuf, str_xlate(&s, "Ttx", "xyz", 3));
  NL();
  debugstr(&s);
}
#endif
#ifdef SELFTEST_EXP
3
len=22 size=32 s=xesy syring goes here.
#endif
