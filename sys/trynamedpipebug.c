#include <sys/types.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>

int main(void)
{
  const char filename[] = ".tmp.fifo";
  int hasbug;
  struct timeval tv;
  fd_set rfds;
  int fd;
  if (mknod(filename, S_IFIFO | 0600, 0) != 0)
    return 1;
  fd = open(filename, O_RDONLY | O_NDELAY);
  FD_ZERO(&rfds);
  FD_SET(fd,&rfds);
  tv.tv_sec = tv.tv_usec = 0;
  hasbug = select(fd+1, &rfds, 0, 0, &tv) > 0;
  unlink(filename);
  return hasbug ? 0 : 1;
}
