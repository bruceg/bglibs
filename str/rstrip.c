/* str/rstrip.c - Strip whitespace from the right of a string
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
#include <ctype.h>
#include "str.h"

/** Strip all white space from the right (end) of the string */
void str_rstrip(str* s)
{
  unsigned len;
  for (len = s->len; len > 0 && isspace(s->s[len-1]); --len) ;
  s->len = len;
  s->s[len] = 0;
}
