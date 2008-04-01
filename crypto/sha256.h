#ifndef BGLIBS__SHA256__H__
#define BGLIBS__SHA256__H__

#include <sysdeps.h>

#define SHA256_DIGEST_LENGTH (256/8)

typedef struct {
  uint32 H[8];
  uint64 bytes;
  uint8 M[64];
} SHA256_ctx;

void SHA256_init(SHA256_ctx*);
void SHA256_update(SHA256_ctx*, const void*, unsigned long);
void SHA256_final(SHA256_ctx*, uint8*);

#endif
