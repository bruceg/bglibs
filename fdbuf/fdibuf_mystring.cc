// Copyright (C) 1999,2000,2005 Bruce Guenter <bruce@untroubled.org>
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

#include <string.h>
#include "fdbuf.h"
#include "mystring/mystring.h"

bool fdibuf::getline(mystring& out, char terminator)
{
  lock();
  count = 0;
  if(bufstart >= buflength)
    refill();
  if(eof() || error()) {
    unlock();
    return false;
  }
  out = "";
  while(!eof() && !error()) {
    char* ptr = buf+bufstart;
    unsigned bufleft = buflength - bufstart;
    const char* end = (const char*)memchr(ptr, terminator, bufleft);
    if(!end) {
      out += mystring(ptr, bufleft);
      bufstart = buflength;
      count += bufleft;
      refill();
    }
    else {
      unsigned copylen = end - ptr;
      out += mystring(ptr, copylen);
      bufstart += copylen+1;
      count += copylen+1;
      break;
    }
  }
  unlock();
  return true;
}
