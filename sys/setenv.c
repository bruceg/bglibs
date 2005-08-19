/* $Id$ */
#include <stdlib.h>
#include <string.h>
#include "sysdeps.h"

#ifndef HASSETENV
int setenv(const char* name, const char* value, int overwrite)
{
  char* newenv;
  size_t namelen;
  size_t valuelen;
  if (!overwrite && getenv(name)) return 0;
  namelen = strlen(name);
  valuelen = strlen(value);
  newenv = malloc(namelen+1+valuelen+1);
  memcpy(newenv, name, namelen);
  newenv[namelen] = '=';
  memcpy(newenv+namelen+1, value, valuelen+1);
  return putenv(newenv);
}
#endif
