#include <stdlib.h>

#include "gqueue.h"

int gqueue_push(struct gqueue* q, unsigned datasize, const void* data,
		adt_copy_fn* fn)
{
  struct gqueue_node* n;
  if ((n = malloc(datasize + sizeof *n)) == 0) return 0;
  if (!fn(n->data, data)) {
    free(n);
    return 0;
  }
  if ((n->next = q->head) == 0)
    q->tail = n;
  q->head = n;
  ++q->count;
  return 1;
}
