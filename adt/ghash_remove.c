/* $Id$ */
#include <stdlib.h>
#include <string.h>

#include "ghash.h"

/** Remove an entry from a \c ghash table. This function attempts to do
 * the minimum amount of rebuilding necessary to adjust the positions of
 * entries that may fall in the same slot as the newly removed entry.
 *
 * \returns True if the entry was found (and removed), false otherwise.
 */
int ghash_remove(struct ghash* d, const void* key)
{
  void* entry;
  void** slot;
  unsigned i;

  /* First, locate the key in the hash table. */
  if ((slot = ghash_find(d, key)) == 0)
    return 0;
  entry = *slot;

  /* Then free it, and empty out its slot. */
  if (d->keyfree != 0)
    d->keyfree(ghash_entry_keyptr(entry));
  if (d->datafree != 0)
    d->datafree(ghash_entry_dataptr(entry, d->keysize));
  free(entry);
  *slot = 0;
  --d->count;

  /* Finally, make sure all subsequent entries are properly inserted. */
  for (i = (slot - d->table + 1) % d->size;
       (entry = d->table[i]) != 0;
       i = (i + 1) % d->size) {
    --d->count;
    d->table[i] = 0;
    ghash_insert(d, entry);
  }
  return 1;
}

#ifdef SELFTEST_MAIN
#include <stdio.h>

static struct ghash dict;
GHASH_DECL(test,const char*,int);
GHASH_DEFN(test,const char*,int,adt_hashsp,adt_cmpsp,adt_copysp,0,adt_freesp,0);

static void print(struct test_entry* entry)
{
  printf("[%s] = %d\n", entry->key, entry->data);
}

const char* keys[] = {
  "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
  "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
  0
};

int main(void)
{
  int i;
  struct test_entry* p;
  test_init(&dict);
  for (i = 0; keys[i] != 0; ++i)
    test_add(&dict, &keys[i], &i);
  ghash_remove(&dict, &keys[1]);
  ghash_remove(&dict, &keys[11]);
  ghash_remove(&dict, &keys[24]);
  for (i = 0; keys[i] != 0; ++i) {
    if ((p = test_get(&dict, &keys[i])) == 0)
      printf("Could not locate key [%s]\n", keys[i]);
    else if (p->data != i)
      printf("Result for key [%s] is wrong (was %d should be %d)\n",
	     keys[i], p->data, i);
  }
  test_foreach(&dict, print);
  test_free(&dict);
  return 0;
}
#endif
#ifdef SELFTEST_EXP
Could not locate key [b]
Could not locate key [l]
Could not locate key [y]
[e] = 4
[d] = 3
[g] = 6
[f] = 5
[a] = 0
[c] = 2
[m] = 12
[o] = 14
[n] = 13
[i] = 8
[h] = 7
[k] = 10
[j] = 9
[u] = 20
[t] = 19
[w] = 22
[v] = 21
[q] = 16
[p] = 15
[s] = 18
[r] = 17
[x] = 23
[z] = 25
#endif
