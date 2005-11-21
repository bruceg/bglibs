// Copyright (C) 1999,2000 Bruce Guenter <bruce@untroubled.org>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

#include "cdb++.h"
#include "internal.h"

char* cdb_reader::seek(const mystring& key, uint32& dlen)
{
  if(failed)
    return 0;
  uint32 h = hash(key);
  uint32 pos = 8 * (h & 255);

  uint32 poshash = unpack(map+pos); 
  uint32 lenhash = unpack(map+pos+4);
  
  if(!lenhash)
    return 0;
  uint32 h2 = (h >> 8) % lenhash;

  for (uint32 loop = 0; loop < lenhash; ++loop) {
    unsigned char* ptrhash = map + poshash + 8*h2;
    uint32 poskd = unpack(ptrhash + 4);
    if(!poskd)
      return 0;
    if(unpack(ptrhash) == h) {
      unsigned char* ptrmatch = map + poskd;
      if(unpack(ptrmatch) == key.length() &&
	 !memcmp(ptrmatch+8, key.c_str(), key.length())) {
	dlen = unpack(ptrmatch + 4);
	return (char*)ptrmatch + 8 + key.length();
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
  uint32 datalen;
  char* dataptr = seek(key, datalen);
  if(!dataptr)
    return 0;
  return new datum(key, mystring(dataptr, datalen));
}
