/* $Id$ */
#ifndef CRC32__H__
#define CRC32__H__

#include <crc/gcrc.h>
#include <uint32.h>

#define CRC32POLY    ((uint32)0x04C11DB7UL)
#define CRC32REVPOLY ((uint32)0xEDB88320UL)
#define CRC32INIT    ((uint32)0xFFFFFFFFUL)
#define CRC32POST    ((uint32)0xFFFFFFFFUL)

extern const uint32 crc32_table[256];
#define crc32_update(C,D,L) gcrc32rfl((C),(D),(L),crc32_table)
#define crc32_block(B,L) (crc32_update(CRC32INIT,(B),(L))^CRC32POST)

#endif
