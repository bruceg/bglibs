#include "path.h"
#include "iobuf/iobuf.h"

static str path;
static str entries;

static void nl(void) { obuf_putsflush(&outbuf, "\n"); }
void showpath(void)
{
  obuf_putstr(&outbuf, &path); nl();
}

int main(void)
{
  str_copys(&path, "/");
  path_merge(&path, "a"); showpath();
  path_merge(&path, "b"); showpath();
  path_merge(&path, "../c"); showpath();
  path_merge(&path, "/d"); showpath();
  str_copys(&path, "");
  path_merge(&path, "a"); showpath();
  path_merge(&path, "b"); showpath();
  path_merge(&path, "../c"); showpath();
  path_merge(&path, "/d"); showpath();
  obuf_puti(&outbuf, path_match("str/*.c", &entries)); nl();
  obuf_write(&outbuf, entries.s, entries.len); nl();
  return 0;
}
