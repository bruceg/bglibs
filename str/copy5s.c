/* str/copy5s.c - Copy five C strings into a dynamic str
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

int str_copy5s(str* s, const char* a, const char* b, const char* c,
	       const char* d, const char* e)
{
  char* ptr;
  unsigned alen = strlen(a);
  unsigned blen = strlen(b);
  unsigned clen = strlen(c);
  unsigned dlen = strlen(d);
  unsigned elen = strlen(e);
  if (!str_ready(s, alen+blen+clen+dlen+elen)) return 0;
  ptr = s->s;
  memcpy(ptr, a, alen); ptr += alen;
  memcpy(ptr, b, blen); ptr += blen;
  memcpy(ptr, c, clen); ptr += clen;
  memcpy(ptr, d, dlen); ptr += dlen;
  memcpy(ptr, e, elen); ptr += elen;
  *ptr = 0;
  return 1;
}