#include <stdlib.h>
#include <string.h>

#include "ghash.h"

/** Rebuild the entry pointer table in a \c ghash table. This function
 * is used internally after either rehashing the table or removing an
 * entry. */
int ghash_rebuild(struct ghash* d)
{
  unsigned i;
  void** newtable;
  void** oldtable;
  void** oldptr;
  if (d->table) {
    if ((newtable = malloc(d->size * sizeof *newtable)) == 0) return 0;
    memset(newtable, 0, d->size * sizeof *newtable);
    oldptr = oldtable = d->table;
    d->table = newtable;
    d->count = 0;
    for (i = 0; i < d->size; ++i, ++oldptr)
      if (*oldptr)
	ghash_insert(d, *oldptr);
    free(oldtable);
  }
  return 1;
}
