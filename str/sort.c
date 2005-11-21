/* $Id$ */
/* str/sort.c - Split up a string and sort its pieces.
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
#include <string.h>
#include "str.h"

static int default_cmp(const str_sortentry* a,
		       const str_sortentry* b)
{
  int i;
  unsigned long alen = a->len;
  unsigned long blen = b->len;
  if (alen < blen) {
    if ((i = memcmp(a->str, b->str, alen)) != 0) return i;
    return -1;
  }
  else if (alen > blen) {
    if ((i = memcmp(a->str, b->str, blen)) != 0) return i;
    return 1;
  }
  else
    return memcmp(a->str, b->str, blen);
}

/** Sort a string.

\param s The string to sort.
\param sep The character which delimits the substrings.
\param count The number of substrings within \c s (set to \c -1 if not known).
\param fn The comparison function.  Defaults to a function that works like \c memcmp.

\note This function allocates a temporary array of substring pointers,
and so may return \c 0 if memory allocation fails.  The string itself is
not reallocated.
*/
int str_sort(str* s, char sep, long count,
	     int (*fn)(const str_sortentry* a, const str_sortentry* b))
{
  str_sortentry* ptrs;
  const char* ptr;
  const char* end;
  long i;
  str tmp = {0,0,0};
  if (count == -1) {
    for (count = 0, ptr = s->s, end = s->s+s->len; ptr != 0 && ptr < end;
	 ++count) {
      ptr = memchr(ptr, sep, end-ptr);
      if (ptr) ++ptr;
    }
  }
  if ((ptrs = alloca(count * sizeof *ptrs)) == 0) return 0;
  if (!str_copy(&tmp, s)) { str_free(&tmp); return 0; }
  if (fn == 0) fn = default_cmp;
  for (i = 0, ptr = tmp.s, end = tmp.s+tmp.len; i < count; ++i) {
    const char* ptrend = memchr(ptr, sep, end-ptr);
    if (ptrend == 0) ptrend = end;
    ptrs[i].str = ptr;
    ptrs[i].len = ptrend - ptr;
    ptr = ptrend + 1;
  }
  qsort(ptrs, count, sizeof(*ptrs), (int (*)(const void*,const void*))fn);
  str_truncate(s, 0);
  for (i = 0; i < count; i++) {
    str_catb(s, ptrs[i].str, ptrs[i].len);
    str_catc(s, sep);
  }
  str_free(&tmp);
  return 1;
}
