#include <stdlib.h>
#include <string.h>

#include "ghash.h"

#ifdef SELFTEST_MAIN
#include "selftest.c"

static struct ghash dict;
GHASH_DECL(test,const char*,int);
GHASH_DEFN(test,const char*,int,adt_hashsp,adt_cmpsp,adt_copysp,0,adt_freesp,0);

static void print(struct test_entry* entry)
{
  obuf_putf(&outbuf, "{[}s{] = }i{\n}", entry->key, entry->data);
}

const char* keys[] = {
  "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
  "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
  0
};

static void testkeys(void)
{
  int i;
  struct test_entry* p;
  for (i = 0; keys[i] != 0; ++i) {
    if ((p = test_get(&dict, &keys[i])) == 0)
      obuf_putf(&outbuf, "{Could not locate key [}s{]\n}", keys[i]);
    else if (p->data != i)
      obuf_putf(&outbuf,
		"{Result for key [}s{] is wrong (was }d{ should be }d{)\n}",
		keys[i], p->data, i);
  }
}

MAIN
{
  int i;
  struct test_entry* p;
  struct ghashiter iter;

  test_init(&dict);
  obuf_putf(&outbuf, "{size=}d{ count=}d{\n}", dict.size, dict.count);

  for (i = 0; keys[i] != 0; ++i) {
    test_add(&dict, &keys[i], &i);
    obuf_putf(&outbuf, "{size=}d{ count=}d{\n}", dict.size, dict.count);
  }

  testkeys();

  test_foreach(&dict, print);

  ghash_remove(&dict, &keys[1]);
  ghash_remove(&dict, &keys[11]);
  ghash_remove(&dict, &keys[24]);
  testkeys();

  i = 1010;
  if ((p = ghash_set(&dict, &keys[10], &i)) != 0)
    obuf_putf(&outbuf, "{Set [}s{] to }d{\n}", p->key, p->data);
  i = 1020;
  if ((p = ghash_set(&dict, &keys[20], &i)) != 0)
    obuf_putf(&outbuf, "{Set [}s{] to }d{\n}", p->key, p->data);

  ghashiter_loop(&iter, &dict) {
    p = iter.entry;
    obuf_putf(&outbuf, "{[}s{] = }i{\n}", p->key, p->data);
  }

  test_free(&dict);
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
Could not locate key [b]
Could not locate key [l]
Could not locate key [y]
Set [k] to 1010
Set [u] to 1020
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
[k] = 1010
[j] = 9
[u] = 1020
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
