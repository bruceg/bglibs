/* $Id$ */
#include "gqueue.h"

void* gqueue_top(const struct gqueue* q)
{
  return (q->head == 0) ? 0 : q->head->data;
}
