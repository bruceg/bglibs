/* str/findnextof.c - Find the next character of a set in a string
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
#include "str.h"

int str_findnextof(str* s, const char* list, unsigned pos)
{
  int map[256];
  unsigned i;
  char* p;
  if (pos >= s->len) return -1;
  str_buildmap(map, list);
  for (i = pos, p = s->s+pos; i < s->len; i++, p++)
    if (map[*(unsigned char*)p] >= 0)
      return i;
  return -1;
}
