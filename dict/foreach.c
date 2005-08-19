/* $Id$ */
#include <stdlib.h>
#include <string.h>
#include "dict.h"

void dict_foreach(dict* d, void (*fn)(const str* key, void** dataptr))
{
  unsigned i;
  dict_entry** entry;
  
  for (i = 0, entry = d->table; i < d->size; ++i, ++entry)
    if (*entry)
      fn(&(*entry)->key, &(*entry)->data);
}
