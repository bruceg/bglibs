#include "uint32.h"

void uint32_unpack_lsb(const unsigned char b[4], uint32* u)
{
  uint32 r;
  r = b[3];
  r <<= 8;
  r += b[2];
  r <<= 8;
  r += b[1];
  r <<= 8;
  r += b[0];
  *u = r;
}
