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

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include "cdb++.h"
#include "internal.h"

#define FAIL do{ abort(); return 0; }while(0)

inline uint32 max(uint32 a, uint32 b)
{
  return a > b ? a : b;
}

cdb_reader::cdb_reader(const mystring& filename)
  : map(0),
    failed(1),
    eof(false)
{
  int fd = open(filename.c_str(), O_RDONLY);
  if(fd == -1) return;
  struct stat buf;
  if(fstat(fd, &buf) != -1)
    map = (unsigned char*)mmap(0, buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
  close(fd);
  failed = !map;
  firstrec();
}

cdb_reader::~cdb_reader()
{
}

void cdb_reader::abort()
{
  failed = true;
  if(map)
    munmap(map, size);
}

bool cdb_reader::firstrec()
{
  if(failed) return false;
  ptr = map + 2048;
  eod = map + unpack(map);
  return true;
}

datum* cdb_reader::nextrec()
{
  if(eof) return 0;
  if(ptr >= eod) {
    eof = true;
    return 0;
  }
  if(failed || eod-ptr < 8) FAIL;
  uint32 klen = unpack(ptr); ptr += 4;
  uint32 dlen = unpack(ptr); ptr += 4;
  if ((uint32)(eod - ptr) < klen + dlen) FAIL;
  datum* result = new datum(mystring((char*)ptr,      klen),
			    mystring((char*)ptr+klen, dlen));
  ptr += klen + dlen;
  return result;
}
