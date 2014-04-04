#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include "iobuf.h"

/** The default iobuf buffer size, defaults to 8192. */
unsigned iobuf_bufsize = 8192;

#if !defined(MAP_ANONYMOUS) && defined(MAP_ANON)
#define MAP_ANONYMOUS MAP_ANON
#endif

#ifndef MAP_FAILED
#define MAP_FAILED ((void*)-1)
#endif

/** Initialize an \c iobuf structure. */
int iobuf_init(iobuf* io, int fd, unsigned bufsize, char* buffer, unsigned flags)
{
  memset(io, 0, sizeof *io);
  if (!bufsize) bufsize = iobuf_bufsize;
#ifdef MAP_ANONYMOUS
  if (!buffer) {
    if ((buffer = mmap(0, bufsize, PROT_READ|PROT_WRITE,
		       MAP_PRIVATE|MAP_ANONYMOUS, -1, 0)) != MAP_FAILED)
      flags |= IOBUF_NEEDSMUNMAP;
  }
#endif
  if (!buffer) {
    if ((buffer = malloc(bufsize)) == 0) return 0;
    flags |= IOBUF_NEEDSFREE;
  }
  io->fd = fd;
  io->buffer = buffer;
  io->bufsize = bufsize;
  io->flags = flags;
  return 1;
}

#ifdef SELFTEST_MAIN
MAIN
{
  static iobuf io;
  debugfn(iobuf_init(&io, 1, 0, 0, 0x100));
  if (io.buffer == 0) obuf_puts(&outbuf, "buffer is NULL!\n");
  if (io.buffer == MAP_FAILED) obuf_puts(&outbuf, "mmap failed!\n");
  obuf_puts(&outbuf, "bufsize="); obuf_putu(&outbuf, io.bufsize); NL();
  obuf_puts(&outbuf, "flags="); obuf_putx(&outbuf, io.flags); NL();
  obuf_puts(&outbuf, "fd="); obuf_puti(&outbuf, io.fd); NL();
}
#endif
#ifdef SELFTEST_EXP
result=1
bufsize=8192
flags=180
fd=1
#endif
