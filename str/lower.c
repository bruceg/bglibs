/* str/lower.c - Translate a string into lowercase
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

/** Translate all upper-case characters to lower-case */
void str_lower(str* s)
{
  char* ptr;
  unsigned i;
  for (i = 0, ptr = s->s; i < s->len; ++i, ++ptr)
    if (isupper(*ptr)) *ptr = tolower(*ptr);
}
