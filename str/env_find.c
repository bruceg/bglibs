/* $Id$ */
#include <string.h>
#include "str.h"
#include "iter.h"
#include "env.h"

/** Find the named variable in the environment string.
 * \returns a pointer to the \b start of the assignment.
 */
const char* envstr_find(const struct str* env, const char* var, long len)
{
  striter i;
  striter_loop(&i, env, 0) {
    if (memcmp(i.startptr, var, len) == 0
	&& i.startptr[len] == '=')
      return i.startptr;
  }
  return 0;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
MAIN
{
  static const str env = {"A=1\0B=2\0C=3\0", 12, 0};
  const char* c;
  c = envstr_find(&env, "B", 1);
  obuf_puts(&outbuf, c); obuf_endl(&outbuf);
}
#endif
#ifdef SELFTEST_EXP
B=2
#endif
