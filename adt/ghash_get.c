#include "ghash.h"

void* ghash_get(struct ghash* d, const void* key, unsigned long hash,
		adt_cmp_fn* keycmp)
{
  const unsigned size = d->size;
  unsigned start;
  unsigned i;
  void** p;
  
  if (size == 0)
    return 0;
  start = hash % size;
  i = start;
  p = d->table + start;

  do {
    void* entry = *p;
    if (entry == 0) return 0;
    if (ghash_entry_hash(entry) == hash &&
	keycmp(key, ghash_entry_keyptr(entry)) == 0)
      return entry;
    ++p;
    if (++i >= size) i = 0, p = d->table;
  } while (i != start);
  return 0;
}
