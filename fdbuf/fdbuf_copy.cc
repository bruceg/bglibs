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

#include "fdbuf.h"

///////////////////////////////////////////////////////////////////////////////
// Other routines
///////////////////////////////////////////////////////////////////////////////
bool fdbuf_copy(fdibuf& in, fdobuf& out, bool noflush)
{
  if(in.eof())
    return true;
  if(!in || !out)
    return false;
  do {
    char buf[FDBUF_SIZE];
    if(!in.read(buf, FDBUF_SIZE) && in.last_count() == 0)
      break;
    if(!out.write(buf, in.last_count()) && out.last_count() < in.last_count())
      return false;
  } while(!in.eof());
  if(!noflush && !out.flush())
    return false;
  return in.eof();
}
