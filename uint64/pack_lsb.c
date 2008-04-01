#include "uint64.h"
#include "uint32.h"

#ifndef __INLINE_UINT_LSB
void uint64_pack_lsb(uint64 u, unsigned char b[4])
{
#ifdef HAS_ULONG64
  b[0] = u & 0xff;
  u >>= 8;
  b[1] = u & 0xff;
  u >>= 8;
  b[2] = u & 0xff;
  u >>= 8;
  b[3] = u & 0xff;
  u >>= 8;
  b[4] = u & 0xff;
  u >>= 8;
  b[5] = u & 0xff;
  u >>= 8;
  b[6] = u & 0xff;
  u >>= 8;
  b[7] = u;
#else
  uint32 hi, lo;
  lo = u;
  b[0] = lo & 0xff;
  lo >>= 8;
  b[1] = lo & 0xff;
  lo >>= 8;
  b[2] = lo & 0xff;
  lo >>= 8;
  b[3] = lo;
  hi = u >> 32;
  b[4] = hi & 0xff;
  hi >>= 8;
  b[5] = hi & 0xff;
  hi >>= 8;
  b[6] = hi & 0xff;
  hi >>= 8;
  b[7] = hi;
#endif
}
#endif
