#include <unistd.h>
#include "iobuf.h"

#define BREAK write(1,"brk\n",4)

int main(void)
{
  obuf buf;
  /* Create an artifically tiny buffer size */
  obuf_init(&buf, 1, 0, 0, 8);
  /* Test proper merging of short writes */
  obuf_puts(&buf, "0123"); BREAK;
  obuf_puts(&buf, "456789\n"); BREAK;
  /* Test pass-through of large writes */
  obuf_puts(&buf, "01234567\n"); BREAK;
  /* Test formatting of numbers */
  obuf_putiw(&buf,  10, 0,   0); obuf_putsflush(&buf, "\n");
  obuf_putiw(&buf, -10, 0,   0); obuf_putsflush(&buf, "\n");
  obuf_putuw(&buf,  10, 0,   0); obuf_putsflush(&buf, "\n");
  obuf_putiw(&buf,  10, 5, ' '); obuf_putsflush(&buf, "\n");
  obuf_putiw(&buf,  10, 5, '0'); obuf_putsflush(&buf, "\n");
  obuf_putiw(&buf, -10, 5, ' '); obuf_putsflush(&buf, "\n");
  obuf_putiw(&buf, -10, 5, '0'); obuf_putsflush(&buf, "\n");
  obuf_putuw(&buf,  10, 5, ' '); obuf_putsflush(&buf, "\n");
  obuf_putuw(&buf,  10, 5, '0'); obuf_putsflush(&buf, "\n");
  return 0;
}
