#include <string.h>
#include "str.h"
#include "env.h"

/** Unset the named variable in the environment string. */
void envstr_unset(struct str* env, const char* var)
{
  const char* found;
  if ((found = envstr_find(env, var, strlen(var))) != 0)
    str_spliceb(env, found - env->s, strlen(found) + 1, 0, 0);
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
MAIN
{
  static str env;
  str_copyb(&env, "A=1\0B=2\0C=3\0", 12);
  envstr_unset(&env, "B"); debugstr(&env);
  envstr_unset(&env, "A"); debugstr(&env);
  envstr_unset(&env, "C"); debugstr(&env);
}
#endif
#ifdef SELFTEST_EXP
len=8 size=16 s=A=1^@C=3^@
len=4 size=16 s=C=3^@
len=0 size=16 s=
#endif
