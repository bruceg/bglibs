#include "uint64.h"
#include "uint32.h"

uint64 uint64_get_lsb(const unsigned char b[4])
{
#ifdef HAS_ULONG64
  uint64 r;
  r = b[0];
  r <<= 8;
  r += b[1];
  r <<= 8;
  r += b[2];
  r <<= 8;
  r += b[3];
  r <<= 8;
  r += b[4];
  r <<= 8;
  r += b[5];
  r <<= 8;
  r += b[6];
  r <<= 8;
  r += b[7];
  return r;
#else
  /* 64-bit operations are painfully expensive on 32-bit systems */
  uint32 hi, lo;
  hi = b[0];
  hi <<= 8;
  hi += b[1];
  hi <<= 8;
  hi += b[2];
  hi <<= 8;
  hi += b[3];
  lo = b[4];
  lo <<= 8;
  lo += b[5];
  lo <<= 8;
  lo += b[6];
  lo <<= 8;
  lo += b[7];
  return (((uint64)hi) << 32) + lo;
#endif
}
