#include <unistd.h>
#include "dns.h"
#include "uint32.h"
#include "surfrand.h"

static struct surfrand state;
static int inited = 0;

/** Initialize the DNS library random state. */
void dns_random_init(const char data[SURF_SEED])
{
  struct timeval tv;
  uint32 altdata[SURF_SEED_U32];

  surfrand_init(&state, data ? (const uint32*)data : altdata, SURF_SEED_U32);

  gettimeofday(&tv,0);
  state.counter[8] = tv.tv_sec;
  state.counter[9] = tv.tv_usec;

  state.counter[10] = getpid();
  state.counter[11] = getppid();

  inited = 1;
}

/** Generate a random number less than \c n. */
unsigned int dns_random(unsigned int n)
{
  return surfrand_uniform(&state, n);
}
