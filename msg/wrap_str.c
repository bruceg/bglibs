#include "msg.h"
#include "wrap.h"

void wrap_str(int result)
{
  if (!result)
    die_oom(wrap_exit);
}
