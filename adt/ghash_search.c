#include "ghash.h"

/** Search for the first entry in the \c ghash table for which the given
 * function returns true. */
void* ghash_search(struct ghash* d, int (*fn)(const void* entry))
{
  void** p;
  unsigned long i;
  for (i = 0, p = d->table; i < d->size; ++i, ++p)
    if (*p != 0)
      if (fn(*p))
	return *p;
  return 0;
}
