/* str/copyns.c - Copy N C strings into a dynamic str
 * Copyright (C) 2004  Bruce Guenter <bruceg@em.ca>
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
#include <stdarg.h>
#include <string.h>
#include "str.h"

/** Copy in the concatenation of N C strings */
int str_copyns(str* s, unsigned int count, ...)
{
  const char* ptr;
  va_list ap;
  va_start(ap, count);
  s->len = 0;
  while (count-- > 0) {
    if ((ptr = va_arg(ap, const char*)) != 0)
      if (!str_cats(s, ptr))
	return 0;
  }
  return 1;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
MAIN
{
  static str s;
  debugstrfn(str_copyns(&s, 7, "1", 0, "2", "3", "4", "5", "6"), &s);
}
#endif
#ifdef SELFTEST_EXP
result=1 len=6 size=16 s=123456
#endif
