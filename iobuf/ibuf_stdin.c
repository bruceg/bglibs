#include <unistd.h>
#include "iobuf.h"

static char buffer[4096];

/** Input buffer for \c stdin */
ibuf inbuf = { { 0, buffer, 4096, 0, 0, 0, 0, IOBUF_NEEDSCLOSE, 0 },
	       0, (ibuf_fn)read };
