#ifndef BGLIBS__SURFRAND__H__
#define BGLIBS__SURFRAND__H__

#include <crypto/surf.h>

/** \defgroup surfrand surfrand: Random number generator based on SURF
@{ */

/** The surfrand state structure */
struct surfrand
{
  /** counts the number of bytes in \c generated that have been output
   * already */
  unsigned left;
  /** temporary storage for the generated random data */
  uint32 generated[SURF_OUT_U32];
  /** the input seed data */
  uint32 seed[SURF_SEED_U32];
  /** a giant counter used as input to SURF */
  uint32 counter[SURF_IN_U32];
};

void surfrand_init(struct surfrand* c, const uint32* data, unsigned words);
uint32 surfrand_uint32(struct surfrand* c);
double surfrand_double(struct surfrand* c);
void surfrand_fill(struct surfrand* c, unsigned char* buf, unsigned len);
uint32 surfrand_uniform(struct surfrand* c, uint32 bound);

/** @} */

#endif
