#ifndef CHECKVPW__CGI_ARGS__H__
#define CHECKVPW__CGI_ARGS__H__

#include "mystring.h"

class CGIArgs 
{
private:
  struct arg
  {
    arg* next;
    const mystring var;
    const mystring val;
    arg(mystring r, mystring l)
      : next(0), var(r), val(l)
      {
      }
    ~arg()
      {
	delete next;
      }
  };
  arg* head;
  arg* tail;
  unsigned argc;
  mystring errstr;
protected:
  void init();
  void append(mystring, mystring);
public:
  CGIArgs() : head(0), tail(0), argc(0)
    {
      init();
    }
  ~CGIArgs();
  bool error() const 
    {
      return !errstr.empty();
    }
  mystring errorstr() const
    {
      return errstr;
    }
  bool exists(mystring) const;
  mystring get(mystring var, mystring dflt = 0) const;
  mystring operator[](mystring var) const
    {
      return get(var);
    }
};

#endif
