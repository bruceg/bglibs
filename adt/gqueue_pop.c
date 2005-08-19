/* $Id$ */
#include <stdlib.h>

#include "gqueue.h"

void gqueue_pop(struct gqueue* q, adt_free_fn* fn)
{
  if (q->head) {
    struct gqueue_node* head = q->head;
    fn(head->data);
    if ((q->head = head->next) == 0)
      q->tail = 0, q->count = 0;
    else
      --q->count;
    free(head);
  }
}

