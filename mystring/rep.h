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

#ifndef MYSTRING__REP__H__
#define MYSTRING__REP__H__

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

#endif
