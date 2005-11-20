/* $Id$ */
/* str/join.c - Join two strings together
 * Copyright (C) 2001  Bruce Guenter <bruce@untroubled.org>
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

/** Join two strings together with exactly one instance of the seperator. */
int str_join(str* s, char sep, const str* in)
{
  return str_joinb(s, sep, in->s, in->len);
}

/** Join a C string to this string */
int str_joins(str* s, char sep, const char* in)
{
  return str_joinb(s, sep, in, strlen(in));
}

/** Join a binary block to this string */
int str_joinb(str* s, char sep, const char* in, unsigned len)
{
  unsigned len1;
  unsigned off2;
  unsigned len2;
  
  len1 = s->len;
  while (len1 > 0 && s->s[len1-1] == sep) --len1;
  
  off2 = 0;
  while (off2 < len && in[off2] == sep) ++off2;
  len2 = len - off2;
  
  if (!str_realloc(s, len1+1+len2)) return 0;
  
  s->s[len1++] = sep;
  memcpy(s->s+len1, in+off2, len2);
  s->len = len1 + len2;
  s->s[s->len] = 0;
  return 1;
}
