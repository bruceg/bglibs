#include "iobuf.h"

int main(void)
{
  obuf buf;
  obuf_init(&buf, 1, 0, 10);
  obuf_puts(&buf, "testing...");
  obuf_puts(&buf, " 1, 2, 3");
  obuf_putsflush(&buf, "...\n");
  obuf_putiw(&buf,  10, 10, ' '); obuf_putsflush(&buf, "\n");
  obuf_putiw(&buf,  10, 10, '0'); obuf_putsflush(&buf, "\n");
  obuf_putiw(&buf, -10, 10, ' '); obuf_putsflush(&buf, "\n");
  obuf_putiw(&buf, -10, 10, '0'); obuf_putsflush(&buf, "\n");
  return 0;
}
