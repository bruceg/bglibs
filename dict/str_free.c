#include <stdlib.h>
#include "str/str.h"
#include "dict.h"

void dict_str_free(void* v)
{
  str* s = (str*)v;
  str_free(s);
  free(s);
}
