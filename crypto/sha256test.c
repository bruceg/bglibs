#ifdef SELFTEST_MAIN
#include <stdio.h>
#include <string.h>
#include "sha256.h"

static void test(const char* str, long len)
{
   SHA256_ctx ctx;
   unsigned i;
   unsigned char digest[ SHA256_DIGEST_LENGTH ];
   SHA256_init( &ctx );
   SHA256_update( &ctx, str, len );
   SHA256_final( &ctx );
   SHA256_digest( &ctx, digest );
   for ( i = 0; i < sizeof(digest) ; i++ )
      printf( "%02x", digest[ i ] );
   printf( "\n" );
   for ( i = 0; i < 8; i++ )
      printf( "%08lx", ctx.H[ i ] );
   printf( "\n" );
}

int main( void )
{
   const char test1[] = "abc";
   const char test2[] = 
      "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
   test( test1, strlen( test1 ) );
   test( test2, strlen( test2 ) );
   return 0;
}
#endif
#ifdef SELFTEST_EXP
ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad
ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad
248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1
248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1
#endif
