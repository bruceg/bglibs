/* $Id$ */
#ifndef CRC16__XMODEM__H__
#define CRC16__XMODEM__H__

#include <uint16.h>

#define CRC16_XMODEM_POLY ((uint16)0x8408)
#define CRC16_XMODEM_INIT ((uint16)0x0000)
#define CRC16_XMODEM_POST ((uint16)0x0000)

extern const uint16 crc16_xmodem_table[256];
#define crc16_xmodem_update(C,D,L) gcrc16rfl((C),(D),(L),crc16_xmodem_table)
#define crc16_xmodem_block(B,L) crc16_xmodem_update(0,(B),(L))

#endif
