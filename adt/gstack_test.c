#include <stdlib.h>
#include <string.h>

#include "gstack.h"

#ifdef SELFTEST_MAIN
#include "selftest.c"

GSTACK_DECL(test,int);
GSTACK_DEFN(test,int,0,0);
static struct gstack stack;

MAIN
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
