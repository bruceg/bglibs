#if !defined( _sha512a_h )
#define _sha512a_h

#include <sysdeps.h>

#define SHA512_DIGEST_LENGTH (512/8)

typedef struct {
      uint32 H[ 16 ];
      uint32 hbits, lbits;
      uint8 M[ 256 ];
      uint32 mlen;
} SHA512_ctx;

void SHA512_init  ( SHA512_ctx* );
void SHA512_update( SHA512_ctx*, const void*, unsigned long );
void SHA512_final ( SHA512_ctx* );
void SHA512_digest( SHA512_ctx*, uint8* );

#endif
