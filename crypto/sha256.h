#if !defined( _sha256_h )
#define _sha256_h

#define byte unsigned char 
#define word32 unsigned long

#define SHA256_DIGEST_LENGTH (256/8)

typedef struct {
      word32 H[ 8 ];
      word32 hbits, lbits;
      byte M[ 64 ];
      byte mlen;
} SHA256_ctx;

void SHA256_init  ( SHA256_ctx* );
void SHA256_update( SHA256_ctx*, const void*, word32 );
void SHA256_final ( SHA256_ctx* );
void SHA256_digest( SHA256_ctx*, byte* );

#endif
