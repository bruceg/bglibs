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

#ifndef CDBXX__INTERNAL__H__
#define CDBXX__INTERNAL__H__

#include "cdb++.h"

#define CDB_HASHSTART ((uint32) 5381)

inline void pack(unsigned char* buf, uint32 num)
{
    *buf++ = num; num >>= 8;
    *buf++ = num; num >>= 8;
    *buf++ = num; num >>= 8;
    *buf = num;
}

inline uint32 unpack(const unsigned char* p)
{
  return uint32(p[0]) |
    uint32(p[1]) << 8 |
    uint32(p[2]) << 16 |
    uint32(p[3]) << 24;
}

inline uint32 hash(const mystring& s)
{
  uint32 h = CDB_HASHSTART;
  const char* p = s.c_str();
  for(unsigned len = s.length(); len > 0; len--)
    h = ((h << 5) + h) ^ *p++;
  return h;
}

#endif
