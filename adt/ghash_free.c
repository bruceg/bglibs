#include <stdlib.h>
#include <string.h>

#include "ghash.h"

void ghash_free(struct ghash* d,
		ghash_free_fn* keyfree, unsigned keysize,
		ghash_free_fn* datafree)
{
  unsigned long i;
  void** p;
  if (d->table != 0) {
    if (keyfree != 0) {
      for (i = 0, p = d->table; i < d->size; ++i, ++p)
	if (*p != 0) keyfree(ghash_entry_keyptr(*p));
    }
    if (datafree != 0) {
      for (i = 0, p = d->table; i < d->size; ++i, ++p)
	if (*p != 0) datafree(ghash_entry_dataptr(*p, keysize));
    }
    for (i = 0, p = d->table; i < d->size; ++i, ++p)
      if (*p != 0)
	free(*p);
    free(d->table);
  }
  memset(d, 0, sizeof *d);
}
