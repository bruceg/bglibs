#include <string.h>
#include "sysdeps.h"

#ifndef HASUNSETENV
void unsetenv(const char *name)
{
  extern char** environ;

  if (name) {
    unsigned long len = strlen(name);
    char** p = environ;

    while (*p) {
      if (strncmp(*p, name, len) == 0 && (*p)[len] == '=') {
	do
	  *p = *(p + 1);
	while (*++p);
      }
      else
	++p;
    }
  }
}
#endif
