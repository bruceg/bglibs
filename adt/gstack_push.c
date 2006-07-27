/* $Id$ */
#include <stdlib.h>
#include <string.h>

#include "gstack.h"

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

#ifdef SELFTEST_MAIN
#include "selftest.c"

GSTACK_DECL(test,int);
GSTACK_DEFN(test,int,0,0);
static struct gstack stack;

void selftest(void)
{
  int i;
  int* p;
  for (i = 0; i < 4; ++i)
    debugfn(test_push(&stack, &i));
  while ((p = test_top(&stack)) != 0) {
    obuf_puti(&outbuf, *p); NL();
    test_pop(&stack);
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
