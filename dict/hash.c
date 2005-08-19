/* $Id$ */
#include "dict.h"

/* Public domain hash function written by D. J. Bernstein */

uint32 dict_hashadd(uint32 h, const unsigned char* data, unsigned long len)
{
  while (len--)
    h = (h + (h << 5)) ^ *data++;
  return h;
}
