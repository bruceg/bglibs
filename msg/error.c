#include "msg.h"

void msg_error(const char* a, const char* b, const char* c, const char* d,
	       const char* e, const char* f, int showsys)
{
  msg_common("Error", a, b, c, d, e, f, showsys);
}
