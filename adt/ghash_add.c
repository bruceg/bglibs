#include <stdlib.h>
#include <string.h>

#include "ghash.h"

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

static int ghash_grow(struct ghash* d, unsigned count)
{
  unsigned i;
  unsigned newsize;
  void** newtable;
  void** oldtable;
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
  if (oldtable) {
    for (i = 0; i < oldsize; i++)
      if (oldtable[i]) ghash_insert(d, oldtable[i]);
    free(oldtable);
  }
  return 1;
}

int ghash_add(struct ghash* d, const void* key, const void* data)
{
  const unsigned long hash = d->hashfn(key);
  void* newe;
  if (!ghash_grow(d, d->count + 1)) return 0;
  if ((newe = malloc(d->entrysize)) == 0) return 0;
  memset(newe, 0, d->entrysize);
  ghash_entry_hash(newe) = hash;
  if (!d->keycopy(ghash_entry_keyptr(newe), key)) {
    free(newe);
    return 0;
  }
  if (!d->datacopy(ghash_entry_dataptr(newe, d->keysize), data)) {
    d->keyfree(ghash_entry_keyptr(newe));
    free(newe);
    return 0;
  }
  ghash_insert(d, newe);
  return 1;
}

#ifdef SELFTEST_MAIN
#include <stdio.h>

static int keycmp(char* const* a, char* const* b)
{
  return strcmp(*a, *b);
}

static int keycopy(char** a, char* const* b)
{
  return (*a = strdup(*b)) != 0;
}

static int datacopy(char** a, char* const* b)
{
  *a = *b;
  return 1;
}

static void keyfree(char** a)
{
  free(*a);
}

static unsigned long hash(char* const* a)
{
  return ghash_hashs(*a);
}

static struct ghash dict;
GHASH_DECL(test,char*,int);
GHASH_DEFN(test,char*,int,hash,keycmp,keycopy,datacopy,keyfree,0);

static void print(struct test_entry* entry)
{
  printf("[%s] = %d\n", entry->key, entry->data);
}

char* keys[] = {
  "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
  "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
  0
};

int main(void)
{
  int i;
  struct test_entry* p;
  test_init(&dict);
  printf("size=%d count=%d\n", dict.size, dict.count);
  for (i = 0; keys[i] != 0; ++i) {
    test_add(&dict, &keys[i], &i);
    printf("size=%d count=%d\n", dict.size, dict.count);
  }
  for (i = 0; keys[i] != 0; ++i) {
    if ((p = test_get(&dict, &keys[i])) == 0)
      printf("Could not locate key [%s]\n", keys[i]);
    if (p->data != i)
      printf("Result for key [%s] is wrong (was %d should be %d)\n",
	     keys[i], p->data, i);
  }
  test_foreach(&dict, print);
  test_free(&dict);
  return 0;
}
#endif
#ifdef SELFTEST_EXP
size=0 count=0
size=31 count=1
size=31 count=2
size=31 count=3
size=31 count=4
size=31 count=5
size=31 count=6
size=31 count=7
size=31 count=8
size=31 count=9
size=31 count=10
size=31 count=11
size=31 count=12
size=31 count=13
size=31 count=14
size=31 count=15
size=61 count=16
size=61 count=17
size=61 count=18
size=61 count=19
size=61 count=20
size=61 count=21
size=61 count=22
size=61 count=23
size=61 count=24
size=61 count=25
size=61 count=26
[e] = 4
[d] = 3
[g] = 6
[f] = 5
[a] = 0
[c] = 2
[b] = 1
[m] = 12
[l] = 11
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
[y] = 24
[x] = 23
[z] = 25
#endif
