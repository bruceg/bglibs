#include "iobuf.h"

static char buffer[4096];

obuf errbuf = { { 2, buffer, 4096, 0, 0, 0, 0, 0, 0, 0, 1 }, 0, 0 };
