#include <unistd.h>

#include "msg.h"
#include "wrap.h"

void wrap_chdir(const char* dir)
{
  if (chdir(dir) != 0)
    die3sys(1, "Could not chdir to '", dir, "'");
}
