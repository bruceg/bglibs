#include "iter.h"

int striter_valid(striter* iter)
{
  return iter->str && iter->start < iter->str->len;
}
