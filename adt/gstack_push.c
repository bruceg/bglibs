#include <stdlib.h>
#include <string.h>

#include "gstack.h"

/** Add a new element onto the stack. If the copy function \c fn is \c
 * NULL memcpy is used in its place. */
int gstack_push(struct gstack* s, unsigned datasize, const void* data,
		adt_copy_fn* fn)
{
  struct gstack_node* n;
  if ((n = malloc(datasize + sizeof *n)) == 0) return 0;
  if (fn == 0)
    memcpy(n->data, data, datasize);
  else if (!fn(n->data, data)) {
    free(n);
    return 0;
  }
  n->next = s->head;
  s->head = n;
  ++s->count;
  return 1;
}
