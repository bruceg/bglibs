/* $Id$ */
#ifndef CDB__STR__H__
#define CDB__STR__H__

#include "cdb.h"
struct str;

extern int cdb_get(struct cdb*, const struct str* key, struct str* data);
extern int cdb_getnext(struct cdb*, const struct str* key, struct str* data);

#endif
