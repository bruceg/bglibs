#include <errno.h>
#include "msg.h"

const char program[] = "testprog";

int main(void) {
  warn2("a", "b");
  errno = EAGAIN;
  die3sys(123, "c", "d", "e");
  return 0;
}
