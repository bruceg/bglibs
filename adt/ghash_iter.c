/* $Id$ */
#include "ghash.h"

static void next(struct ghashiter* iter, unsigned i)
{
  if (iter->ghashp->table != 0) {
    while (i < iter->ghashp->size && iter->ghashp->table[i] == 0)
      ++i;
    iter->index = i;
    iter->entry = iter->ghashp->table[i];
  }
}

/** Set a \c ghash iterator to the first element in the table. */
void ghashiter_first(struct ghashiter* iter, const struct ghash* h)
{
  iter->ghashp = h;
  next(iter, 0);
}

/** Check if the \c ghash iterator is still valid. */
int ghashiter_valid(const struct ghashiter* iter)
{
  return iter->ghashp->table != 0
    && iter->index < iter->ghashp->size
    && iter->ghashp->table[iter->index] != 0;
}

/** Advance the \c ghash iterator to the next element in the table. */
void ghashiter_next(struct ghashiter* iter)
{
  next(iter, iter->index + 1);
}
