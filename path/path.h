#ifndef LIB__PATH__H__
#define LIB__PATH__H__

#include "str/str.h"

#define PATH_MATCH_DOTFILES 1

extern int path_merge(str* path, const char* start);
extern int fnmatch(const char* filename, const char* pattern,
		   unsigned options);
extern int has_magic(const char* str);
extern int path_match(const char* pattern, str* result, unsigned options);
extern int path_contains(const char* path, const char* part);

#endif
