#if !defined( _sha256_h )
#define _sha256_h

#include <sysdeps.h>

#define SHA256_DIGEST_LENGTH (256/8)

typedef struct {
      uint32 H[ 8 ];
      uint32 hbits, lbits;
      uint8 M[ 64 ];
      uint8 mlen;
} SHA256_ctx;

void SHA256_init  ( SHA256_ctx* );
void SHA256_update( SHA256_ctx*, const void*, unsigned long );
void SHA256_final ( SHA256_ctx* );
void SHA256_digest( SHA256_ctx*, uint8* );

#endif
