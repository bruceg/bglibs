#include <stdlib.h>
#include <string.h>

#include "ghash.h"

int ghash_rehash(struct ghash* d, adt_hash_fn* hash)
{
  unsigned i;
  void** entry;
  for (i = 0, entry = d->table; i < d->size; ++i, ++entry)
    if (*entry)
      ghash_entry_hash(*entry) = hash(ghash_entry_keyptr(*entry));
  return ghash_rebuild(d);
}
