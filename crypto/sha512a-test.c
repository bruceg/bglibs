#include <stdio.h>
#include <time.h>
#include "sha512a.h"

byte b;

#define BUF_SIZE 1024

int main( int argc, char* argv[] )
{
   SHA512_ctx ctx;
   byte buf[ BUF_SIZE ];
   int len;
   int i;
   long secondCount;
   byte digest[ 64 ];

/* test 1 data */
   const byte* test1 = "abc";

   word32 result1[ 16 ] = {
           0xddaf35a1, 0x93617aba, 0xcc417349, 0xae204131,
           0x12e6fa4e, 0x89a97ea2, 0x0a9eeee6, 0x4b55d39a,
           0x2192992a, 0x274fc1a8, 0x36ba3c23, 0xa3feebbd,
           0x454d4423, 0x643ce80e, 0x2a9ac94f, 0xa54ca49f,
   };

/* test 2 data */
   const byte* test2 = 
           "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu";

   word32 result2[ 16 ] = {
           0x8e959b75, 0xdae313da, 0x8cf4f728, 0x14fc143f,
           0x8f7779c6, 0xeb9f7fa1, 0x7299aead, 0xb6889018,
           0x501d289e, 0x4900f7e4, 0x331b99de, 0xc4b5433a,
           0xc7d329ee, 0xb6dd2654, 0x5e96e55b, 0x874be909
   };

/* test 1 */

   printf( "test 1\n\n" );
   SHA512_init( &ctx );
   SHA512_update( &ctx, test1, strlen( test1 ) );
   SHA512_final( &ctx );
   printf( "SHA512(\"%s\") = \n\t", test1 );

/* illustrating printing using output of SHA512_digest function */

   SHA512_digest( &ctx, digest );

   for ( i = 0; i < sizeof(digest) ; i++ )
   {
      printf( "%02x", digest[ i ] );
   }

   if ( memcmp( ctx.H, result1, sizeof(digest) ) == 0 )
   {
      printf( " test ok\n" );
   }
   else
   {
      printf( " test failed\n" );
   }
   printf( "\n" );

/* test 2 */

   printf( "test 2\n\n" );
   SHA512_init( &ctx );
   SHA512_update( &ctx, test2, strlen( test2 ) );
   SHA512_final( &ctx );

/* illustrating printing using ctx.H values directly */

   SHA512_digest( &ctx, digest );

   printf( "SHA512(\"%s\") = \n\t", test2 );

   for ( i = 0; i < 16; i++ )
   {
      printf( "%08x", ctx.H[ i ] );
   }

   if ( memcmp( ctx.H, result2, 64 ) == 0 )
   {
      printf( " test ok\n" );
   }
   else
   {
      printf( " test failed\n" );
   }
   printf( "\n" );

}
