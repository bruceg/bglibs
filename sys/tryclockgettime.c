#include "systime.h"

int main(void)
{
  struct timespec ts;
  return clock_gettime(CLOCK_REALTIME, &ts) != 0;
}
