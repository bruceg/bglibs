/* str/cat.c - Append to a string
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
#include <string.h>
#include "str.h"

/** Append another string */
int str_cat(str* s, const str* in)
{
  return str_catb(s, in->s, in->len);
}

/** Append a C string */
int str_cats(str* s, const char* in)
{
  return str_catb(s, in, strlen(in));
}

/** Append a single character */
int str_catc(str* s, char in)
{
  if (!str_realloc(s, s->len+1)) return 0;
  s->s[s->len++] = in;
  s->s[s->len] = 0;
  return 1;
}

/** Append a binary block */
int str_catb(str* s, const char* in, unsigned len)
{
  if (!str_realloc(s, s->len + len)) return 0;
  memcpy(s->s+s->len, in, len);
  s->len += len;
  s->s[s->len] = 0;
  return 1;
}
