/* $Id$ */
#include "sysdeps.h"
#include <fcntl.h>
#include <unistd.h>
#include "trigger.h"

/** Set up a trigger for polling.
 *
 * The passed \c iopoll_fd must have at least two slots, as the second
 * slot may be required in the event the OS exhibits the "named pipe
 * bug".  This function must be called every time the trigger is pulled
 * to reset the trigger's state.
 */
int trigger_set(iopoll_fd* io, const char* path)
{
  if (io[0].fd > 0)
    close(io[0].fd);
#ifdef HASNAMEDPIPEBUG
  if (io[1].fd > 0)
    close(io[1].fd);
#endif
  if ((io[0].fd = open(path, O_RDONLY | O_NDELAY)) == -1)
    return 0;
  io[0].events = IOPOLL_READ;
  io[0].revents = 0;
#ifdef HASNAMEDPIPEBUG
  if ((io[1].fd = open(path, O_WRONLY | O_NDELAY)) == -1)
    return 0;
#else
  io[1].fd = -1;
#endif
  io[1].events = 0;
  io[1].revents = 0;
  return 1;
}
