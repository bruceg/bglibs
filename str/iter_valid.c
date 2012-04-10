#include "iter.h"
#include "bglibs/str.h"

/** Determine if a striter is valid (after initialization or advancing). */
int striter_valid(striter* iter)
{
  return iter->str && iter->start < iter->str->len;
}
