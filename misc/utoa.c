/* $Id$ */
#include "misc.h"

static char utoa_buf[32];

const char* utoa(unsigned long i)
{
  char* ptr = utoa_buf + sizeof utoa_buf - 1;
  *ptr-- = 0;
  while (i >= 10) {
    *ptr-- = (i % 10) + '0';
    i /= 10;
  }
  *ptr = i + '0';
  return ptr;
}
