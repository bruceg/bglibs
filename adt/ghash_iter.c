#include "ghash.h"

static void next(struct ghashiter* iter, unsigned i)
{
  while (i < iter->ghashp->size && iter->ghashp->table[i] == 0)
    ++i;
  iter->index = i;
  iter->entry = iter->ghashp->table[i];
}

void ghashiter_first(struct ghashiter* iter, const struct ghash* h)
{
  iter->ghashp = h;
  next(iter, 0);
}

int ghashiter_valid(const struct ghashiter* iter)
{
  return iter->index < iter->ghashp->size
    && iter->ghashp->table[iter->index] != 0;
}

void ghashiter_next(struct ghashiter* iter)
{
  next(iter, iter->index + 1);
}

#ifdef SELFTEST_MAIN
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static struct ghash dict;
GHASH_DECL(test,char*,int);
GHASH_DEFN(test,char*,int,ghash_hashsp,keycmp,keycopy,datacopy,keyfree,0);

char* keys[] = {
  "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
  "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
  0
};

int main(void)
{
  int i;
  struct test_entry* p;
  struct ghashiter iter;
  test_init(&dict);
  for (i = 0; keys[i] != 0; ++i)
    test_add(&dict, &keys[i], &i);
  ghashiter_loop(&iter,&dict) {
    p = iter.entry;
    printf("[%s] = %i\n", p->key, p->data);
  }
  return 0;
}
#endif
#ifdef SELFTEST_EXP
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
