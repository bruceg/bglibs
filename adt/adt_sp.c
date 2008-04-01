#include <stdlib.h>
#include <string.h>

#include "common.h"

int adt_cmpsp(const char* const* a, const char* const* b)
{
  return strcmp(*a, *b);
}

int adt_copysp(const char** a, const char* const* b)
{
  return (*a = strdup(*b)) != 0;
}

adt_hash_t adt_hashsp(const char* const* p)
{
  return adt_hashs((const unsigned char*)*p);
}

void adt_freesp(const char** a)
{
  free((char*)*a);
}
