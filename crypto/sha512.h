/* $Id$ */
#ifndef BGLIBS__SHA512__H__
#define BGLIBS__SHA512__H__

#include <sysdeps.h>

#define SHA512_DIGEST_LENGTH (512/8)

struct SHA512_ctx {
  uint64 H[8];
  uint64 bytes;
  uint8 M[256];
};
typedef struct SHA512_ctx SHA512_ctx;

void SHA512_init  (SHA512_ctx*);
void SHA512_update(SHA512_ctx*, const void*, unsigned long);
void SHA512_final (SHA512_ctx*, uint8*);

#endif
