/* $Id$ */
#include "dict.h"

int dict_init(dict* d)
{
  d->count = d->size = 0;
  d->table = 0;
  return 1;
}
