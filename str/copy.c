/* str/copy.c - Copy one string into another
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
#include "bglibs/str.h"

/** Copy in a string */
int str_copy(str* s, const str* in)
{
  return str_copyb(s, in->s, in->len);
}

/** Copy in a C string */
int str_copys(str* s, const char* in)
{
  return str_copyb(s, in, strlen(in));
}

/** Copy in a binary block */
int str_copyb(str* s, const char* in, unsigned len)
{
  if (!str_ready(s, len)) return 0;
  memcpy(s->s, in, len);
  s->len = len;
  s->s[len] = 0;
  return 1;
}

#ifdef SELFTEST_MAIN
MAIN
{
  static str s;
  debugstrfn(str_copys(&s, "foo"), &s);
  debugstrfn(str_copyb(&s, "012345678901234567", 17), &s);
  debugstrfn(str_copys(&s, "foo"), &s);
}
#endif
#ifdef SELFTEST_EXP
result=1 len=3 size=16 s=foo
result=1 len=17 size=32 s=01234567890123456
result=1 len=3 size=32 s=foo
#endif
