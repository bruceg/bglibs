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

#ifndef MYSTRING__H__
#define MYSTRING__H__

#include <sys/types.h>
#include "fdbuf/fdbuf.h"

struct mystringrep
{
  unsigned length;
  unsigned references;
  unsigned size;
  char buf[1];

  void attach();
  void detach();
  mystringrep* append(const char*, unsigned);
  
  static mystringrep* alloc(unsigned);
  static mystringrep* dup(const char*, unsigned);
  static mystringrep* dup(const char*, unsigned,
			  const char*, unsigned);
};

#ifndef MYSTRING_TRACE
inline void mystringrep::attach()
{
  references++;
}
#endif

extern mystringrep nil;

class mystringjoin;
class mystring
{
  friend class mystringtmp;
  friend class mystringjoin;
private:
  mystringrep* rep;

protected:
  void dupnil();
  void dup(const char*, size_t);
  void dup(const char*);
  void append(const char*);
  void append(const char*, size_t);
  void assign(const char*);
  void assign(const char*, size_t);
public:
  static const mystring NUL;
  
  mystring() { dupnil(); }
  mystring(const char* s) { dup(s); }
  mystring(const mystring& s) { dup(s.rep->buf, s.rep->length); }
  mystring(const char* str, size_t len) { dup(str, len); }
  mystring(const mystringjoin&);
  ~mystring();

  const char* c_str() const { return rep->buf; }

  bool operator!() const { return empty(); }
  
  char operator[](size_t i) const { return rep->buf[i]; }
  
  size_t length() const { return rep->length; }

  bool empty() const { return rep->length == 0; }
  
  int operator!=(const char* in) const;
  int operator!=(const mystring& in) const;
  bool operator==(const char* in) const
    {
      return !operator!=(in);
    }
  bool operator==(const mystring& in) const
    {
      return !operator!=(in);
    }

  void operator=(const char* in) { assign(in); }
  void operator=(const mystring& in) { assign(in.rep->buf, in.rep->length); }
  void operator=(const mystringjoin& in);

  mystring subst(char from, char to) const;
  
  mystring lower() const;
  mystring upper() const;

  int find_first(char, size_t = 0) const;
  int find_first(const mystring&, size_t = 0) const;
  int find_first(const char*, size_t = 0) const;
  int find_first(const char*, size_t, size_t) const;
  int find(char ch, size_t offset = 0) const { return find_first(ch, offset); }

  int find_last(char, size_t = (size_t)-1) const;
  int find_last(const mystring&, size_t = (size_t)-1) const;
  int find_last(const char*, size_t = 0) const;
  int find_last(const char*, size_t, size_t) const;

  mystring left(size_t) const;
  mystring right(size_t) const;
  mystring sub(size_t, size_t) const;

  mystring lstrip() const;
  mystring rstrip() const;
  mystring strip() const;

  void operator+=(const mystring& str) {append(str.rep->buf, str.rep->length);}
  void operator+=(const char* str) { append(str); }
  void operator+=(char ch)
    {
      char str[2] = { ch, 0 };
      append(str, 1);
    }
};

class mystringjoin
{
private:
  const mystringjoin* prev;
  mystringrep* rep;
  const char* str;

  mystringjoin();
public:
  mystringjoin(const mystringjoin& j)
    : prev(j.prev), rep(j.rep), str(j.str)
    {
      rep->attach();
    }
  mystringjoin(const mystring& s)
    : prev(0), rep(s.rep), str(s.rep->buf)
    {
      rep->attach();
    }
  mystringjoin(const char* s)
    : prev(0), rep(0), str(s)
    {
    }
  mystringjoin(const mystringjoin& p, const mystring& s)
    : prev(&p), rep(s.rep), str(s.rep->buf)
    {
      rep->attach();
    }
  mystringjoin(const mystringjoin& p, const char* s)
    : prev(&p), rep(0), str(s)
    {
    }
  ~mystringjoin()
    {
      if(rep) rep->detach();
    }
  mystringrep* traverse() const;
};

inline mystring::mystring(const mystringjoin& j)
  : rep(j.traverse())
{
  rep->attach();
}

#ifndef MYSTRING_TRACE
inline mystring::~mystring()
{
  rep->detach();
}
#endif

inline mystringjoin operator+(const mystringjoin& a, const mystring& b)
{
  return mystringjoin(a, b);
}

inline mystringjoin operator+(const mystringjoin& a, const char* b)
{
  return mystringjoin(a, b);
}

inline fdobuf& operator<<(fdobuf& out, const mystring& str)
{
  out.write(str.c_str(), str.length());
  return out;
}

//istream& operator>>(istream& in, mystring& str);

class mystring_iter
{
  const mystring str;
  const char sep;
  int pos;
  mystring part;

  void advance();
public:
  mystring_iter(const mystring&, char = '\0');
  ~mystring_iter();
  
  operator bool() const { return pos >= 0; }
  bool operator!() const { return pos < 0; }
  mystring operator*() const { return part; }
  void operator++() { advance(); }
};

typedef mystring string;

#endif
