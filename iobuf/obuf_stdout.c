#include "iobuf.h"

static char buffer[4096];

obuf outbuf = { { 1, buffer, 4096, 0, 0, 0, 0, 0, 0, 0 }, 0, 0 };
