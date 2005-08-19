/* $Id$ */
#include <string.h>
#include "vpwentry.h"

void vpwentry_free(vpwentry* vpw)
{
  str_free(&vpw->name);
  str_free(&vpw->pass);
  str_free(&vpw->directory);
  str_free(&vpw->forwards);
  str_free(&vpw->personal);
  memset(vpw, 0, sizeof *vpw);
}
