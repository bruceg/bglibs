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

#ifndef MYSTRING__JOIN__H__
#define MYSTRING__JOIN__H__

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

inline mystringjoin operator+(const mystringjoin& a, const mystring& b)
{
  return mystringjoin(a, b);
}

inline mystringjoin operator+(const mystringjoin& a, const char* b)
{
  return mystringjoin(a, b);
}

#endif
