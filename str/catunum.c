/* str/catunumw.c - Append an unsigned integer
 * Copyright (C) 2015  Bruce Guenter <bruce@untroubled.org>
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
#include "fmt.h"
#include "str.h"

/** Append an unsigned integer, optionally padded to a minimum width */
int str_catunumw(str* s, unsigned long in, unsigned width, char pad,
		 unsigned base, const char* digits)
{
  unsigned len = fmt_unumw(0, in, width, pad, base, digits);

  if (!str_realloc(s, s->len + len)) return 0;
  s->len += fmt_unumw(s->s + s->len, in, width, pad, base, digits);
  s->s[s->len] = 0;
  return 1;
}
