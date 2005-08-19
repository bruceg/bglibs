/* $Id$ */
#include <stdlib.h>

#include "msg.h"

void msg_debug_init(void)
{
  const char* env;
  if ((env = getenv("MSG_DEBUG")) != 0)
    msg_debug_bits = strtoul(env, 0, 10);
}
