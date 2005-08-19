/* $Id$ */
#include "uint16.h"

#ifndef __INLINE_UINT_LSB
void uint16_unpack_lsb(const unsigned char s[2], uint16* u)
{
  *u = (s[1] << 8) + s[0];
}
#endif

#ifndef __INLINE_UINT_MSB
void uint16_unpack_msb(const unsigned char s[2], uint16* u)
{
  *u = (s[0] << 8) + s[1];
}
#endif
