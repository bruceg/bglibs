#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ghash.h"

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

static struct ghash d;
GHASH_DECL(test,char*,int);
GHASH_DEFN(test,char*,int,hash,keycmp,keycopy,datacopy,keyfree,0);

static void print(struct test_entry* entry)
{
  printf("[%s] = %d\n", entry->key, entry->data);
}

int main(int argc, char* argv[])
{
  int i;
  test_init(&d);
  for (i = 0; i < argc; ++i)
    test_add(&d, &argv[i], &i);
  for (i = 0; i < argc; ++i)
    if (test_get(&d, &argv[i]) == 0)
      printf("Could not locate key [%s]\n", argv[i]);
  test_foreach(&d, print);
  test_free(&d);
  return 0;
}
