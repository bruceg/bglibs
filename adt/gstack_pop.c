/* $Id$ */
#include <stdlib.h>

#include "gstack.h"

void gstack_pop(struct gstack* s, adt_free_fn* fn)
{
  if (s->head) {
    struct gstack_node* head = s->head;
    fn(head->data);
    s->head = head->next;
    free(head);
    --s->count;
  }
}

