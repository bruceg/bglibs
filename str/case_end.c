/* str/case_start.c - Case insensitive string suffix matching
 * Copyright (C) 2010  Bruce Guenter <bruce@untroubled.org>
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
#include <ctype.h>
#include <string.h>

#include "bglibs/str.h"

/** Match the suffix of the string to a binary chunk. */
int str_case_endb(const str* a, const char* b, unsigned len)
{
  const char* aptr;
  if (len > a->len) return 0;
  for (aptr = a->s + a->len - len; len > 0; --len) {
    char bc = *b++;
    char ac = *aptr++;
    if (isupper(bc)) bc = tolower(bc);
    if (isupper(ac)) ac = tolower(ac);
    if (ac != bc) return 0;
  }
  return 1;
}

/** Match the suffix of the string to a C string. */
int str_case_ends(const str* a, const char* b)
{
  return str_case_endb(a, b, strlen(b));
}

/** Match the suffix of the string to another string. */
int str_case_end(const str* a, const str* b)
{
  return str_case_endb(a, b->s, b->len);
}

#ifdef SELFTEST_MAIN
static str s = { "AbCdEfGh", 8, 0 };
void t(const char* f)
{
  obuf_putu(&outbuf, str_case_ends(&s, f));
  obuf_endl(&outbuf);
}
MAIN
{
  t("h");
  t("H");
  t("i");
  t("Gh");
  t("GH");
  t("AbCdEfGh");
  t("XYZZY");
  t("00AbCdEfGh");
}
#endif
#ifdef SELFTEST_EXP
1
1
0
1
1
1
0
0
#endif
