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

#include <stdio.h>
#include <unistd.h>
#include "cdb++.h"
#include "internal.h"

#define safeadd(u,v) do{ uint32 newu = u+v; if(newu<u) FAIL; u=newu; }while(0)
#define FAIL do{ abort(); return false; }while(0)

void cdb_writer::abort()
{
  if(!failed) {
    unlink(fntemp.c_str());
    out.close();
    failed = true;
  }
}

cdb_writer::cdb_writer(const mystring& filename, int mode)
  : out(filename.c_str(), fdobuf::excl | fdobuf::create, mode),
    fntemp(filename),
    pos(sizeof cdbm.final),
    failed(false)
{
  if(!out)
    failed = true;
  else {
    if(!out.write(cdbm.final, sizeof cdbm.final))
      abort();
  }
}

cdb_writer::~cdb_writer()
{
  abort();
}

bool cdb_writer::put(const mystring& key, const mystring& data)
{
  if(failed) return 0;
  unsigned char packbuf[8];
  pack(packbuf, key.length());
  pack(packbuf+4, data.length());
  if(!out.write(packbuf, sizeof packbuf)) FAIL;

  uint32 h = hash(key);
  if(!out.write(key.c_str(), key.length()) ||
     !out.write(data.c_str(), data.length()) ||
     !cdbm.add(h, pos))
    FAIL;
  safeadd(pos, 8);
  safeadd(pos, key.length());
  safeadd(pos, data.length());
  return true;
}

bool cdb_writer::end(const mystring& filename)
{
  if(failed) return 0;
  if(!cdbm.split_hashes()) FAIL;
  for(unsigned i = 0; i < 256; i++) {
    uint32 len = cdbm.throw_hash(pos, i);
    for(uint32 u = 0; u < len; u++) {
      unsigned char packbuf[8];
      pack(packbuf, cdbm.hash[u].h);
      pack(packbuf+4, cdbm.hash[u].p);
      if(!out.write(packbuf, sizeof packbuf)) FAIL;
      safeadd(pos, 8);
    }
  }
  if(!out.rewind()) FAIL;
  if(!out.write(cdbm.final, sizeof(cdbm.final))) FAIL;
  if(!out.flush()) FAIL;
  out.close();
  if(rename(fntemp.c_str(), filename.c_str())) FAIL;
  return true;
}
