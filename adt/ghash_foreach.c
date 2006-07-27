/* $Id$ */
#include "ghash.h"

/** Iterate over a \c ghash table, calling a function for each entry. */
void ghash_foreach(struct ghash* d, void (*fn)(void* entry))
{
  void** p;
  unsigned long i;
  for (i = 0, p = d->table; i < d->size; ++i, ++p)
    if (*p != 0)
      fn(*p);
}
