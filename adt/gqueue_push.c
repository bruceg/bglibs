/* $Id$ */
#include <stdlib.h>
#include <string.h>

#include "gqueue.h"

/** Add a new element onto the queue. If the copy function \c fn is \c
 * NULL memcpy is used in its place. */
int gqueue_push(struct gqueue* q, unsigned datasize, const void* data,
		adt_copy_fn* fn)
{
  struct gqueue_node* n;
  if ((n = malloc(datasize + sizeof *n)) == 0) return 0;
  if (fn == 0)
    memcpy(n->data, data, datasize);
  else if (!fn(n->data, data)) {
    free(n);
    return 0;
  }
  if (q->tail == 0)
    q->head = n;
  else
    q->tail->next = n;
  q->tail = n;
  ++q->count;
  return 1;
}
