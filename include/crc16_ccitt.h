#ifndef CRC16__CCITT__H__
#define CRC16__CCITT__H__

#include "gcrc.h"

#define CRC16_CCITT_POLY ((uint16)0x1021)
#define CRC16_CCITT_INIT ((uint16)0xffff)
#define CRC16_CCITT_POST ((uint16)0)

extern const uint16 crc16_ccitt_table[256];
#define crc16_ccitt_update(C,D,L) gcrc16fwd((C),(D),(L),crc16_ccitt_table)
#define crc16_ccitt_block(B,L) crc16_ccitt_update(CRC16_CCITT_INIT,(B),(L))

#endif
