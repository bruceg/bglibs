/* $Id$ */
#include <stdlib.h>

#include "gstack.h"

int gstack_push(struct gstack* s, unsigned datasize, const void* data,
		adt_copy_fn* fn)
{
  struct gstack_node* n;
  if ((n = malloc(datasize + sizeof *n)) == 0) return 0;
  if (!fn(n->data, data)) {
    free(n);
    return 0;
  }
  n->next = s->head;
  s->head = n;
  ++s->count;
  return 1;
}
