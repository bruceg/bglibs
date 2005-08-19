/* $Id$ */
#include "msg.h"

void msg_warn(const char* a, const char* b, const char* c, const char* d,
	      const char* e, const char* f, int showsys)
{
  msg_common("Warning", a, b, c, d, e, f, showsys);
}
