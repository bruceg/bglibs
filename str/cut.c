/* str/cut.c - Cut characters from the left or right of a string.
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

/** Cut \c count bytes from the right (end) of the string */
void str_rcut(str* s, unsigned count)
{
  unsigned newlen;
  newlen = (s->len < count) ? 0 : s->len - count;
  str_truncate(s, newlen);
}

/** Cut \c count bytes from the left (front) of the string */
void str_lcut(str* s, unsigned count)
{
  unsigned newlen;
  newlen = (s->len < count) ? 0 : s->len - count;
  if (count > s->len)
    newlen = 0;
  else {
    newlen = s->len - count;
    memmove(s->s, s->s+count, newlen);
  }
  str_truncate(s, newlen);
}
