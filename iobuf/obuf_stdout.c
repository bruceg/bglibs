#include <unistd.h>
#include "obuf.h"

static char buffer[4096];

/** Output buffer for \c stdout */
obuf outbuf = { { 1, buffer, 4096, 0, 0, 0, 0, IOBUF_NEEDSCLOSE, 0 },
		0, 0, (obuf_fn)write };
