#include "err.h"

void error(const char* a, const char* b, const char* c, const char* d,
	   int showsys)
{
  err_message("Error", a, b, c, d, showsys);
}
