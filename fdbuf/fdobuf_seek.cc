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

#include "fdbuf.h"
#include <errno.h>
#include <unistd.h>

bool fdobuf::seek(unsigned o)
{
  if(flags)
    return false;

  lock();
  unsigned buf_start = offset;
  unsigned buf_end = offset + buflength;
  if(o >= buf_start && o < buf_end) {
    bufpos = o - buf_start;
  }
  else {
    if(!nflush(false)) {
      unlock();
      return false;
    }
    if(lseek(fd, o, SEEK_SET) != (off_t)o) {
      errnum = errno;
      flags |= flag_error;
      unlock();
      return false;
    }
    offset = o;
  }
  count = 0;
  unlock();
  return true;
}
