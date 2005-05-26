#include "msg.h"

void msg_debugf(int bit, int showsys, const char* format, ...)
{
  if (msg_debug_bits & bit) {
    va_list ap;
    va_start(ap, format);
    msg_commonfv("Debug", showsys, format, ap);
    va_end(ap);
  }
}
