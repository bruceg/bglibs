/* str/catiwll.c - Append an signed long long integer
 * Copyright (C) 2002,2005  Bruce Guenter <bruce@untroubled.org>
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
#include "bglibs/str.h"

/** Append a signed long long integer in decimal. */
int str_catill(str* s, long long in)
{
  return str_catsllnumw(s, in, 0, 0, 10, str_lcase_digits);
}

/** Append a signed long long integer in decimal, padded to a minimum
    width. */
int str_catiwll(str* s, long long in, unsigned width, char pad)
{
  return str_catsllnumw(s, in, width, pad, 10, str_lcase_digits);
}
