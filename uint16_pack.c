#include "uint16.h"

void uint16_pack_lsb(uint16 u, unsigned char s[2])
{
  s[0] = u & 255;
  s[1] = u >> 8;
}

void uint16_pack_msb(uint16 u, unsigned char s[2])
{
  s[1] = u & 255;
  s[0] = u >> 8;
}
