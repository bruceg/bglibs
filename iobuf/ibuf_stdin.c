#include "iobuf.h"

static char buffer[4096];

ibuf inbuf = { { 0, buffer, 4096, 0, 0, 0, 0, 0, 0, 0 }, 0 };
