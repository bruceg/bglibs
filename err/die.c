#include <stdlib.h>
#include "err.h"

void die(int code,
	 const char* a, const char* b, const char* c, const char* d,
	 int showsys)
{
  err_message("Fatal", a, b, c, d, showsys);
  exit(code);
}
