/* $Id$ */
#include <stdlib.h>
#include "msg.h"

void msg_dief(int code, int showsys, const char* format, ...)
{
  va_list ap;
  va_start(ap, format);
  msg_commonfv("Fatal", showsys, format, ap);
  exit(code);
  va_end(ap);
}
