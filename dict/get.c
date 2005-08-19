/* $Id$ */
#include "dict.h"

dict_entry* dict_get(dict* d, const str* s)
{
  uint32 hash;
  unsigned i;
  dict_entry* slot;
  if (!d->size || !d->table) return 0;
  hash = dict_hashstr(s);
  i = hash % d->size;
  while ((slot = d->table[i]) != 0) {
    if (hash == slot->hash)
      if (str_diff(s, &slot->key) == 0)
	return slot;
    if (++i >= d->size) i = 0;
  }
  return 0;
}
