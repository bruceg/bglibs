#include "crc16_arc.h"

uint16 crc16_arc_update(uint16 crc, const char* data, long len)
{
  const unsigned char* ptr = data;
  while (len-- > 0)
    crc = crc16_arc_table[(crc ^ *ptr++) & 0xff] ^ (crc >> 8);
  return crc;
}
