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

#ifndef CDBXX__CDBXX__H__
#define CDBXX__CDBXX__H__

#include "mystring/mystring.h"
#include "cdb++/datum.h"
#include "fdbuf/fdbuf.h"

typedef unsigned uint32;

#define CDBMAKE_HPLIST 1000

class cdb_reader
{
private:
  unsigned char* map;
  size_t size;
  unsigned char* ptr;
  unsigned char* eod;
  bool failed;
  bool eof;

  void abort();
  char* seek(const mystring& key, uint32& len);
public:
  cdb_reader(const mystring& filename);
  ~cdb_reader();

  bool firstrec();
  datum* nextrec();
  datum* get(const mystring& key);
  bool operator!() const
    {
      return failed;
    }
  operator bool() const
    {
      return !failed;
    }
};

struct cdbmake {
  struct hp 
  {
    uint32 h;
    uint32 p;
  };
  
  struct hplist 
  {
    cdbmake::hp hp[CDBMAKE_HPLIST];
    hplist* next;
    int num;
  };
  
  unsigned char final[2048];
  uint32 count[256];
  uint32 start[256];
  hplist *head;
  hp *split;
  hp *hash;
  uint32 numentries;

  cdbmake();
  ~cdbmake();
  int add(uint32 h, uint32 p);
  int split_hashes();
  uint32 throw_hash(uint32 pos, int b);
};

class cdb_writer
{
private:
  fdobuf out;
  cdbmake cdbm;
  mystring fntemp;
  uint32 pos;
  bool failed;
  
  void abort();
public:
  cdb_writer(const mystring& filename, int mode = 0666);
  ~cdb_writer();
  bool put(const mystring& key, const mystring& data);
  bool end(const mystring& filename);
  bool operator!() const
    {
      return failed;
    }
  operator bool() const
    {
      return !failed;
    }
};

extern datum* cdb_getrec(const mystring& filename, const mystring& key);

#endif // CDBXX__H__
