#include "msg.h"

void msg_debug(int bit,
	       const char* a, const char* b, const char* c, const char* d,
	       const char* e, const char* f, int showsys)
{
  if (msg_debug_bits & bit)
    msg_common("Debug", a, b, c, d, e, f, showsys);
}
