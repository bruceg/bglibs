#include <stdlib.h>
#include "obuf.h"
#include "installer.h"

const int msg_show_pid = 0;

extern void instprep(void);

const char* install_prefix = "";

int main(int argc, char* argv[])
{
  const char* tmp;
  if (argc > 1)
    install_prefix = argv[1];
  else if ((tmp = getenv("install_prefix")) != 0)
    install_prefix = tmp;
  instprep();
  insthier();
  obuf_flush(&outbuf);
  return 0;
}
