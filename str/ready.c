/* str/ready.c - Ensure a string has a minimum size
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
#include <stdlib.h>
#include "str.h"

/* Make sure that the string has at least size+1 bytes of available space. */
int str_ready(str* s, unsigned size)
{
  char* news;
  unsigned long asize;
  if ((asize = size + 1) < size) return 0;
  if (asize >= s->size) {
    asize += asize/8 + STR_BLOCKSIZE-1;
    asize -= asize % STR_BLOCKSIZE;
    if (asize <= size) return 0;
    if ((news = malloc(asize)) == 0) return 0;
    if (s->s) free(s->s);
    s->size = asize;
    s->s = news;
  }
  return 1;
}
