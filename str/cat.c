/* str/cat.c - Append to a string
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
#include <string.h>
#include "str.h"

int str_cat(str* s, const str* in)
{
  return str_catb(s, in->s, in->len);
}

int str_cats(str* s, const char* in)
{
  return str_catb(s, in, strlen(in));
}

int str_catc(str* s, char in)
{
  if (!str_realloc(s, s->len+1)) return 0;
  s->s[s->len++] = in;
  s->s[s->len] = 0;
  return 1;
}

int str_catb(str* s, const char* in, unsigned len)
{
  if (!str_realloc(s, s->len + len)) return 0;
  memcpy(s->s+s->len, in, len);
  s->len += len;
  s->s[s->len] = 0;
  return 1;
}
