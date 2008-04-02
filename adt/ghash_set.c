#include <string.h>
#include "ghash.h"

/** Replace or add an entry into a generic hash table. */
void* ghash_set(struct ghash* d, const void* key, const void* data)
{
  void* e;
  void* dataptr;

  if ((e = ghash_get(d, key)) == 0)
    return ghash_add(d, key, data);

  dataptr = ghash_entry_dataptr(e, d->keysize);
  if (d->datacopy == 0)
    memcpy(dataptr, data, d->entrysize - d->keysize - sizeof(adt_hash_t));
  else if (!d->datacopy(dataptr, data))
    return 0;
  return e;
}
