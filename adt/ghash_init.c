#include "ghash.h"

void ghash_init(struct ghash* d,
		unsigned long keysize,
		unsigned long entrysize,
		adt_hash_fn* hashfn,
		adt_cmp_fn* keycmp,
		adt_copy_fn* keycopy,
		adt_copy_fn* datacopy,
		adt_free_fn* keyfree,
		adt_free_fn* datafree)
{
  d->table = 0;
  d->count = 0;
  d->size = 0;

  d->keysize = keysize;
  d->entrysize = entrysize;
  
  d->hashfn = hashfn;
  d->keycmp = keycmp;
  d->keycopy = keycopy;
  d->datacopy = datacopy;
  d->keyfree = keyfree;
  d->datafree = datafree;
}
