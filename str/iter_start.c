#include "iter.h"
#include "str.h"

/** Start (initialize) a striter. */
void striter_start(striter* iter, const str* s, char sep)
{
  iter->str = s;
  iter->start = 0;
  iter->startptr = s->s;
  iter->len = str_findfirst(s, sep);
  iter->sep = sep;
  if (iter->len == (unsigned)-1)
    iter->len = s->len;
}
