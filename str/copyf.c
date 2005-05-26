/* str/copyf.c - Copy formatted data into a string
 * Copyright (C) 2005  Bruce Guenter <bruceg@em.ca>
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
#include "str.h"
#include "fmt/multi.h"

/** Copy formatted data using \c fmt_multi */
int str_copyf(str* s, const char* format, ...)
{
  unsigned length;
  va_list ap;

  va_start(ap, format);
  length = fmt_multiv(0, format, ap);
  va_end(ap);

  if (!str_ready(s, length))
    return 0;

  va_start(ap, format);
  fmt_multiv(s->s, format, ap);
  va_end(ap);

  s->s[s->len = length] = 0;
  return 1;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
void selftest(void)
{
  static str s;
  debugstrfn(str_copyf(&s, "d{bar}x", 10, 20), &s);
}
#endif
#ifdef SELFTEST_EXP
result=1 len=7 size=16 s=10bar14
#endif
