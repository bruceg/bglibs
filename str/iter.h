#ifndef STR_ITER__H__
#define STR_ITER__H__

#include "str/str.h"

typedef struct
{
  const str* str;
  unsigned start;
  const char* startptr;
  unsigned len;
  char sep;
} striter;

void striter_start(striter* iter, const str* str, char sep);
int striter_valid(striter*);
int striter_advance(striter*);
#define str_copyiter(STR,ITER) str_copyb(STR,(ITER)->startptr,(ITER)->len)
#define str_catiter(STR,ITER) str_catb(STR,(ITER)->startptr,(ITER)->len)
#define obuf_putiter(OBUF,ITER) obuf_write(OBUF,(ITER)->startptr,(ITER)->len)

#endif
