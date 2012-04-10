#include <string.h>
#include "bglibs/str.h"
#include "env.h"

/** Set the named variable to the given value in the environment string. */
int envstr_set(struct str* env, const char* var, const char* val, int overwrite)
{
  const char* found;
  if ((found = envstr_find(env, var, strlen(var))) != 0) {
    if (!overwrite)
      return 1;
    str_spliceb(env, found - env->s, strlen(found) + 1, 0, 0);
  }
  return str_cats(env, var)
    && str_catc(env, '=')
    && str_cats(env, val)
    && str_catc(env, 0);
}

#ifdef SELFTEST_MAIN
MAIN
{
  static str env;
  debugstrfn(envstr_set(&env, "A", "4", 0), &env);
  debugstrfn(envstr_set(&env, "A", "5", 0), &env);
  debugstrfn(envstr_set(&env, "A", "6", 1), &env);
}
#endif
#ifdef SELFTEST_EXP
result=1 len=4 size=16 s=A=4^@
result=1 len=4 size=16 s=A=4^@
result=1 len=4 size=16 s=A=6^@
#endif
