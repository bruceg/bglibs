#include <shadow.h>

void main()
{
  struct spwd *spw;

  spw = getspnam("");
  puts(spw->sp_pwdp);
}
