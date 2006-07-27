/* $Id$ */
#include <stdlib.h>

#include "gqueue.h"

/** Remove the first (least recently pushed) element from the queue. If
 * the free function \c fn is \c NULL no data freeing is done. Note that
 * this does not return a pointer to the popped item, as once the item
 * has been popped it is also freed. */
void gqueue_pop(struct gqueue* q, adt_free_fn* fn)
{
  if (q->head) {
    struct gqueue_node* head = q->head;
    if (fn != 0)
      fn(head->data);
    if ((q->head = head->next) == 0)
      q->tail = 0, q->count = 0;
    else
      --q->count;
    free(head);
  }
}

