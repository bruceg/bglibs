/* Adapted from public domain sources written by D. J. Bernstein */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include "cdb.h"

void cdb_free(struct cdb *c)
{
  if (c->map) {
    munmap(c->map,c->size);
    c->map = 0;
  }
}

void cdb_findstart(struct cdb *c)
{
  c->loop = 0;
}

void cdb_init(struct cdb *c,int fd)
{
  struct stat st;
  char *x;

  cdb_free(c);
  cdb_findstart(c);
  c->fd = fd;

  if (fstat(fd,&st) == 0)
    if ((uint32)st.st_size <= 0xffffffff) {
      x = mmap(0,st.st_size,PROT_READ,MAP_SHARED,fd,0);
      if (x + 1) {
	c->size = st.st_size;
	c->map = x;
      }
    }
}
