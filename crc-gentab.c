#include <string.h>

#include "sysdeps.h"
#include "iobuf/iobuf.h"
#include "msg/msg.h"

/* This generalized CRC table generator is derived from the descriptions in
 *   ftp://ftp.adelaide.edu.au/pub/rocksoft/crc_v3.txt
 * The above is the best and most comprehensive description I have seen
 * to date of CRC theory, math, algorithms, and code.
 */

const char program[] = "crc-gentab";
const int msg_show_pid = 0;

static int calc_columns(int bits, const char* suffix)
{
  int width = (bits+3)/4 + strlen(suffix) + 1;
  int cols = 78 / width;
  if (cols >= 16) return 16;
  if (cols >= 8) return 8;
  if (cols >= 4) return 4;
  if (cols >= 2) return 2;
  return 1;
}

static uint64 reflect(uint64 value, int bits)
{
  uint64 out = 0;
  while (bits-- > 0) {
    out <<= 1;
    if (value & 1)
      out |= 1;
    value >>= 1;
  }
  return out;
}

static uint64 crctab[256];

static void gentab(int bits, uint64 poly, int reflected)
{
  int i;
  uint64 topbit = 1ULL << (bits-1);
  uint64 mask = ~0ULL >> (64-bits);
  for (i = 0; i < 256; ++i) {
    uint64 crc = i;
    int j;
    if (reflected) crc = reflect(crc, 8);
    crc <<= bits - 8;
    for (j = 0; j < 8; ++j)
      crc = (crc << 1) ^ ((crc & topbit) ? poly : 0);
    if (reflected) crc = reflect(crc, bits);
    crctab[i] = crc & mask;
  }
}

int main(int argc, char* argv[])
{
  const char* bitstr;
  const char* polystr;
  int bits;
  int digits;
  uint64 poly;
  const char* suffix;
  int columns;
  char* end;
  int i;
  const char* name;
  int reflected;

  if (argc != 5) {
    msg3("usage: ", program, " NAME BITS POLY [normal|reflected]");
    return 1;
  }

  name = argv[1];

  bitstr = argv[2];
  bits = strtol(bitstr, &end, 0);
  if (*end != 0) die2(1, "Invalid bits value: ", bitstr);
  if (bits <= 0 || bits > 64) die1(1, "bits must be between 1 and 64");
  digits = (bits + 3) / 4;
  if (bits > 32) suffix = "ULL,";
  else if (bits > 16) suffix = "UL,";
  else suffix = "U,";
  columns = calc_columns(bits, suffix);

  polystr = argv[3];
  poly = strtoull(polystr, &end, 0);
  if (*end != 0) die2(1, "Invalid poly value: ", polystr);

  reflected = 0;
  if (strcasecmp(argv[4], "reflected") == 0)
    reflected = 1;
  else if (strcasecmp(argv[4], "normal") != 0)
    die2(1, "Must be either 'normal' or 'reflected': ", argv[4]);

  gentab(bits, poly, reflected);

  obuf_put3s(&outbuf, "#include \"", name, ".h\"\n\n");
  obuf_put5s(&outbuf, "const uint", bitstr, " ", name,
	     "_table[256] = {\n");
  for (i = 0; i < 256; ++i) {
    int column = i % columns;
    if (column == 0)
      obuf_puts(&outbuf, "  ");
    obuf_puts(&outbuf, "0x");
    if (bits > 32) {
      obuf_putxw(&outbuf, crctab[i]>>32, digits-8, '0');
      obuf_putxw(&outbuf, crctab[i], 8, '0');
    }
    else
      obuf_putxw(&outbuf, crctab[i], digits, '0');
    obuf_puts(&outbuf, suffix);
    obuf_putc(&outbuf, (column == (columns-1)) ? '\n' : ' ');
  }
  obuf_puts(&outbuf, "};\n");
  obuf_flush(&outbuf);
  return 0;
}
