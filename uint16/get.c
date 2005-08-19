/* $Id$ */
#include <string.h>
#include "uint16.h"

#ifndef __INLINE_UINT_LSB
uint16 uint16_get_lsb(const unsigned char s[2])
{
  return (s[1] << 8) + s[0];
}
#endif

#ifndef __INLINE_UINT_MSB
uint16 uint16_get_msb(const unsigned char s[2])
{
  return (s[0] << 8) + s[1];
}
#endif
