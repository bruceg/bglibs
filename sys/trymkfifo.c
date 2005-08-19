/* $Id$ */
#include <sys/types.h>
#include <sys/stat.h>

int main(void)
{
  mkfifo("nothing", 0);
}
