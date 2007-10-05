/* $Id$ */
#include "ghash.h"

/** Replace or add an entry into a generic hash table. */
void* ghash_set(struct ghash* d, const void* key, const void* data)
{
  void* e;
  if ((e = ghash_get(d, key)) != 0)
    return d->datacopy(ghash_entry_dataptr(e, d->keysize), data);
  return ghash_add(d, key, data);
}
