/* Adapted from public domain sources written by D. J. Bernstein */

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "cdb.h"

#if 0
int cdb_firstrec(struct cdb* c, uint32* keylen, uint32* datalen)
{
  c->nextrec = 0;
  if (c->map) {
    if (size <= 2048) return 0;
    
}

int cdb_nextrec(struct cdb* c, uint32* keylen, uint32* datalen)
{
}
#endif
