#ifndef IO_BUF__H__
#define IO_BUF__H__

#define IOBUF_EOF 1
#define IOBUF_ERROR 2

extern unsigned iobuf_bufsize;

struct iobuf
{
  int fd;
  char* buffer;
  unsigned bufsize;		/* Total buffer size */
  unsigned buflen;		/* Length of the data in the buffer */
  unsigned bufstart;		/* Start of the data in the buffer */
  unsigned offset;		/* Current file read/write offset */
  int errnum;			/* Saved errno flag */
  unsigned flags;		/* Status flags */
  int do_free;			/* Free the buffer */
  int do_close;			/* Set if iobuf should close the FD */
};
typedef struct iobuf iobuf;

int iobuf_init(iobuf* io, int fd, int do_close, unsigned bufsize, char* buffer);
int iobuf_close(iobuf* io);
#define iobuf_closed(io) ((io)->fd == -1)
#define iobuf_error(io) ((io)->flags & IOBUF_ERROR)

struct ibuf
{
  iobuf io;
  unsigned count;
};
typedef struct ibuf ibuf;

extern ibuf inbuf;

int ibuf_init(ibuf* in, int fd, int do_close, unsigned bufsize);
int ibuf_open(ibuf* in, const char* filename, unsigned bufsize);
int ibuf_eof(ibuf* in);
#define ibuf_close(in) iobuf_close(&((in)->io))
#define ibuf_closed(in) iobuf_closed(&((in)->io))
#define ibuf_error(in) iobuf_error(&((in)->io))
int ibuf_refill(ibuf* in);
int ibuf_read_large(ibuf* in, char* data, unsigned datalen);
int ibuf_read(ibuf* in, char* data, unsigned datalen);
unsigned ibuf_tell(ibuf* in);
int ibuf_seek(ibuf* in, unsigned offset);
#define ibuf_rewind(in) ibuf_seek(in,0)
#define ibuf_seekfwd(in,off) ibuf_seek(ibuf_tell(in)+(offset))

int ibuf_peek(ibuf* in, char* ch);
int ibuf_getc(ibuf* in, char* ch);
int ibuf_getu(ibuf* in, unsigned long* data);
int ibuf_gets(ibuf* in, char* data, unsigned datalen, char boundary);

struct obuf
{
  iobuf io;
  unsigned bufpos;
  unsigned count;
};
typedef struct obuf obuf;

extern obuf outbuf;
extern obuf errbuf;

#include <fcntl.h>
#define OBUF_CREATE O_CREAT
#define OBUF_EXCLUSIVE O_EXCL
#define OBUF_TRUNCATE O_TRUNC
#define OBUF_APPEND O_APPEND

int obuf_init(obuf* out, int fd, int do_close, unsigned bufsize);
int obuf_open(obuf* out, const char* filename, int flags, int mode, unsigned bufsize);
int obuf_close(obuf* out);
#define obuf_error(out) iobuf_error(&(out)->io)
#define obuf_closed(out) iobuf_closed(&(out)->io)
int obuf_flush(obuf* out);
int obuf_sync(obuf* out);
int obuf_write_large(obuf* out, const char* data, unsigned datalen);
int obuf_write(obuf* out, const char* data, unsigned datalen);
int obuf_seek(obuf* out, unsigned offset);
#define obuf_rewind(out) obuf_seek(out,0)
#define obuf_tell(out) ((out)->io.offset+(out)->bufpos)

int obuf_pad(obuf* out, unsigned width, char ch);
int obuf_endl(obuf* out);
int obuf_putc(obuf* out, char ch);
#define obuf_puts(out,str) obuf_write(out,str,strlen(str))
int obuf_putsflush(obuf* out, const char* str);
int obuf_putiw(obuf* out, long data, unsigned width);
#define obuf_puti(out,data) obuf_putiw(out, data, 0)
int obuf_putuw(obuf* out, unsigned long data, unsigned width);
#define obuf_putu(out,data) obuf_putuw(out, data, 0)
int obuf_putnetstring(obuf* out, const char* data, unsigned datalen);

int iobuf_copy(ibuf* in, obuf* out);
int iobuf_copyflush(ibuf* in, obuf* out);

#endif
