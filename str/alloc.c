/* str/alloc.c - Allocate storage for a string
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
  if ((newsize = size + 1) < size) return 0;
  if (newsize >= s->size) {
    newsize += newsize/8 + STR_BLOCKSIZE-1;
    newsize -= newsize % STR_BLOCKSIZE;
    if (newsize <= size) return 0;
    if ((news = malloc(newsize)) == 0) return 0;
    if (s->s) {
      if (copy) memcpy(news, s->s, s->len+1);
      free(s->s);
    }
    s->size = newsize;
    s->s = news;
  }
  return 1;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
void test(str* s, unsigned size)
{
  debugstrfn(str_alloc(s, size, 0), s);
}
void selftest(void)
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
