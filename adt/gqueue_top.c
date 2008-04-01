#include "gqueue.h"

/** Return the address of first (least recently pushed) element in the
 * queue. */
void* gqueue_top(const struct gqueue* q)
{
  return (q->head == 0) ? 0 : q->head->data;
}
