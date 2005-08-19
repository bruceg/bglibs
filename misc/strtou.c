/* $Id$ */
#include "misc.h"

unsigned long strtou(const char* str, const char** end)
{
  unsigned long u;
  unsigned digit;
  for (u = 0; (digit = *str - '0') <= 9; ++str)
    u = u * 10 + digit;
  *end = str;
  return u;
}
