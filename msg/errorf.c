#include "msg.h"

void msg_errorf(int showsys, const char* format, ...)
{
  va_list ap;
  va_start(ap, format);
  msg_commonfv("Error", showsys, format, ap);
  va_end(ap);
}
