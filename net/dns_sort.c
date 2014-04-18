#include "dns.h"

static int mxcmp(const void* aptr, const void* bptr)
{
  const struct dns_mx* a = aptr;
  const struct dns_mx* b = bptr;
  return (int)a->distance - (int)b->distance;
}

void dns_sort_mx(struct dns_mx* mxs, int count)
{
  qsort(mxs, count, sizeof *mxs, mxcmp);
}
