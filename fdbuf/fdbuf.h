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

#ifndef FDBUF__H__
#define FDBUF__H__

#include "config.h"
#include <string.h>
#include <fcntl.h>

#ifdef _REENTRANT
#include <pthread.h>
#endif

#ifndef FDBUF_SIZE
#define FDBUF_SIZE 4096
#endif

class mystring;

class fdbuf 
{
public:
  enum flagbits { flag_eof=1, flag_error=2, flag_closed=4 };

  fdbuf(int fdesc, bool dc, unsigned bufsz = FDBUF_SIZE);
  ~fdbuf();
  bool error() const;
  bool closed() const;
  bool close();
#ifdef _REENTRANT
  void lock() { pthread_mutex_lock(&mutex); }
  void unlock() { pthread_mutex_unlock(&mutex); }
#else
#ifdef FDBUF_MUTEX_DEBUG
  void lock();
  void unlock();
#else
  void lock() { }
  void unlock() { }
#endif
#endif
protected:
  char* const buf;
  unsigned buflength;		// Length of the data in the buffer
  unsigned bufstart;		// Start of the data in the buffer
  unsigned offset;		// Current file read/write offset
  int errnum;			// Saved error flag
  unsigned flags;		// Status flags

  const unsigned bufsize;	// Total buffer size
  const int fd;
  const bool do_close;		// True to close on destructor

#ifdef _REENTRANT
  pthread_mutex_t mutex;
#else
#ifdef FDBUF_MUTEX_DEBUG
  unsigned mutex;
#endif
#endif
};

class fdobuf;

class fdibuf : protected fdbuf
{
public:
  fdibuf(const char* filename, unsigned bufsz = FDBUF_SIZE);
  fdibuf(int fdesc, bool dc = false, unsigned bufsz = FDBUF_SIZE);
  virtual ~fdibuf();
  bool close() { lock(); bool r = fdbuf::close(); unlock(); return r; }
  bool eof() const;
  bool operator!() const ;
  operator bool() const { return !operator!(); }
  virtual bool get(char& ch);
  virtual bool getline(mystring& out, char terminator = '\n');
  virtual bool getnetstring(mystring& out);
  virtual bool read(char*, unsigned);
  bool read(unsigned char* b, unsigned l) { return read((char*)b, l); }
  bool read(signed char* b, unsigned l) { return read((char*)b, l); }
  unsigned last_count() { return count; }
  bool seek(unsigned o);
  bool seekfwd(unsigned o);
  bool rewind() { return seek(0); }
  unsigned tell() const { return offset-buflength+bufstart; }
protected:
  unsigned count;		// Number of bytes read by last operation
  bool refill();
};

class fdobuf : protected fdbuf
{
public:
  enum openflags { create=O_CREAT, 
		   excl=O_EXCL,
		   trunc=O_TRUNC,
		   append=O_APPEND };

  fdobuf(const char* filename, int, int mode = 0666,
	 unsigned bufsz = FDBUF_SIZE);
  fdobuf(int fdesc, bool dc=false, unsigned bufsz = FDBUF_SIZE);
  virtual ~fdobuf();
  bool close();
  bool operator!() const;
  operator bool() const
    {
      return !operator!();
    }
  bool flush();
  bool sync();
  virtual bool write(char);
  bool write(unsigned char c) { return write((char)c); }
  bool write(signed char c) { return write((char)c); }
  virtual bool write(const char*, unsigned);
  bool write(const unsigned char* b, unsigned l) { return write((char*)b, l); }
  bool write(const signed char* b, unsigned l) { return write((char*)b, l); }
  unsigned last_count() { return count; }
  bool seek(unsigned o);
  bool rewind() { return seek(0); }
  unsigned tell() const { return offset + bufpos; }

  bool chown(uid_t, gid_t) const;
  bool chmod(mode_t) const;
  
  fdobuf& operator<<(const char* str)
    {
      write(str, strlen(str));
      return *this;
    }
  fdobuf& operator<<(char ch)
    {
      write(ch);
      return *this;
    }
  fdobuf& operator<<(fdobuf& (*manip)(fdobuf&))
    {
      return manip(*this);
    }
  fdobuf& operator<<(unsigned long);
  fdobuf& operator<<(signed long);
  fdobuf& operator<<(unsigned i) { return operator<<((unsigned long)i); }
  fdobuf& operator<<(signed i) { return operator<<((signed long)i); }
  fdobuf& operator<<(unsigned short i) { return operator<<((unsigned long)i); }
  fdobuf& operator<<(signed short i) { return operator<<((signed long)i); }
protected:
  virtual bool nflush(bool withsync);

  unsigned bufpos;		// Current write position in the buffer
  unsigned count;		// Number of bytes written by last operation
};

fdobuf& endl(fdobuf& fd);

bool fdbuf_copy(fdibuf&, fdobuf&, bool noflush = false);

extern fdobuf fout;
extern fdobuf ferr;
extern fdibuf fin;

#endif // NULLMAILER__FDBUF__H__
