/* str/catuwll.c - Append an unsigned long long integer
 * Copyright (C) 2003  Bruce Guenter <bruceg@em.ca>
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

/** Append an unsigned long long integer in hexadecimal. */
int str_catxll(str* s, unsigned long long in)
{
  return str_catullnumw(s, in, 0, 0, 10, str_lcase_digits);
}

/** Append an unsigned long long integer in hexadecimal, padded to a
    minimum width. */
int str_catxwll(str* s, unsigned long long in, unsigned width, char pad)
{
  return str_catullnumw(s, in, width, pad, 10, str_lcase_digits);
}