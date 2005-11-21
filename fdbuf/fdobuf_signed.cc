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
#include <limits.h>

#define MAXSTRLEN ((sizeof(signed long)*CHAR_BIT)/3)

fdobuf& fdobuf::operator<<(signed long i)
{
  if(i == 0)
    return operator<<('0');
  if(i < 0) {
    operator<<('-');
    i = -i;
  }
  char buf[MAXSTRLEN+1];
  char* ptr = buf+MAXSTRLEN;
  *ptr-- = 0;
  while(i) {
    *ptr-- = i % 10 + '0';
    i /= 10;
  }
  return operator<<(ptr+1);
}
