#include <stdio.h>
#include <time.h>
#include "sha256.h"

byte b;

#define BUF_SIZE 1024

int main( int argc, char* argv[] )
{
   SHA256_ctx ctx;
   byte buf[ BUF_SIZE ];
   int len;
   int i;
   long secondCount;
   byte digest[ 20 ];

/* test 1 data */
   const byte* test1 = "abc";

   word32 result1[ 8 ] = {
	   0xba7816bf, 0x8f01cfea, 0x414140de, 0x5dae2223,
	   0xb00361a3, 0x96177a9c, 0xb410ff61, 0xf20015ad
   };

/* test 2 date */
   const byte* test2 = 
      "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";

   word32 result2[ 8 ] = {
	   0x248d6a61, 0xd20638b8, 0xe5c02693, 0x0c3e6039,
	   0xa33ce459, 0x64ff2167, 0xf6ecedd4, 0x19db06c1
   };

/* test 1 */

   printf( "test 1\n\n" );
   SHA256_init( &ctx );
   SHA256_update( &ctx, test1, strlen( test1 ) );
   SHA256_final( &ctx );
   printf( "SHA256(\"%s\") = \n\t", test1 );

/* illustrating printing using output of SHA256_digest function */

   SHA256_digest( &ctx, digest );

   for ( i = 0; i < 20 ; i++ )
   {
      printf( "%02x", digest[ i ] );
   }

   if ( memcmp( ctx.H, result1, 32 ) == 0 )
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
   SHA256_init( &ctx );
   SHA256_update( &ctx, test2, strlen( test2 ) );
   SHA256_final( &ctx );

/* illustrating printing using ctx.H values directly */

   SHA256_digest( &ctx, digest );

   printf( "SHA256(\"%s\") = \n\t", test2 );

   for ( i = 0; i < 5; i++ )
   {
      printf( "%08x", ctx.H[ i ] );
   }

   if ( memcmp( ctx.H, result2, 32 ) == 0 )
   {
      printf( " test ok\n" );
   }
   else
   {
      printf( " test failed\n" );
   }
   printf( "\n" );

}
