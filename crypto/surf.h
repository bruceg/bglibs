#ifndef BGLIBS__SURF__H__
#define BGLIBS__SURF__H__

#include <uint32.h>

#define SURF_OUT_U32  8
#define SURF_IN_U32   12
#define SURF_SEED_U32 32

#define SURF_OUT      32
#define SURF_IN       48
#define SURF_SEED     128

void surf(uint32 out[8], const uint32 in[12], const uint32 seed[32]);
void surfc(unsigned char out[32],
           const unsigned char in[48], const unsigned char seed[128]);

#endif
