/* $Id$ */
#ifndef CRC64__H__
#define CRC64__H__

#include <crc/gcrc.h>

/* Polynomial: x^64 + x^62 + x^57 + x^55 + x^54 + x^53 + x^52 + x^47 +
 * x^46 + x^45 + x^40 + x^39 + x^38 + x^37 + x^35 + x^33 + x^32 + x^31 +
 * x^29 + x^27 + x^24 + x^23 + x^22 + x^21 + x^19 + x^17 + x^13 + x^12 +
 * x^10 + x^9 + x^7 + x^4 + x + 1
 * Borrowed from the DLT1 spec (ECMA-182).
 */
#define CRC64POLY    ((uint64)0x42F0E1EBA9EA3693ULL)
#define CRC64INIT    ((uint64)0xffffffffffffffffULL)
#define CRC64POST    ((uint64)0xffffffffffffffffULL)

extern const uint64 crc64_table[256];
#define crc64_update(C,B,L) gcrc64fwd((C),(B),(L),crc64_table)
#define crc64_block(B,L) (crc64_update(CRC64INIT,(B),(L))^CRC64POST)

#endif
