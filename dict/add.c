#include <stdlib.h>
#include <string.h>
#include "dict.h"

static const unsigned size_prime_list[] = {
  /* The sizes were calculated as the largest prime less than a power
     of two.  This gives them the distribution properties of primes
     while wasting a minimum of overhead space.  The "q" below
     represents the smallest integer that results in a prime
     number. */
  /* (2 ^  5) - q */         31U,
  /* (2 ^  6) - q */         61U,
  /* (2 ^  7) - q */        127U,
  /* (2 ^  8) - q */        251U,
  /* (2 ^  9) - q */        509U,
  /* (2 ^ 10) - q */       1021U,
  /* (2 ^ 11) - q */       2039U,
  /* (2 ^ 12) - q */       4093U,
  /* (2 ^ 13) - q */       8191U,
  /* (2 ^ 14) - q */      16381U,
  /* (2 ^ 15) - q */      32749U,
  /* (2 ^ 16) - q */      65521U,
  /* (2 ^ 17) - q */     131071U,
  /* (2 ^ 18) - q */     262139U,
  /* (2 ^ 19) - q */     524287U,
  /* (2 ^ 20) - q */    1048573U,
  /* (2 ^ 21) - q */    2097143U,
  /* (2 ^ 22) - q */    4194301U,
  /* (2 ^ 23) - q */    8388593U,
  /* (2 ^ 24) - q */   16777213U,
  /* (2 ^ 25) - q */   33554393U,
  /* (2 ^ 26) - q */   67108859U,
  /* (2 ^ 27) - q */  134217689U,
  /* (2 ^ 28) - q */  268435399U,
  /* (2 ^ 29) - q */  536870909U,
  /* (2 ^ 30) - q */ 1073741789U,
  /* (2 ^ 31) - q */ 2147483647U,
  /* (2 ^ 32) - q */ 4294967291U,
};

static void dict_insert(dict* d, dict_entry* e)
{
  unsigned i;
  for (i = e->hash % d->size; d->table[i]; i = (i + 1) % d->size) ;
  d->table[i] = e;
  ++d->count;
}

static int dict_grow(dict* d, unsigned count)
{
  unsigned i;
  unsigned newsize;
  dict_entry** newtable;
  dict_entry** oldtable;
  unsigned oldsize;
  count *= 2;
  if (d->size >= count) return 1;
  for (i = 0; count > size_prime_list[i]; ++i) ;
  newsize = size_prime_list[i];
  if ((newtable = malloc(newsize * sizeof *newtable)) == 0) return 0;
  memset(newtable, 0, newsize * sizeof *newtable);
  oldsize = d->size;
  oldtable = d->table;
  d->size = newsize;
  d->table = newtable;
  d->count = 0;
  for (i = 0; i < oldsize; i++)
    if (oldtable[i]) dict_insert(d, oldtable[i]);
  if (oldtable) free(oldtable);
  return 1;
}

int dict_add(dict* d, const str* key, void* data)
{
  dict_entry* newe;
  if (!dict_grow(d, d->count + 1)) return 0;
  if ((newe = malloc(sizeof *newe)) == 0) return 0;
  memset(newe, 0, sizeof *newe);
  newe->hash = dict_hashstr(key);
  str_copy(&newe->key, key);
  newe->data = data;
  dict_insert(d, newe);
  return 1;
}
