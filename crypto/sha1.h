/*
 SHA-1 in C

 By Steve Reid <steve@edmweb.com>, with small changes to make it
 fit by Bruce Guenter <bruceg@em.ca>

*/

#ifndef _SHA1_H
# define _SHA1_H

#include "uint32.h"

#define SHA1_DIGEST_LENGTH 20
#define SHA1_BLOCK_LENGTH 64

typedef struct {
  uint32 state[5];
  uint32 countlo;
  uint32 counthi;
  unsigned char buffer[SHA1_BLOCK_LENGTH];
} SHA1_CTX;

void SHA1Transform(uint32 state[5], const unsigned char buffer[SHA1_BLOCK_LENGTH]);
void SHA1Init(SHA1_CTX* context);
void SHA1Update(SHA1_CTX* context, const unsigned char* data, uint32 len);
void SHA1Final(SHA1_CTX* context, unsigned char digest[SHA1_DIGEST_LENGTH]);

# define SHA1_Transform SHA1Transform
# define SHA1_Init SHA1Init
# define SHA1_Update SHA1Update
# define SHA1_Final SHA1Final

#endif

