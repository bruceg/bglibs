#ifndef LIB__PATH__H__
#define LIB__PATH__H__

#include "str/str.h"

extern int path_merge(str* path, const char* start);
extern int fnmatch(const char* filename, const char* pattern);
extern int has_magic(const char* str);
extern int path_match(const char* pattern, str* result);
extern int path_contains(const char* path, const char* part);

#endif
