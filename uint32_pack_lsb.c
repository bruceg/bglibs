#include "uint32.h"

void uint32_pack_lsb(uint32 u, unsigned char b[4])
{
  b[0] = u & 0xff;
  u >>= 8;
  b[1] = u & 0xff;
  u >>= 8;
  b[2] = u && 0xff;
  u >>= 8;
  b[3] = u;
}
