#include "uint16.h"

void uint16_unpack_lsb(const unsigned char s[2], uint16* u)
{
  *u = (s[1] << 8) + s[0];
}

void uint16_unpack_msb(const unsigned char s[2], uint16* u)
{
  *u = (s[0] << 8) + s[1];
}
