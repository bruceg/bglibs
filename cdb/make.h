#ifndef CDB_MAKE_H
#define CDB_MAKE_H

#include "iobuf.h"
#include "sysdeps.h"

#define CDB_HPLIST 1000

struct cdb_hp { uint32 h; uint32 p; } ;

struct cdb_hplist {
  struct cdb_hp hp[CDB_HPLIST];
  struct cdb_hplist *next;
  int num;
} ;

#endif
