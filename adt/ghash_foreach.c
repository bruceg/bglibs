#include "ghash.h"

void ghash_foreach(struct ghash* d, void (*fn)(void* entry))
{
  void** p;
  unsigned long i;
  for (i = 0, p = d->table; i < d->size; ++i, ++p)
    if (*p != 0)
      fn(*p);
}
