#include <sys/types.h>
#include <sys/wait.h>

int main()
{
  waitpid(0,0,0);
}
