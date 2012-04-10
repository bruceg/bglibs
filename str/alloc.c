/* str/alloc.c - Allocate storage for a string
 * Copyright (C) 2002-2005  Bruce Guenter <bruce@untroubled.org>
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
#include <string.h>
#include "str.h"

/** Allocate storage for a string.
 *
 * \param s String to modify.
 * \param size Minimum number of bytes for which to allocate.
 * \param copy If set, the existing string will be copied into the new string.
 *
 * The size given is incremented to account for adding a trailing \c NUL
 * byte (to ensure compatibility with C string functions) and is then
 * rounded up to the nearest \c STR_BLOCKSIZE interval.
 */
int str_alloc(str* s, unsigned size, int copy)
{
  char* news;
  unsigned newsize;
  /* overflow protection */
  if ((newsize = size + 1) < size)
    return 0;
  if (newsize >= s->size) {
    newsize += newsize/8 + STR_BLOCKSIZE-1;
    newsize -= newsize % STR_BLOCKSIZE;
    /* more overflow protection */
    if (newsize <= size)
      return 0;
    if ((news = copy ? realloc(s->s, newsize) : malloc(newsize)) == 0)
      return 0;
    if (!copy)
      free(s->s);
    s->size = newsize;
    s->s = news;
  }
  return 1;
}

#ifdef SELFTEST_MAIN
void test(str* s, unsigned size)
{
  debugstrfn(str_alloc(s, size, 0), s);
}
MAIN
{
  static str s;
  test(&s, 0);
  test(&s, 1);
  test(&s, 14);
  test(&s, 15);
  test(&s, 16);
}
#endif
#ifdef SELFTEST_EXP
result=1 len=0 size=16 s=
result=1 len=0 size=16 s=
result=1 len=0 size=16 s=
result=1 len=0 size=32 s=
result=1 len=0 size=32 s=
#endif
