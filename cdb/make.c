/* Public domain. */

#include <errno.h>
#include <stdlib.h>
#include "cdb.h"
#include "make.h"

int cdb_make_start(struct cdb_make *c,int fd)
{
  c->head = 0;
  c->split = 0;
  c->hash = 0;
  c->numentries = 0;
  c->fd = fd;
  c->pos = sizeof c->final;
  if (!obuf_init(&c->b,fd,0,IOBUF_SEEKABLE,8192)) return -1;
  if (!obuf_write(&c->b,c->final,sizeof c->final)) {
    obuf_close(&c->b);
    return -1;
  }
  return 0;
}

static int posplus(struct cdb_make *c,uint32 len)
{
  uint32 newpos = c->pos + len;
  if (newpos < len) { errno = ENOMEM; return -1; }
  c->pos = newpos;
  return 0;
}

int cdb_make_addend(struct cdb_make *c,unsigned int keylen,unsigned int datalen,uint32 h)
{
  struct cdb_hplist *head;

  head = c->head;
  if (!head || (head->num >= CDB_HPLIST)) {
    head = (struct cdb_hplist *) malloc(sizeof(struct cdb_hplist));
    if (!head) return -1;
    head->num = 0;
    head->next = c->head;
    c->head = head;
  }
  head->hp[head->num].h = h;
  head->hp[head->num].p = c->pos;
  ++head->num;
  ++c->numentries;
  if (posplus(c,8) == -1) return -1;
  if (posplus(c,keylen) == -1) return -1;
  if (posplus(c,datalen) == -1) return -1;
  return 0;
}

int cdb_make_addbegin(struct cdb_make *c,unsigned int keylen,unsigned int datalen)
{
  char buf[8];

  if (keylen > 0xffffffff) { errno = ENOMEM; return -1; }
  if (datalen > 0xffffffff) { errno = ENOMEM; return -1; }

  uint32_pack(keylen,buf);
  uint32_pack(datalen,buf + 4);
  if (!obuf_write(&c->b,buf,8)) return -1;
  return 0;
}

int cdb_make_add(struct cdb_make *c,const char *key,unsigned int keylen,const char *data,unsigned int datalen)
{
  if (cdb_make_addbegin(c,keylen,datalen) == -1) return -1;
  if (!obuf_write(&c->b,key,keylen)) return -1;
  if (!obuf_write(&c->b,data,datalen)) return -1;
  return cdb_make_addend(c,keylen,datalen,cdb_hash(key,keylen));
}

int cdb_make_finish(struct cdb_make *c)
{
  char buf[8];
  int i;
  uint32 len;
  uint32 u;
  uint32 memsize;
  uint32 count;
  uint32 where;
  struct cdb_hplist *x;
  struct cdb_hp *hp;

  for (i = 0;i < 256;++i)
    c->count[i] = 0;

  for (x = c->head;x;x = x->next) {
    i = x->num;
    while (i--)
      ++c->count[255 & x->hp[i].h];
  }

  memsize = 1;
  for (i = 0;i < 256;++i) {
    u = c->count[i] * 2;
    if (u > memsize)
      memsize = u;
  }

  memsize += c->numentries; /* no overflow possible up to now */
  u = (uint32) 0 - (uint32) 1;
  u /= sizeof(struct cdb_hp);
  if (memsize > u) { errno = ENOMEM; return -1; }

  c->split = (struct cdb_hp *) malloc(memsize * sizeof(struct cdb_hp));
  if (!c->split) return -1;

  c->hash = c->split + c->numentries;

  u = 0;
  for (i = 0;i < 256;++i) {
    u += c->count[i]; /* bounded by numentries, so no overflow */
    c->start[i] = u;
  }

  for (x = c->head;x;x = x->next) {
    i = x->num;
    while (i--)
      c->split[--c->start[255 & x->hp[i].h]] = x->hp[i];
  }

  for (i = 0;i < 256;++i) {
    count = c->count[i];

    len = count + count; /* no overflow possible */
    uint32_pack(c->pos,c->final + 8 * i);
    uint32_pack(len,c->final + 8 * i + 4);

    for (u = 0;u < len;++u)
      c->hash[u].h = c->hash[u].p = 0;

    hp = c->split + c->start[i];
    for (u = 0;u < count;++u) {
      where = (hp->h >> 8) % len;
      while (c->hash[where].p)
	if (++where == len)
	  where = 0;
      c->hash[where] = *hp++;
    }

    for (u = 0;u < len;++u) {
      uint32_pack(c->hash[u].h,buf);
      uint32_pack(c->hash[u].p,buf + 4);
      if (!obuf_write(&c->b,buf,8)) return -1;
      if (posplus(c,8) == -1) return -1;
    }
  }

  if (!obuf_seek(&c->b,0)) return -1;
  if (!obuf_write(&c->b,c->final,sizeof c->final)) return -1;
  return 0;
}
