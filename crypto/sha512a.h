#if !defined( _sha512a_h )
#define _sha512a_h

#define byte unsigned char 

#define word32 unsigned long

#define SHA512_DIGEST_LENGTH (512/8)

typedef struct {
      word32 H[ 16 ];
      word32 hbits, lbits;
      byte M[ 256 ];
      word32 mlen;
} SHA512_ctx;

void SHA512_init  ( SHA512_ctx* );
void SHA512_update( SHA512_ctx*, const void*, word32 );
void SHA512_final ( SHA512_ctx* );
void SHA512_digest( SHA512_ctx*, byte* );

#endif
