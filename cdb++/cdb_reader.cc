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

#define FAIL do{ abort(); return 0; }while(0)

inline uint32 max(uint32 a, uint32 b)
{
  return a > b ? a : b;
}

cdb_reader::cdb_reader(const mystring& filename)
  : in(filename.c_str()),
    failed(!in),
    eof(false)
{
  firstrec();
}

cdb_reader::~cdb_reader()
{
}

void cdb_reader::abort()
{
  failed = true;
  in.close();
}

bool cdb_reader::firstrec()
{
  if(failed) return false;
  if(!in.seek(0) || !in.read(header, 2048)) {
    abort();
    return false;
  }
  eof = false;
  eod = unpack(header);
  pos = 2048;
  return true;
}

datum* cdb_reader::nextrec()
{
  if(eof) return 0;
  if(pos >= eod) {
    eof = true;
    return 0;
  }
  if(failed || eod-pos < 8 || !in.seek(pos)) FAIL;
  pos += 8;
  unsigned char buf[8];
  if(!in.read(buf, 8)) FAIL;
  uint32 klen = unpack(buf);
  uint32 dlen = unpack(buf+4);
  if (eod - pos < klen) FAIL;
  pos += klen;
  if (eod - pos < dlen) FAIL;
  pos += dlen;
  char tmp[max(klen, dlen)];
  if(!in.read(tmp, klen)) FAIL;
  mystring key(tmp, klen);
  if(!in.read(tmp, dlen)) FAIL;
  mystring data(tmp, dlen);
  return new datum(key, data);
}
