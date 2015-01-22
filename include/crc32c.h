#ifndef CRC32C__H__
#define CRC32C__H__

#include "gcrc.h"

#define CRC32CPOLY    ((uint32)0x1EDC6F41UL)
#define CRC32CINIT    ((uint32)0xFFFFFFFFUL)
#define CRC32CPOST    ((uint32)0xFFFFFFFFUL)

extern const uint32 crc32c_table[256];
#define crc32c_update(C,D,L) gcrc32rfl((C),(D),(L),crc32c_table)
#define crc32c_block(B,L) (crc32c_update(CRC32CINIT,(B),(L))^CRC32CPOST)

#endif
