#include "msg.h"

void die_oom(int code)
{
  die1(code, "Out of memory");
}
