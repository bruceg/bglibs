#include "err.h"

void warn(const char* a, const char* b, const char* c, const char* d,
	  int showsys)
{
  err_message("Warning", a, b, c, d, showsys);
}
