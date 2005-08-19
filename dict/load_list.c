/* $Id$ */
#include "iobuf/iobuf.h"
#include "str/str.h"
#include "dict.h"
#include "load.h"

int dict_load_list(dict* d, const char* filename, int mustexist,
		   int (*xform)(str*))
{
  ibuf in;
  str tmp = {0,0,0};
  int result = 1;
  
  if (!dict_init(d)) return 0;
  if (!ibuf_open(&in, filename, 0)) return !mustexist;
  while (ibuf_getstr(&in, &tmp, '\n')) {
    str_strip(&tmp);
    if (tmp.len > 0 && tmp.s[0] != '#') {
      if (xform != 0) if (!xform(&tmp)) { result = 0; break; }
      if (!dict_add(d, &tmp, 0)) { result = 0; break; }
    }
  }
  str_free(&tmp);
  ibuf_close(&in);
  return result;
}
