#ifndef BGLIBS__STR__ENV__H__
#define BGLIBS__STR__ENV__H__

struct str;

/** \defgroup envstr envstr: Environment variables in a str.

\par Calling Convention

All functions that allocate memory return \c 0 (false) if the function
failed due to being unable to allocate memory, and non-zero (true)
otherwise.

@{ */

extern const char* envstr_find(const struct str* env, const char* var, long varlen);
extern const char* envstr_get(const struct str* env, const char* var);
extern int envstr_set(struct str* env, const char* var, const char* val, int overwrite);
extern void envstr_unset(struct str* env, const char* var);
extern int envstr_put(struct str* env, const char* asgn, int overwrite);
extern int envstr_from_array(struct str* env, char** array, int overwrite);
extern int envstr_from_string(struct str* env, const char* s, int overwrite);
extern char** envstr_make_array(const struct str* env);

/* @} */

#endif
