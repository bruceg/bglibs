#include "sysdeps.h"
#include <fcntl.h>
#include <unistd.h>
#include "trigger.h"

/** Activate a trigger. */
void trigger_pull(const char* path)
{
  int fd;
  if ((fd = open(path, O_WRONLY | O_NDELAY)) >= 0) {
    write(fd, "", 1);
    close(fd);
  }
}
