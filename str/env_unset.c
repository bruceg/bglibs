#include <string.h>
#include "str.h"
#include "env.h"

/** Unset the named variable in the environment string. */
void envstr_unset(struct str* env, const char* var)
{
  const char* found;
  unsigned long varlen = strlen(var);
  while ((found = envstr_find(env, var, varlen)) != 0) {
    unsigned long foundlen = strlen(found) + 1;
    unsigned long foundoff = found - env->s;
    if (foundoff + foundlen > env->len - foundoff)
      foundlen = env->len - (found - env->s);
    str_spliceb(env, found - env->s, foundlen, 0, 0);
  }
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
MAIN
{
  static str env;
  str_copyb(&env, "A=1\0B=2\0C=3\0B=2", 15);
  envstr_unset(&env, "B"); debugstr(&env);
  envstr_unset(&env, "A"); debugstr(&env);
  envstr_unset(&env, "C"); debugstr(&env);
}
#endif
#ifdef SELFTEST_EXP
len=8 size=32 s=A=1^@C=3^@
len=4 size=32 s=C=3^@
len=0 size=32 s=
#endif
