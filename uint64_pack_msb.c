#include "uint64.h"
#include "uint32.h"

#ifndef __INLINE_UINT_MSB
void uint64_pack_msb(uint64 u, unsigned char b[4])
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
  uint32 hi, lo;
  lo = u;
  b[7] = lo & 0xff;
  lo >>= 8;
  b[6] = lo & 0xff;
  lo >>= 8;
  b[5] = lo & 0xff;
  lo >>= 8;
  b[4] = lo;
  hi = u >> 32;
  b[3] = hi & 0xff;
  hi >>= 8;
  b[2] = hi & 0xff;
  hi >>= 8;
  b[1] = hi & 0xff;
  hi >>= 8;
  b[0] = hi;
#endif
}
#endif
