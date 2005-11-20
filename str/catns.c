/* $Id$ */
/* str/catns.c - Append N C strings to a dynamic str
 * Copyright (C) 2004  Bruce Guenter <bruce@untroubled.org>
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
#include "selftest.c"
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
