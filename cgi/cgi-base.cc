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

#include <config.h>
#include <stdlib.h>
#include "ac/time.h"
#include <unistd.h>
#include "cgi-base.h"
#include "fdbuf/fdbuf.h"

#ifndef HAVE_SRANDOM
void srandom(unsigned int seed);
#endif

static void redirect(mystring url)
{
  fout << "Location: " << url << "\r\n\r\n";
}

void content_type(mystring ct)
{
  fout << "Content-Type: " << ct << "\r\n\r\n";
}

static void error_message(mystring msg)
{
  content_type("text/html");
  fout << "<html>"
    "<head>\n"
    "<title>CGI Error</title></head>\n"
    "<body>\n"
    "<p align=center><h1>CGI Error</h1></p><hr>\n"
    "<p>The following error occurred while trying to process your request:\r\n"
    "<pre>\r\n" << msg << "\r\n</pre></p>\r\n";
  const mystring referer = getenv("HTTP_REFERER");
  if(!referer.empty())
    fout << "<p><a href=\"" << referer << "\">Back</a></p>\n";
  fout << "</body>\n";
}

static void response_message(mystring msg)
{
  content_type("text/html");
  fout << "<html>"
    "<head>\n"
    "<title>CGI Response</title></head>\n"
    "<body>\n"
    "<p align=center><h1>CGI Response</h1></p><hr>\n"
    "<p>" << msg << "</p>\n";
  const mystring referer = getenv("HTTP_REFERER");
  if(!referer.empty())
    fout << "<p><a href=\"" << referer << "\">Back</a></p>\n";
  fout << "</body>\n";
}

CGI::CGI(const CGIArgs& a)
  : args(a),
    redirect_url(a["redirect"]),
    error_redirect_url(a["error_redirect"]),
    vdomain(a["vdomain"]),
    password(a["password"])
{
  must_be_defined("vdomain");
  must_be_defined("password");
}

void CGI::must_be_defined(mystring var)
{
  if(!args.exists(var))
    error("The input field '" + var + "' must be defined");
}

void CGI::success(mystring msg)
{
  if(!redirect_url) response_message(msg);
  else redirect(redirect_url);
  exit(0);
}

void CGI::error(mystring msg)
{
  if(!error_redirect_url) error_message(msg);
  else redirect(error_redirect_url);
  exit(1);
}

int main(void)
{
  struct timeval tv;
  gettimeofday(&tv, 0);
  srandom(tv.tv_usec ^ tv.tv_sec);
  CGIArgs cgiargs;
  if(cgiargs.error())
    error_message(cgiargs.errorstr());
  else
    CGI(cgiargs).main();
}
