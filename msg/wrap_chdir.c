#include <unistd.h>

#include "msg.h"
#include "wrap.h"

void wrap_chdir(const char* dir)
{
  if (chdir(dir) != 0)
    die3sys(wrap_exit, "Could not chdir to '", dir, "'");
}
