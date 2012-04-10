#include <string.h>
#include "fmt.h"

/** Format a pad character. */
unsigned fmt_pad(char* buffer, unsigned width, char pad)
{
  if (buffer != 0)
    memset(buffer, pad, width);
  return width;
}
