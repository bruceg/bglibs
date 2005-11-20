/* $Id$ */
/* str/start.c - String prefix matching
 * Copyright (C) 2003  Bruce Guenter <bruce@untroubled.org>
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
#include <string.h>

#include "str.h"

/** Match the prefix of the string to a C string. */
int str_starts(const str* a, const char* b)
{
  return str_startb(a, b, strlen(b));
}

/** Match the prefix of the string to another string. */
int str_start(const str* a, const str* b)
{
  return str_startb(a, b->s, b->len);
}

/** Match the prefix of the string to a binary chunk. */
int str_startb(const str* a, const char* b, unsigned len)
{
  if (len > a->len) return 0;
  return memcmp(a->s, b, len) == 0;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
static str s = { "Return-Path: <>\n", 16, 0 };
void t(const char* f)
{
  obuf_putu(&outbuf, str_starts(&s, f));
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
0
0
1
0
0
0
#endif
