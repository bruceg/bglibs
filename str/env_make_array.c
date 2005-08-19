/* $Id$ */
#include <stdlib.h>
#include <string.h>
#include "str.h"
#include "iter.h"
#include "env.h"

/** Make a \c environ compatable array of pointers to the environment
 * string.
 * \note The returned pointer is dynamically allocated, and must be
 * freed. */
char** envstr_make_array(const struct str* env)
{
  striter i;
  int count;
  char** array;
  for (striter_start(&i, env, 0), count = 0;
       striter_valid(&i);
       striter_advance(&i), ++count)
    ;
  array = malloc((count + 1) * sizeof *array);
  for (striter_start(&i, env, 0), count = 0;
       striter_valid(&i);
       striter_advance(&i), ++count)
    array[count] = (char*)i.startptr;
  array[count] = 0;
  return array;
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
void selftest(void)
{
  static const str env = {"A=1\0B=2\0C=3\0", 12, 0};
  char** e;
  int i;
  e = envstr_make_array(&env);
  for (i = 0; e[i] != 0; ++i) {
    obuf_puti(&outbuf, i);
    obuf_putc(&outbuf, ':');
    obuf_puts(&outbuf, e[i]);
    obuf_endl(&outbuf);
  }
}
#endif
#ifdef SELFTEST_EXP
0:A=1
1:B=2
2:C=3
#endif
