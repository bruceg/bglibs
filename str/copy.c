/* str/copy.c - Copy one string into another
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

int str_copy(str* s, str* in)
{
  return str_copyb(s, in->s, in->len);
}

int str_copys(str* s, const char* in)
{
  return str_copyb(s, in, strlen(in));
}

int str_copyb(str* s, const char* in, unsigned len)
{
  if (!str_ready(s, len)) return 0;
  memcpy(s->s, in, len);
  s->len = len;
  return 1;
}
