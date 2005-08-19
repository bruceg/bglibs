/* $Id$ */
#include "msg.h"

void msg_warnf(int showsys, const char* format, ...)
{
  va_list ap;
  va_start(ap, format);
  msg_commonfv("Warning", showsys, format, ap);
  va_end(ap);
}
