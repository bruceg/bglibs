#include "crc16_ccitt.h"

uint16 crc16_ccitt_update(uint16 crc, const char* data, long len)
{
  const unsigned char* ptr = data;
  while (len-- > 0)
    crc = crc16_ccitt_table[(unsigned char)(crc>>8) ^ *ptr++] ^ (crc << 8);
  return crc;
}
