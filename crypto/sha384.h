#ifndef BGLIBS__SHA384__H__
#define BGLIBS__SHA384__H__

#include <sysdeps.h>
#include <crypto/sha512.h>

#define SHA384_DIGEST_LENGTH (384/8)

typedef struct SHA512_ctx SHA384_ctx;

void SHA384_init  (SHA384_ctx*);
#define SHA384_update SHA512_update
void SHA384_final (SHA384_ctx*, uint8*);

#endif
