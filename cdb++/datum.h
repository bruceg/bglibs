#ifndef DATUM__H__
#define DATUM__H__

#include "fdbuf.h"
#include "mystring.h"

struct datum 
{
  mystring key;
  mystring data;

  datum(const mystring& k, const mystring& d);
  ~datum();
};

fdobuf& operator<<(fdobuf&, const datum&);

#endif // DATUM__H__
