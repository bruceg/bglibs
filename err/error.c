/* $Id$ */
#include "err.h"

void error(const char* a, const char* b, const char* c, const char* d,
	   const char* e, const char* f, int showsys)
{
  err_message("Error", a, b, c, d, e, f, showsys);
}
