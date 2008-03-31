#ifndef BGLIBS__SURFRAND__H__
#define BGLIBS__SURFRAND__H__

#include <crypto/surf.h>

struct surfrand
{
  unsigned used;
  uint32 generated[SURF_OUT_U32];
  uint32 seed[SURF_SEED_U32];
  uint32 counter[SURF_IN_U32];
};

void surfrand_init(struct surfrand* c, const uint32* data, unsigned words);
uint32 surfrand_uint32(struct surfrand* c);
double surfrand_double(struct surfrand* c);
void surfrand_fill(struct surfrand* c, unsigned char* buf, unsigned len);

#endif
