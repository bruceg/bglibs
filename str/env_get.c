#include <string.h>
#include "str.h"
#include "striter.h"
#include "env.h"

/** Get the value of an assignment from an environment string. */
const char* envstr_get(const struct str* env, const char* var)
{
  long varlen = strlen(var);
  if ((var = envstr_find(env, var, varlen)) != 0)
    var += varlen + 1;
  return var;
}

#ifdef SELFTEST_MAIN
MAIN
{
  static const str env = {"A=1\0B=2\0C=3\0", 12, 0};
  const char* c;
  c = envstr_get(&env, "B");
  obuf_puts(&outbuf, c); obuf_endl(&outbuf);
}
#endif
#ifdef SELFTEST_EXP
2
#endif
