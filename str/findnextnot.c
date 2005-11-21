/* $Id$ */
/* str/findnextnot.c - Find the next character in a string not of a set
 * Copyright (C) 2001  Bruce Guenter <bruce@untroubled.org>
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
#include "str.h"

/** Find the next instance of a character not in the given list of
    characters, on or after \c pos */
int str_findnextnot(const str* s, const char* list, unsigned pos)
{
  char* p;
  int map[256];
  if (pos >= s->len) return -1;
  str_buildmap(map, list);
  for (p = s->s + pos; pos < s->len; pos++, p++)
    if (map[*(unsigned char*)p] < 0)
      return pos;
  return -1;
}
