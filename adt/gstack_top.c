/* $Id$ */
#include "gstack.h"

/** Return the address of first (most recently pushed) element in the
 * queue. */
void* gstack_top(const struct gstack* s)
{
  return (s->head == 0) ? 0 : s->head->data;
}
