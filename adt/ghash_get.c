#include "ghash.h"

void* ghash_get(struct ghash* d, const void* key, unsigned long hash,
		ghash_cmp_fn* keycmp)
{
  const unsigned size = d->size;
  const unsigned start = hash % size;
  unsigned i = start;
  void** p = d->table + start;

  do {
    void* entry = *p;
    if (entry == 0) return 0;
    if (ghash_entry_hash(entry) == hash &&
	keycmp(key, ghash_entry_keyptr(entry)) == 0)
      return entry;
    if (++i >= size) i = 0, p = d->table;
  } while (i != start);
  return 0;
}
