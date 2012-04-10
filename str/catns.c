/* str/catns.c - Append N C strings to a dynamic str
 * Copyright (C) 2004,2005  Bruce Guenter <bruce@untroubled.org>
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
#include <stdarg.h>
#include <string.h>
#include "bglibs/str.h"

/** Append N C strings */
int str_catns(str* s, unsigned int count, ...)
{
  const char* ptr;
  va_list ap;
  va_start(ap, count);
  while (count-- > 0) {
    if ((ptr = va_arg(ap, const char*)) != 0)
      if (!str_cats(s, ptr))
	return 0;
  }
  va_end(ap);
  return 1;
}

#ifdef SELFTEST_MAIN
MAIN
{
  static str s;
  str_copyb(&s, "abc", 3);
  debugstrfn(str_catns(&s, 7, "1", "2", 0, "3", "4", "5", "6"), &s);
}
#endif
#ifdef SELFTEST_EXP
result=1 len=9 size=16 s=abc123456
#endif
