#include <stdio.h>
#include "str.h"

static str a = {0};
static str b = {0};

void debugstr(const char* name, str* s)
{
  unsigned i;
  printf("%s.len=%u %s.size=%u %s.s=", name, s->len, name, s->size, name);
  for (i = 0; i < s->len; i++)
    printf("%02x", s->s[i]);
  puts("");
}

#define DS(X) debugstr(#X, &X)

int main(void)
{
  str_init(&a); DS(a);
  str_ready(&b, 17); DS(b);
  str_copys(&a, "foo"); DS(a);
  str_copy(&b, &a); DS(b);
  str_copyb(&a, "012345678901234567", 17); DS(a);
  str_cat(&b, &a); DS(b);
  str_catc(&b, 0); DS(b);
  printf("findfirst=%d\n", str_findfirst(&a, '7'));
  printf("findfirstof=%d\n", str_findfirstof(&a, "789"));
  printf("findnext=%d\n", str_findnext(&a, '4', 8));
  printf("findnextof=%d\n", str_findnextof(&a, "456", 8));
  return 0;
}
