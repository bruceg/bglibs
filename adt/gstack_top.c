/* $Id$ */
#include "gstack.h"

void* gstack_top(const struct gstack* s)
{
  return (s->head == 0) ? 0 : s->head->data;
}
