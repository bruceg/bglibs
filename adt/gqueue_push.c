/* $Id$ */
#include <stdlib.h>
#include <string.h>

#include "gqueue.h"

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
  if ((n->next = q->head) == 0)
    q->tail = n;
  q->head = n;
  ++q->count;
  return 1;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"

GQUEUE_DECL(test,int);
GQUEUE_DEFN(test,int,0,0);
static struct gqueue queue;

void selftest(void)
{
  int i;
  int* p;
  for (i = 0; i < 4; ++i)
    debugfn(test_push(&queue, &i));
  while ((p = test_top(&queue)) != 0) {
    obuf_puti(&outbuf, *p); NL();
    test_pop(&queue);
  }
}

#endif
#ifdef SELFTEST_EXP
result=1
result=1
result=1
result=1
3
2
1
0
#endif
