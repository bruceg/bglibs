#ifndef BGLIBS__STR__ENV__H__
#define BGLIBS__STR__ENV__H__

#include <str/str.h>

/** \defgroup envstr envstr: Environment variables in a str.

\par Calling Convention

All functions that allocate memory return \c 0 (false) if the function
failed due to being unable to allocate memory, and non-zero (true)
otherwise.

@{ */

extern const char* envstr_find(const str* env, const char* var, long varlen);
extern const char* envstr_get(const str* env, const char* var);
extern int envstr_set(str* env, const char* var, const char* val, int overwrite);
extern void envstr_unset(str* env, const char* var);
extern int envstr_put(str* env, const char* asgn, int overwrite);
extern int envstr_from_array(str* env, char** array, int overwrite);
extern int envstr_from_string(str* env, const char* s, int overwrite);
extern char** envstr_make_array(const str* env);

/* @} */

#endif
