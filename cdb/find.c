/* $Id$ */
/* Adapted from public domain sources written by D. J. Bernstein */

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "cdb.h"
#include "uint32.h"

int cdb_read(struct cdb *c,char *buf,unsigned int len,uint32 pos)
{
  if (c->map) {
    if ((pos > c->size) || (c->size - pos < len)) goto FORMAT;
    memcpy(buf, c->map + pos, len);
  }
  else {
    if (lseek(c->fd, pos, SEEK_SET) == -1) return -1;
    while (len > 0) {
      int r;
      do
        r = read(c->fd,buf,len);
      while ((r == -1) && (errno == EINTR));
      if (r == -1) return -1;
      if (r == 0) goto FORMAT;
      buf += r;
      len -= r;
    }
  }
  return 0;

  FORMAT:
#ifdef EPROTO
  errno = EPROTO;
#else
  errno = EINTR;
#endif
  return -1;
}

static int match(struct cdb *c,const char *key,unsigned int len,uint32 pos)
{
  char buf[32];
  unsigned int n;

  while (len > 0) {
    n = sizeof buf;
    if (n > len) n = len;
    if (cdb_read(c,buf,n,pos) == -1) return -1;
    if (memcmp(buf, key, n) != 0) return 0;
    pos += n;
    key += n;
    len -= n;
  }
  return 1;
}

int cdb_findnext(struct cdb *c,const char *key,unsigned int len)
{
  char buf[8];
  uint32 pos;
  uint32 u;

  if (!c->loop) {
    u = cdb_hash(key,len);
    if (cdb_read(c,buf,8,(u << 3) & 2047) == -1) return -1;
    c->hslots = uint32_get(buf + 4);
    if (!c->hslots) return 0;
    c->hpos = uint32_get(buf);
    c->khash = u;
    u >>= 8;
    u %= c->hslots;
    u <<= 3;
    c->kpos = c->hpos + u;
  }

  while (c->loop < c->hslots) {
    if (cdb_read(c,buf,8,c->kpos) == -1) return -1;
    pos = uint32_get(buf + 4);
    if (!pos) return 0;
    c->loop += 1;
    c->kpos += 8;
    if (c->kpos == c->hpos + (c->hslots << 3)) c->kpos = c->hpos;
    u = uint32_get(buf);
    if (u == c->khash) {
      if (cdb_read(c,buf,8,pos) == -1) return -1;
      u = uint32_get(buf);
      if (u == len)
	switch(match(c,key,len,pos + 8)) {
	  case -1:
	    return -1;
	  case 1:
	    c->dlen = uint32_get(buf + 4);
	    c->dpos = pos + 8 + len;
	    return 1;
	}
    }
  }

  return 0;
}

int cdb_find(struct cdb *c,const char *key,unsigned int len)
{
  cdb_findstart(c);
  return cdb_findnext(c,key,len);
}
