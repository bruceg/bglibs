#include <sys/types.h>
#include <fcntl.h>
#include <poll.h>

int main()
{
  struct pollfd x;

  x.fd = open("sys/trypoll.c",O_RDONLY);
  if (x.fd == -1) _exit(111);
  x.events = POLLIN;
  if (poll(&x,1,10) == -1) _exit(1);
  if (x.revents != POLLIN) _exit(1);

  _exit(0);
}
