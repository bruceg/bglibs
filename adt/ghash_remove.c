/* $Id$ */
#include <stdlib.h>
#include <string.h>

#include "ghash.h"

/** Remove an entry from a \c ghash table. This function attempts to do
 * the minimum amount of rebuilding necessary to adjust the positions of
 * entries that may fall in the same slot as the newly removed entry.
 *
 * \returns True if the entry was found (and removed), false otherwise.
 */
int ghash_remove(struct ghash* d, const void* key)
{
  void* entry;
  void** slot;
  unsigned i;

  /* First, locate the key in the hash table. */
  if ((slot = ghash_find(d, key)) == 0)
    return 0;
  entry = *slot;

  /* Then free it, and empty out its slot. */
  if (d->keyfree != 0)
    d->keyfree(ghash_entry_keyptr(entry));
  if (d->datafree != 0)
    d->datafree(ghash_entry_dataptr(entry, d->keysize));
  free(entry);
  *slot = 0;
  --d->count;

  /* Finally, make sure all subsequent entries are properly inserted. */
  for (i = (slot - d->table + 1) % d->size;
       (entry = d->table[i]) != 0;
       i = (i + 1) % d->size) {
    --d->count;
    d->table[i] = 0;
    ghash_insert(d, entry);
  }
  return 1;
}
