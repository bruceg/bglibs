/* str/sort.c - Split up a string and sort its pieces.
 * Copyright (C) 2001  Bruce Guenter <bruceg@em.ca>
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
#include <alloca.h>
#include <stdlib.h>
#include <string.h>
#include "str.h"

typedef struct
{
  const char* str;
  unsigned long len;
} sortentry;

static int str_sort_cmp(const void* aa, const void* bb)
{
  const sortentry* a = (const sortentry*)aa;
  const sortentry* b = (const sortentry*)bb;
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

int str_sort(str* s, char sep, long count)
{
  sortentry* ptrs;
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
  if (!str_copy(&tmp, s)) return 0;
  for (i = 0, ptr = tmp.s, end = tmp.s+tmp.len; i < count; ++i) {
    const char* ptrend = memchr(ptr, sep, end-ptr);
    if (ptrend == 0) ptrend = end;
    ptrs[i].str = ptr;
    ptrs[i].len = ptrend - ptr;
    ptr = ptrend + 1;
  }
  qsort(ptrs, count, sizeof(*ptrs), str_sort_cmp);
  str_truncate(s, 0);
  for (i = 0; i < count; i++) {
    str_catb(s, ptrs[i].str, ptrs[i].len);
    str_catc(s, sep);
  }
  return 1;
}
