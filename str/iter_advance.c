#include "iter.h"
#include "bglibs/str.h"

/** Advance a striter to the next substring. */
int striter_advance(striter* iter)
{
  unsigned end;
  if (!iter->str) return 0;
  iter->start += iter->len + 1;
  iter->startptr = iter->str->s + iter->start;
  if (iter->start >= iter->str->len) return 0;
  end = str_findnext(iter->str, iter->sep, iter->start);
  iter->len = (end == (unsigned)-1) ?
    iter->str->len - iter->start :
    end - iter->start;
  return 1;
}
