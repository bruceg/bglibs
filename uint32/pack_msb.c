#include "uint32.h"

#ifndef __INLINE_UINT_MSB
void uint32_pack_msb(uint32 u, unsigned char b[4])
{
  b[3] = u & 0xff;
  u >>= 8;
  b[2] = u & 0xff;
  u >>= 8;
  b[1] = u & 0xff;
  u >>= 8;
  b[0] = u;
}
#endif
