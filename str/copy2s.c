/* str/copy2s.c - Copy two C strings into a dynamic str
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

/** Copy in the concatenation of 2 C strings */
int str_copy2s(str* s, const char* a, const char* b)
{
  char* ptr;
  unsigned alen = strlen(a);
  unsigned blen = strlen(b);
  unsigned total = alen + blen;
  if (!str_ready(s, total)) return 0;
  s->len = total;
  ptr = s->s;
  memcpy(ptr, a, alen); ptr += alen;
  memcpy(ptr, b, blen); ptr += blen;
  *ptr = 0;
  return 1;
}

#ifdef SELFTEST_MAIN
MAIN
{
  static str s;
  debugstrfn(str_copy2s(&s, "1", "2"), &s);
}
#endif
#ifdef SELFTEST_EXP
result=1 len=2 size=16 s=12
#endif
