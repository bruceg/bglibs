#include <string.h>
#include "str.h"
#include "env.h"

/** Adds the string-based list of assignments to the environment string.
 * Each individual assignment in the list must be ASCII NUL terminated,
 * and the final assignment must be followed by two ASCII NULs. */
int envstr_from_string(str* env, const char* s, int overwrite)
{
  long len;
  while ((len = strlen(s)) > 0) {
    if (!envstr_put(env, s, overwrite))
      return 0;
    s += len + 1;
  }
  return 1;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
void selftest(void)
{
  static str env;
  debugstrfn(envstr_from_string(&env, "A=3\0C=4\0", 1), &env);
}
#endif
#ifdef SELFTEST_EXP
result=1 len=8 size=16 s=A=3^@C=4^@
#endif
