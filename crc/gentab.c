#include <string.h>

#include "sysdeps.h"
#include "iobuf/iobuf.h"
#include "msg/msg.h"

const char program[] = "crc/gentab";
const int msg_show_pid = 0;

int main(int argc, char* argv[])
{
  const char* bitstr;
  const char* polystr;
  int bits;
  int digits;
  uint64 poly;
  uint64 mask;
  const char* suffix;
  int columns;
  char* end;
  int i;

  if (argc != 4) {
    msg1("usage: crc/gentab bits rev-poly columns");
    return 1;
  }

  bitstr = argv[1];
  bits = strtol(bitstr, &end, 0);
  if (*end != 0) die2(1, "Invalid bits value: ", bitstr);
  if (bits <= 0 || bits > 64) die1(1, "bits must be between 1 and 64");
  digits = (bits + 3) / 4;
  mask = ~0ULL >> (64-bits);
  if (bits > 32) suffix = "ULL,";
  else if (bits > 16) suffix = "UL,";
  else suffix = "U,";

  polystr = argv[2];
  poly = strtoull(polystr, &end, 0);
  if (*end != 0) die2(1, "Invalid poly value: ", polystr);

  columns = strtol(argv[3], &end, 0);
  if (*end != 0 || columns < 1) die2(1, "Invalid columns value: ", argv[3]);

  obuf_put3s(&outbuf, "#include \"crc", bitstr, ".h\"\n\n");
  obuf_put5s(&outbuf, "const uint", bitstr, " crc", bitstr,
	     "_table[256] = {\n");
  for (i = 0; i < 256; ++i) {
    uint64 crc;
    int j;
    int column;
    crc = i;
    for (j = 0; j < 8; ++j) {
      unsigned bit = crc & 1;
      crc >>= 1;
      if (bit) crc ^= poly;
    }
    column = i % columns;
    if (column == 0)
      obuf_puts(&outbuf, "  ");
    obuf_puts(&outbuf, "0x");
    if (bits > 32) {
      obuf_putxw(&outbuf, crc>>32, digits-8, '0');
      obuf_putxw(&outbuf, crc, 8, '0');
    }
    else
      obuf_putxw(&outbuf, crc, digits, '0');
    obuf_puts(&outbuf, suffix);
    obuf_putc(&outbuf, (column == (columns-1)) ? '\n' : ' ');
  }
  obuf_puts(&outbuf, "};\n");
  obuf_flush(&outbuf);
  return 0;
}
