#include "misc.h"

char* utoa2(unsigned long i, char* buf)
{
  if (i < 10)
    *buf = i + '0';
  else {
    buf = utoa2(i / 10, buf);
    *buf = (i % 10) + '0';
  }
  *++buf = 0;
  return buf;
}
