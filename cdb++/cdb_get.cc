// Copyright (C) 1999,2000 Bruce Guenter <bruceg@em.ca>
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

static int match(fdibuf& in, const mystring& key)
{
  unsigned len = key.length();
  const char* keyptr = key.c_str();
  
  while (len > 0) {
    char buf[32];
    unsigned n = sizeof buf;
    if(n > len) n = len;
    if(!in.read(buf, n)) return -1;
    for(unsigned i = 0;i < n;++i)
      if(buf[i] != keyptr[i]) return 0;
    keyptr += n;
    len -= n;
  }
  return 1;
}

int cdb_reader::seek(const mystring& key, uint32& dlen)
{
  if(failed)
    return -1;
  uint32 h = hash(key);
  uint32 pos = 8 * (h & 255);

  uint32 poshash = unpack(header+pos); 
  uint32 lenhash = unpack(header+pos+4);
  
  if(!lenhash) return 0;
  uint32 h2 = (h >> 8) % lenhash;

  for (uint32 loop = 0; loop < lenhash; ++loop) {
    if(!in.seek(poshash+8*h2)) return -1;
    unsigned char packbuf[8];
    if(!in.read(packbuf, 8)) return -1;
    uint32 poskd = unpack(packbuf + 4);
    if(!poskd) return 0;
    if(unpack(packbuf) == h) {
      if(!in.seek(poskd)) return -1;
      if(!in.read(packbuf, 8)) return -1;
      if(unpack(packbuf) == key.length())
	switch(match(in, key)) {
	case -1:
	  return -1;
	case 1:
	  dlen = unpack(packbuf + 4);
	  return 1;
	}
    }
    if(++h2 == lenhash) h2 = 0;
  }
  return 0;
}

datum* cdb_reader::get(const mystring& key)
{
  if(failed)
    return 0;
  uint32 len;
  switch(seek(key, len)) {
  case -1: return 0;
  case 0:  return 0;
  }
  // Big assume: data is relatively small (will fit on stack)
  char buf[len];
  datum* d = !in.read(buf, len) ? 0 : new datum(key, mystring(buf, len));
  return d;
}
