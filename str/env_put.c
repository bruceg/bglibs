/* $Id$ */
#include <string.h>
#include "str.h"
#include "env.h"

/** Put an assignment, in \c NAME=value format, into the environment
 * string.
 * \note Unlike putenv, a copy of the assignment is made instead of
 * keeping a copy of the given pointer. */
int envstr_put(struct str* env, const char* asgn, int overwrite)
{
  long varlen;
  const char* found;
  found = strchr(asgn, '=');
  varlen = (found == 0) ? (long)strlen(asgn) : found - asgn;
  if ((found = envstr_find(env, asgn, varlen)) != 0) {
    if (!overwrite)
      return 1;
    str_spliceb(env, found - env->s, strlen(found) + 1, 0, 0);
  }
  return str_cats(env, asgn)
    && str_catc(env, 0);
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
void selftest(void)
{
  static str env;
  debugstrfn(envstr_put(&env, "A=4", 0), &env);
  debugstrfn(envstr_put(&env, "A=5", 0), &env);
  debugstrfn(envstr_put(&env, "A=6", 1), &env);
}
#endif
#ifdef SELFTEST_EXP
result=1 len=4 size=16 s=A=4^@
result=1 len=4 size=16 s=A=4^@
result=1 len=4 size=16 s=A=6^@
#endif
