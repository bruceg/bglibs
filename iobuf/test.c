#include "iobuf.h"

int main(void)
{
  obuf buf;
  obuf_init(&buf, 2, 0, 10);
  obuf_puts(&buf, "testing...");
  obuf_puts(&buf, " 1, 2, 3");
  obuf_putsflush(&buf, "...\n");
  return 0;
}
