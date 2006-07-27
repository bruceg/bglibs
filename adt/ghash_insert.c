/* $Id$ */
#include "ghash.h"

/** Insert an entry into a \c ghash table, without resizing it. */
void ghash_insert(struct ghash* d, void* e)
{
  unsigned i;
  i = ghash_entry_hash(e) % d->size;
  while (d->table[i] != 0)
    i = (i + 1) % d->size;
  d->table[i] = e;
  ++d->count;
}
