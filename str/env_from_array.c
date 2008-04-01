#include <string.h>
#include "str.h"
#include "env.h"

/** Adds the assignments from the array of pointers to the environment
 * string.
 * The array must be terminated by a \c NULL pointer, just the same as
 * the standard \c environ array. */
int envstr_from_array(struct str* env, char** array, int overwrite)
{
  while (*array) {
    if (!envstr_put(env, *array, overwrite))
      return 0;
    ++array;
  }
  return 1;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
MAIN
{
  static str env;
  char* array[3] = {"A=1","B=2",0};
  debugstrfn(envstr_from_array(&env, array, 1), &env);
}
#endif
#ifdef SELFTEST_EXP
result=1 len=8 size=16 s=A=1^@B=2^@
#endif
