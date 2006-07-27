/* $Id$ */
#include <stdlib.h>
#include <string.h>

#include "ghash.h"

/** Free all data (and entries) in a \c ghash table. */
void ghash_free(struct ghash* d)
{
  unsigned long i;
  void** p;
  if (d->table != 0) {
    if (d->keyfree != 0) {
      for (i = 0, p = d->table; i < d->size; ++i, ++p)
	if (*p != 0)
	  d->keyfree(ghash_entry_keyptr(*p));
    }
    if (d->datafree != 0) {
      for (i = 0, p = d->table; i < d->size; ++i, ++p)
	if (*p != 0)
	  d->datafree(ghash_entry_dataptr(*p, d->keysize));
    }
    for (i = 0, p = d->table; i < d->size; ++i, ++p)
      if (*p != 0)
	free(*p);
    free(d->table);
  }
  memset(d, 0, sizeof *d);
}
