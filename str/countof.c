/* $Id$ */
/* str/count.c - Count the instances of a character set
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

/** Count the number of instances of a list of characters */
unsigned str_countof(const str* s, const char* list)
{
  const char* ptr;
  unsigned pos;
  unsigned count;
  int map[256];
  
  str_buildmap(map, list);
  count = 0;
  for (pos = 0, ptr = s->s; pos < s->len; ++ptr, ++pos)
    if (map[*(unsigned char*)ptr] >= 0) ++count;
  return count;
}
