#ifndef CRC64__H__
#define CRC64__H__

#include <uint64.h>

/* Polynomial: x^64 + x^4 + x^3 + x^1 + 1 */
#define CRC64REVPOLY ((uint64)0xd800000000000000ULL)
#define CRC64INIT    ((uint64)0xffffffffffffffffULL)

extern const uint64 crc64_table[256];
extern uint64 crc64_update(uint64 crc, const char* bytes, long len);

#endif
