#ifdef SELFTEST_MAIN
#include <stdio.h>
#include <string.h>
#include "sha512a.h"

static void test(const char* str, long len)
{
   SHA512_ctx ctx;
   unsigned i;
   unsigned char digest[ SHA512_DIGEST_LENGTH ];
   SHA512_init( &ctx );
   SHA512_update( &ctx, str, len );
   SHA512_final( &ctx );
   SHA512_digest( &ctx, digest );
   for ( i = 0; i < sizeof(digest) ; i++ )
      printf( "%02x", digest[ i ] );
   printf( "\n" );
   for ( i = 0; i < 16; i++ )
      printf( "%08lx", ctx.H[ i ] );
   printf( "\n" );
}

int main( void )
{
   const byte* test1 = "abc";
   const byte* test2 = 
           "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu";
   test( test1, strlen( test1 ) );
   test( test2, strlen( test2 ) );
   return 0;
}
#endif
#ifdef SELFTEST_EXP
ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f
ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f
8e959b75dae313da8cf4f72814fc143f8f7779c6eb9f7fa17299aeadb6889018501d289e4900f7e4331b99dec4b5433ac7d329eeb6dd26545e96e55b874be909
8e959b75dae313da8cf4f72814fc143f8f7779c6eb9f7fa17299aeadb6889018501d289e4900f7e4331b99dec4b5433ac7d329eeb6dd26545e96e55b874be909
#endif
