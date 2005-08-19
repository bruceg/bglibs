/* $Id$ */
#include <stdlib.h>
#include <string.h>

#include "ghash.h"

int ghash_rehash(struct ghash* d)
{
  unsigned i;
  void** entry;
  for (i = 0, entry = d->table; i < d->size; ++i, ++entry)
    if (*entry)
      ghash_entry_hash(*entry) = d->hashfn(ghash_entry_keyptr(*entry));
  return ghash_rebuild(d);
}
