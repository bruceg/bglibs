/* $Id$ */
#include "uint64.h"
#include "uint32.h"

#ifndef __INLINE_UINT_LSB
uint64 uint64_get_lsb(const unsigned char b[4])
{
#ifdef HAS_ULONG64
  uint64 r;
  r = b[7];
  r <<= 8;
  r += b[6];
  r <<= 8;
  r += b[5];
  r <<= 8;
  r += b[4];
  r <<= 8;
  r += b[3];
  r <<= 8;
  r += b[2];
  r <<= 8;
  r += b[1];
  r <<= 8;
  r += b[0];
  return r;
#else
  /* 64-bit operations are painfully expensive on 32-bit systems */
  uint32 hi, lo;
  hi = b[7];
  hi <<= 8;
  hi += b[6];
  hi <<= 8;
  hi += b[5];
  hi <<= 8;
  hi += b[4];
  lo = b[3];
  lo <<= 8;
  lo += b[2];
  lo <<= 8;
  lo += b[1];
  lo <<= 8;
  lo += b[0];
  return (((uint64)hi) << 32) + lo;
#endif
}
#endif
