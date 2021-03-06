#include <stdlib.h>
#include <string.h>

#include "gqueue.h"

#ifdef SELFTEST_MAIN

GQUEUE_DECL(test,int);
GQUEUE_DEFN(test,int,0,0);
static struct gqueue queue;

MAIN
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
0
1
2
3
#endif
