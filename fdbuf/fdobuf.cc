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

#include "fdbuf.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

///////////////////////////////////////////////////////////////////////////////
// Class fdobuf
///////////////////////////////////////////////////////////////////////////////
fdobuf::fdobuf(int fdesc, bool dc, unsigned bufsz)
  : fdbuf(fdesc, dc, bufsz),
    bufpos(0)
{
}

fdobuf::fdobuf(const char* filename, int f, int mode, unsigned bufsz)
  : fdbuf(open(filename, O_WRONLY | f, mode), true, bufsz),
    bufpos(0)
{
  if(fd == -1) {
    flags = flag_error;
    errnum = errno;
  }
}

fdobuf::~fdobuf()
{
  flush();
}

bool fdobuf::close()
{
  if(!flush())
    return false;
  lock();
  bool r = fdbuf::close();
  unlock();
  return r;
}

bool fdobuf::operator!() const
{
  return error() || closed();
}

bool fdobuf::nflush(bool withsync)
{
  if(flags)
    return false;
  while(bufstart < buflength) {
    ssize_t written = ::write(fd, buf+bufstart, buflength-bufstart);
    if(written == -1) {
      flags |= flag_error;
      errnum = errno;
      return false;
    }
    else {
      bufstart += written;
      offset += written;
    }
  }
  buflength = 0;
  bufstart = 0;
  bufpos = 0;
  if(withsync && (fsync(fd) == -1)) {
    flags |= flag_error;
    errnum = errno;
    return false;
  }
  return true;
}

bool fdobuf::flush()
{
  lock();
  bool r = nflush(false);
  unlock();
  return r;
}

bool fdobuf::sync()
{
  lock();
  bool r = nflush(true);
  unlock();
  return r;
}

bool fdobuf::write(char ch)
{
  if(flags)
    return false;

  lock();
  count = 0;
  buf[bufpos++] = ch;
  if(buflength >= bufsize && !nflush(false)) {
    unlock();
    return false;
  }
  if(bufpos >= buflength) buflength = bufpos;
  if(buflength >= bufsize && !nflush(false)) {
    unlock();
    return false;
  }
  count = 1;
  unlock();
  return true;
}

bool fdobuf::write(const char* data, unsigned datalen)
{
  if(flags)
    return false;

  lock();
  const char* ptr = data;
  count = 0;
  // Amount is the number of bytes available in the buffer
  unsigned amount = bufsize-bufpos;
  while(datalen >= amount) {
    // If we get here, this copy will completely fill the buffer,
    // requiring a flush
    memcpy(buf+bufpos, ptr, amount);
    bufpos = bufsize;
    buflength = bufsize;
    datalen -= amount;
    ptr += amount;
    if(!nflush(false)) {
      unlock();
      return false;
    }
    count += amount;
    amount = bufsize-bufpos;
  }
  // At this point, the remaining data will fit into the buffer
  memcpy(buf+bufpos, ptr, datalen);
  count += datalen;
  bufpos += datalen;
  if(bufpos > buflength) buflength = bufpos;
  unlock();
  return true;
}

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

fdobuf& fdobuf::operator<<(signed long i)
{
  if(i == 0)
    return operator<<('0');
  else {
    char buf[20];
    char* ptr = buf+19;
    *ptr-- = 0;
    bool negative = false;
    if(i < 0) {
      negative = true;
      i = -i;
    }
    while(i) {
      *ptr-- = i % 10 + '0';
      i /= 10;
    }
    if(negative)
      *ptr-- = '-';
    return operator<<(ptr+1);
  }
}

fdobuf& fdobuf::operator<<(unsigned long i)
{
  if(i == 0)
    return operator<<('0');
  else {
    char buf[20];
    char* ptr = buf+19;
    *ptr-- = 0;
    while(i) {
      *ptr-- = i % 10 + '0';
      i /= 10;
    }
    return operator<<(ptr+1);
  }
}

///////////////////////////////////////////////////////////////////////////////
// Manipulators
///////////////////////////////////////////////////////////////////////////////
fdobuf& endl(fdobuf& fd)
{
  fd.write("\n", 1);
  fd.flush();
  return fd;
}

///////////////////////////////////////////////////////////////////////////////
// Globals
///////////////////////////////////////////////////////////////////////////////
fdobuf fout(1);
fdobuf ferr(2);
