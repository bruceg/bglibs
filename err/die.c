#include <stdlib.h>
#include "err.h"

void die(int code,
	 const char* a, const char* b, const char* c, const char* d,
	 const char* e, const char* f, int showsys)
{
  err_message("Fatal", a, b, c, d, e, f, showsys);
  exit(code);
}
