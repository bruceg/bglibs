#include <unistd.h>
#include "iobuf.h"

static char buffer[4096];

obuf errbuf = { { 2, buffer, 4096, 0, 0, 0, 0, IOBUF_NEEDSCLOSE, 0 },
		0, 0, (obuf_fn)write };
