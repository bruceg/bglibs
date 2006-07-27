/* $Id$ */
#include "ghash.h"

/** Find an entry in a \c ghash table.
 *
 * \returns A pointer to the entry structure, or \c NULL if the \c key
 * was not found. */
void** ghash_find(struct ghash* d, const void* key)
{
  const adt_hash_t hash = d->hashfn(key);
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
	d->keycmp(key, ghash_entry_keyptr(entry)) == 0)
      return p;
    ++p;
    if (++i >= size) i = 0, p = d->table;
  } while (i != start);
  return 0;
}

/** Find an entry in a \c ghash table.
 *
 * \returns A pointer to the data structure or \c NULL if the \c key was
 * not found. */
void* ghash_get(struct ghash* d, const void* key)
{
  void** entry;
  if ((entry = ghash_find(d, key)) == 0)
    return 0;
  return *entry;
}
