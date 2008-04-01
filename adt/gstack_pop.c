#include <stdlib.h>

#include "gstack.h"

/** Remove the first (most recently pushed) element from the queue. If
 * the free function \c fn is \c NULL no data freeing is done. Note that
 * this does not return a pointer to the popped item, as once the item
 * has been popped it is also freed. */
void gstack_pop(struct gstack* s, adt_free_fn* fn)
{
  if (s->head) {
    struct gstack_node* head = s->head;
    if (fn != 0)
      fn(head->data);
    s->head = head->next;
    free(head);
    --s->count;
  }
}

