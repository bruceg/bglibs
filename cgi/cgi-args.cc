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

#include <config.h>
#include <stdlib.h>
#include <sys/types.h>
#include "mystring.h"
#include "cgi-args.h"
#include "fdbuf.h"

// These routines are optimized for the common case:
// - few variables (less than ten)
// - short strings (less than 1K each)
// - infrequent access relative to the remainder of the code

CGIArgs::~CGIArgs()
{
  if(head)
    delete head;
}

bool CGIArgs::exists(mystring var) const
{
  for(arg* ptr = head; ptr; ptr = ptr->next)
    if(ptr->var == var)
      return true;
  return false;
}

mystring CGIArgs::get(mystring var, mystring dflt) const
{
  for(arg* ptr = head; ptr; ptr = ptr->next)
    if(ptr->var == var)
      return ptr->val;
  return dflt;
}

void CGIArgs::append(mystring var, mystring val)
{
  arg* newarg = new arg(var, val);
  if(tail)
    tail->next = newarg;
  else
    head = newarg;
  tail = newarg;
  ++argc;
}

static inline int fromhex(char ch)
{
  if(ch >= '0' && ch <= '9')
    return ch - '0';
  else if(ch >= 'a' && ch <= 'f')
    return ch - 'a' + 0xa;
  else if(ch >= 'A' && ch <= 'F')
    return ch - 'A' + 0xa;
  else
    return -1;
}

static bool get_parts(mystring& var, mystring& val)
{
  if(!fin) return false;
#define BUFSIZE 1024
  char buf[BUFSIZE];
  int i = 0;
  char ch;
  bool havevar = false;
  while(fin.get(ch)) {
    switch(ch) {
    case '+':
      buf[i++] = ' ';
      break;
    case '%':
      char ch1, ch2;
      if(fin.get(ch1) && fin.get(ch2))
	buf[i++] = fromhex(ch1) << 4 | fromhex(ch2);
      break;
    case '=':
      if(havevar)
	buf[i++] = ch;
      else {
	buf[i] = 0;
	var += buf;
	i = 0;
	havevar = true;
      }
      break;
    case '&':
      buf[i] = 0;
      if(havevar)
	val += buf;
      else
	var += buf;
      return true;
    default:
      buf[i++] = ch;
    }
    if(i >= BUFSIZE-1) {
      buf[i] = 0;
      if(!havevar) var += buf;
      else val += buf;
      i = 0;
    }
  }
  if(havevar) {
    buf[i] = 0;
    val += buf;
  }
  return havevar;
}

void CGIArgs::init()
{
  mystring method = getenv("REQUEST_METHOD");
  mystring content = getenv("CONTENT_TYPE");
  if(method != "POST" && method != "post")
    errstr = "Must be accessed via a CGI 'POST' form";
  else if(content == "multipart/form-data")
    errstr = "Must be accessed via a CGI 'POST' form";
  else {
    for(;;) {
      mystring var;
      mystring val;
      if(!get_parts(var, val))
	break;
      append(var, val);
    }
  }
}
