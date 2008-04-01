#include <sys/sendfile.h>
#include <asm/unistd.h>
#include <unistd.h>

void main(void) {
  int x;
  x = __NR_sendfile;
  sendfile(0, 1, 0, 0);
}
