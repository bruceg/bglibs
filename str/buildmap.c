/* str/buildmap.c - Build a lookup table for find*of functions
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

/** Build a map from the list of characters.

Each byte in the output map contains either \c -1 if the corresponding
character was not present in the input string, or the offset of the last
instance of the character in the list.
*/
void str_buildmap(int map[256], const char* list)
{
  unsigned i;
  for (i = 0; i < 256; i++)
    map[i] = -1;
  for (i = 0; *list; i++, list++)
    map[*(unsigned char*)list] = i;
}
