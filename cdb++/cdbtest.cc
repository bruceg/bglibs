#include "cdb++.h"

int main()
{
  cdb_reader r("cdb.cdb");
  cdb_writer w("cdb.temp");
  datum* d;
  while((d = r.nextrec()) != 0) {
    datum* t = cdb_getrec("cdb.cdb", d->key);
    if(t) delete t;
    w.put(d->key, d->data);
  }
  w.end("cdb.cdb");
  return 0;
}
