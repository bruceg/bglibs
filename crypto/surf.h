#ifndef BGLIBS__SURF__H__
#define BGLIBS__SURF__H__

#include <uint32.h>

/** \defgroup crypto crypto: Cryptographic routines 
@{ */

/** The number of 32-bit words output by surf */
#define SURF_OUT_U32  8
/** The number of 32-bit words used as input by surf */
#define SURF_IN_U32   12
/** The number of 32-bit words used as seed data by surf */
#define SURF_SEED_U32 32

/** The number of bytes output by surf */
#define SURF_OUT      32
/** The number of bytes used as input by surf */
#define SURF_IN       48
/** The number of bytes used as seed data by surf */
#define SURF_SEED     128

void surf(uint32 out[8], const uint32 in[12], const uint32 seed[32]);

/** @} */

#endif
