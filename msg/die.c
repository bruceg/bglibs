/* $Id$ */
#include <stdlib.h>
#include "msg.h"

void msg_die(int code,
	     const char* a, const char* b, const char* c, const char* d,
	     const char* e, const char* f, int showsys)
{
  msg_common("Fatal", a, b, c, d, e, f, showsys);
  exit(code);
}
