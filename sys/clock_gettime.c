#include "sysdeps.h"
#ifndef HASCLOCKGETTIME

#include <errno.h>

int clock_gettime(clockid_t clk_id, struct timespec* tp)
{
  struct timeval tv;
  if (clk_id != CLOCK_REALTIME) {
    errno = EINVAL;
    return -1;
  }
  gettimeofday(&tv, 0);
  tp->tv_sec = tv.tv_sec;
  tp->tv_nsec = tv.tv_usec * 1000;
  return 0;
}

#endif
