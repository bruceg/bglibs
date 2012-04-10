/* str/free.c - Free the storage allocated by a string
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
#include "bglibs/str.h"

/** Free a string's storage.
 * This function also sets all the members of the structure to zero.
 */
void str_free(str* s)
{
  if (s->s) free(s->s);
  s->s = 0;
  s->len = 0;
  s->size = 0;
}

#ifdef SELFTEST_MAIN
MAIN
{
  static str s;
  str_alloc(&s, 1, 0);
  debugstr(&s);
  str_free(&s);
  debugstr(&s);
}
#endif
#ifdef SELFTEST_EXP
len=0 size=16 s=
len=0 size=0 s is NULL
#endif
