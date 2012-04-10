#include <stdlib.h>
#include "bglibs/str.h"
#include "dict.h"

void dict_str_free(void* v)
{
  str* s = (str*)v;
  str_free(s);
  free(s);
}
