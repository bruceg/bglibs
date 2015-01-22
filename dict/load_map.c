#include <stdlib.h>
#include <string.h>
#include "iobuf.h"
#include "str.h"
#include "dict.h"

int dict_load_map(dict* d, const char* filename, int mustexist, char sep,
		  int (*keyxform)(str*), int (*valxform)(str*))
{
  ibuf in;
  unsigned i;
  str tmp = {0,0,0};
  int result = 1;

  if (!dict_init(d)) return 0;
  if (!ibuf_open(&in, filename, 0)) return !mustexist;
  while (ibuf_getstr(&in, &tmp, '\n')) {
    str_strip(&tmp);
    if (tmp.len > 0 && tmp.s[0] != '#' &&
	(i = str_findfirst(&tmp, sep)) != (unsigned)-1) {
      str* val;
      if ((val = malloc(sizeof *val)) == 0) { result = 0; break; }
      memset(val, 0, sizeof *val);
      if (!str_copyb(val, tmp.s+i+1, tmp.len-i-1) ||
	  (valxform != 0 && !valxform(val)) ||
	  !str_truncate(&tmp, i) ||
	  (keyxform != 0 && !keyxform(&tmp)) ||
	  !dict_add(d, &tmp, val)) {
	str_free(val);
	free(val);
	result = 0;
	break;
      }
    }
  }
  str_free(&tmp);
  ibuf_close(&in);
  return result;
}
