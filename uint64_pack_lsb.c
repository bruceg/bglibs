#include "uint64.h"
#include "uint32.h"

void uint64_pack_lsb(uint64 u, unsigned char b[4])
{
#ifdef HAS_ULONG64
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
#else
  uint32 half;
  half = u;
  b[7] = half & 0xff;
  half >>= 8;
  b[6] = half & 0xff;
  half >>= 8;
  b[5] = half & 0xff;
  half >>= 8;
  b[4] = half;
  half = u >> 32;
  b[3] = half & 0xff;
  half >>= 8;
  b[2] = half & 0xff;
  half >>= 8;
  b[1] = half & 0xff;
  half >>= 8;
  b[0] = half;
#endif
}
