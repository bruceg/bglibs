// Copyright (C) 1999 Bruce Guenter <bruceg@em.ca>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#include "cdb++.h"
#include "internal.h"

cdbmake::cdbmake()
  : head(0), split(0), hash(0), numentries(0)
{
}

cdbmake::~cdbmake()
{
  delete[] split;
  hplist* x = head;
  while(x) {
    hplist* next = x->next;
    delete x;
    x = next;
  }
}

int cdbmake::add(uint32 h, uint32 p)
{
  hplist *chead = head;

  if (!chead || (chead->num >= CDBMAKE_HPLIST)) {
    chead = new hplist;
    if (!chead) return 0;
    chead->num = 0;
    chead->next = head;
    head = chead;
  }
  chead->hp[chead->num].h = h;
  chead->hp[chead->num].p = p;
  ++chead->num;
  ++numentries;
  return 1;
}

int cdbmake::split_hashes()
{
  for (int i = 0; i < 256; ++i)
    count[i] = 0;

  for (hplist* x = head; x; x = x->next) {
    for(int i = x->num; i--; )
      ++count[255 & x->hp[i].h];
  }

  uint32 memsize = 1;
  for (int i = 0; i < 256; ++i) {
    uint32 u = count[i] * 2;
    if (u > memsize)
      memsize = u;
  }

  memsize += numentries;	// no overflow possible up to now
  uint32 u = (uint32) 0 - (uint32) 1;
  u /= sizeof(hp);
  if (memsize > u) return 0;

  split = new hp[memsize];
  if (!split) return 0;

  hash = split + numentries;

  u = 0;
  for (int i = 0;i < 256;++i) {
    u += count[i];		// bounded by numentries, so no overflow
    start[i] = u;
  }

  for (hplist* x = head; x; x = x->next) {
    int i = x->num;
    while (i--)
      split[--start[255 & x->hp[i].h]] = x->hp[i];
  }

  return 1;
}

uint32 cdbmake::throw_hash(uint32 pos, int b)
{
  uint32 c = count[b];
  uint32 len = c + c;		// no overflow possible
  pack(final + 8 * b, pos);
  pack(final + 8 * b + 4, len);

  if (len) {
    for (uint32 j = 0; j < len; ++j)
      hash[j].h = hash[j].p = 0;

    hp* hp = split + start[b];
    for (uint32 j = 0; j < count[b]; ++j) {
      uint32 where = (hp->h >> 8) % len;
      while (hash[where].p)
	if (++where == len)
	  where = 0;
      hash[where] = *hp++;
    }
  }

  return len;
}
