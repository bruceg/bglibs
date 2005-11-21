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

#include "mystring.h"

mystring_iter::mystring_iter(const mystring& s, char e)
  : str(s), sep(e), pos(0)
{
  advance();
}

mystring_iter::~mystring_iter()
{
}

void mystring_iter::advance()
{
  if(pos == -1)
    return;
  int i = str.find_first(sep, pos);
  if(i == -1) {
    if(pos >= 0 && pos < (int)str.length()) {
      part = str.right(pos);
      pos = str.length();
    }
    else {
      part = "";
      pos = -1;
    }
  }
  else {
    part = str.sub(pos, i-pos);
    pos = i + 1;
  }
}
