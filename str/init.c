/* str/init.c - Initialize a string to a minimum size
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
#include <stdlib.h>
#include "str.h"

/** Initialize a string, giving it a small empty allocation. */
int str_init(str* s)
{
  char* p;
  if ((p = malloc(STR_BLOCKSIZE)) == 0) return 0;
  p[0] = 0;
  s->s = p;
  s->len = 0;
  s->size = STR_BLOCKSIZE;
  return 1;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
MAIN
{
  static str s;
  debugstrfn(str_init(&s), &s);
}
#endif
#ifdef SELFTEST_EXP
result=1 len=0 size=16 s=
#endif
