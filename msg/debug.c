#include <stdlib.h>

#include "msg.h"

void msg_debug(int bit,
	       const char* a, const char* b, const char* c, const char* d,
	       const char* e, const char* f, int showsys)
{
  if (msg_debug_bits & bit)
    msg_common("Debug", a, b, c, d, e, f, showsys);
}

void msg_debug_init(void)
{
  const char* env;
  if ((env = getenv("MSG_DEBUG")) != 0)
    msg_debug_bits = strtoul(env, 0, 10);
}
