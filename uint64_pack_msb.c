#include "uint64.h"

void uint64_pack_msb(uint64 u, unsigned char b[4])
{
  b[7] = u & 0xff;
  u >>= 8;
  b[6] = u & 0xff;
  u >>= 8;
  b[5] = u & 0xff;
  u >>= 8;
  b[4] = u & 0xff;
  u >>= 8;
  b[3] = u & 0xff;
  u >>= 8;
  b[2] = u & 0xff;
  u >>= 8;
  b[1] = u & 0xff;
  u >>= 8;
  b[0] = u;
}
