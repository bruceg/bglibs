#include "uint32.h"

#ifndef __INLINE_UINT_LSB
uint32 uint32_get_lsb(const unsigned char b[4])
{
  uint32 r;
  r = b[3];
  r <<= 8;
  r += b[2];
  r <<= 8;
  r += b[1];
  r <<= 8;
  r += b[0];
  return r;
}
#endif
