#include <stdlib.h>
#include <string.h>
#include "dict.h"

void dict_free(dict* d, void (*free_data)(void*))
{
  unsigned i;
  unsigned size = d->size;
  dict_entry** table = d->table;
  if (free_data)
    for (i = 0; i < size; ++i)
      if (table[i] && table[i]->data)
	free_data(table[i]->data);
  for (i = 0; i < size; i++)
    if (table[i]) {
      str_free(&table[i]->key);
      free(table[i]);
    }
  if (table) free(table);
  memset(d, 0, sizeof *d);
}
