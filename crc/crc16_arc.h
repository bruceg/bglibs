#ifndef CRC16__ARC__H__
#define CRC16__ARC__H__

#include <uint16.h>

#define CRC16_ARC_POLY ((uint16)0x8005)
#define CRC16_ARC_INIT ((uint16)0)
#define CRC16_ARC_POST ((uint16)0)

extern const uint16 crc16_arc_table[256];
extern uint16 crc16_arc_update(uint16 crc, const char* data, long len);
#define crc16_arc_block(B,L) crc16_arc_update(0,(B),(L))

#endif
