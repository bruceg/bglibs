#include "ghash.h"

void ghash_init(struct ghash* d)
{
  d->table = 0;
  d->count = 0;
  d->size = 0;
}
