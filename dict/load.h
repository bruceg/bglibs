#ifndef DICT__LOAD__H__
#define DICT__LOAD__H__

#include "dict.h"

extern int dict_load_list(dict*, const char* filename,  int mustexist,
			  int (*xform)(str*));
extern int dict_load_map(dict*, const char* filename, int mustexist, char sep,
			 int (*keyxform)(str*), int (*valxform)(str*));

#endif
