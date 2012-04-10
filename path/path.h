#ifndef LIB__PATH__H__
#define LIB__PATH__H__

#include "bglibs/str.h"

/** \defgroup path path: Filename manipulation functions

@{ */

/** Make path_match include files starting with dots. */
#define PATH_MATCH_DOTFILES 1

extern int path_merge(str* path, const char* start);
extern int fnmatch(const char* filename, const char* pattern,
		   unsigned options);
extern int has_magic(const char* s);
extern int path_match(const char* pattern, str* result, unsigned options);
extern int path_contains(const char* path, const char* part);
extern int path_mktemp(const char* prefix, str* filename);
extern int path_mkdirs(const char* path, unsigned mode);

/** @} */

#endif
