#include "uint32.h"

uint32 uint32_get_msb(const unsigned char b[4])
{
  uint32 r;
  r = b[0];
  r <<= 8;
  r += b[1];
  r <<= 8;
  r += b[2];
  r <<= 8;
  r += b[3];
  return r;
}
