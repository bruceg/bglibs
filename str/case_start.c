/* str/case_start.c - Case insensitive string prefix matching
 * Copyright (C) 2003  Bruce Guenter <bruceg@em.ca>
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
#include <ctype.h>
#include <string.h>

#include "str.h"

/** Match the prefix of the string to a binary chunk. */
int str_case_startb(const str* a, const char* b, unsigned len)
{
  const char* aptr;
  if (len > a->len) return 0;
  for (aptr = a->s; len > 0; --len) {
    char bc = *b++;
    char ac = *aptr++;
    if (isupper(bc)) bc = tolower(bc);
    if (isupper(ac)) ac = tolower(ac);
    if (ac != bc) return 0;
  }
  return 1;
}

/** Match the prefix of the string to a C string. */
int str_case_starts(const str* a, const char* b)
{
  return str_case_startb(a, b, strlen(b));
}

/** Match the prefix of the string to another string. */
int str_case_start(const str* a, const str* b)
{
  return str_case_startb(a, b->s, b->len);
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
static str s = { "Return-Path: <>\n", 16, 0 };
void t(const char* f)
{
  obuf_putu(&outbuf, str_case_starts(&s, f));
  obuf_endl(&outbuf);
}
MAIN
{
  t("Return-");
  t("return-");
  t("XYZZY");
  t("Return-Path: <>\n");
  t("Return-Path: <>\n ");
  t("return-path: <>\n");
  t("return-path: <>\n ");
}
#endif
#ifdef SELFTEST_EXP
1
1
0
1
0
1
0
#endif
