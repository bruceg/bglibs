#include "iter.h"

void striter_start(striter* iter, const str* str, char sep)
{
  iter->str = str;
  iter->start = 0;
  iter->startptr = str->s;
  iter->len = str_findfirst(str, sep);
  iter->sep = sep;
  if (iter->len == (unsigned)-1)
    iter->len = str->len;
}
