#include <string.h>
#include "iobuf.h"
#include "msg/msg.h"
#include "msg/wrap.h"
#include "path/path.h"
#include "str.h"

const char program[] = "selftest-cmp";

static int str_diff_fnmatch(const str* a, const str* b)
{
  return !fnmatch(a->s, b->s, 0);
}

static int getline(ibuf* in, str* line, const char* src)
{
  int i = ibuf_getstr(in, line, LF);
  if (!i && ibuf_error(in))
    die2sys(111, "Error reading from ", src);
  return i;
}

static void escape(const str* in, str* out)
{
  unsigned int i;
  unsigned int j;
  int ch;
  char buf[7*in->len];

  for (i = j = 0; i < in->len; i++) {
    ch = (unsigned char)in->s[i];
    if (ch < 32 && ch != '\t' && ch != '\n') {
      buf[j++] = '^';
      buf[j++] = ch + 64;
    }
    else if (ch == 127) {
      buf[j++] = '^';
      buf[j++] = '?';
    }
    else if (ch >= 128) {
      buf[j++] = 'M';
      buf[j++] = '-';
      buf[j++] = (ch >= 192) ? 'C' : 'B';
      buf[j++] = 'M';
      buf[j++] = '-';
      ch &= 0x3f;
      if (ch < 32) {
	buf[j++] = '^';
	ch += 64;
      }
      buf[j++] = ch;
    }
    else
      buf[j++] = ch;
  }
  wrap_str(str_copyb(out, buf, j));
}

int main(int argc, char* argv[])
{
  ibuf srcbuf;
  ibuf tstbuf;
  str sline = {0};
  str tline = {0};
  str xline = {0};
  int lineno = 0;
  int (*cmp)(const str*, const str*) = str_diff;

  if (argc != 3)
    die1(111, "Usage: selftest-cmp SOURCE TEST-RESULT");
  if (!ibuf_open(&srcbuf, argv[1], 0))
    diefsys(111, "{Could not open }s", argv[1]);
  if (!ibuf_open(&tstbuf, argv[2], 0))
    diefsys(111, "{Could not open }s", argv[2]);
  
  while (ibuf_getstr(&srcbuf, &sline, LF)) {
    if (str_starts(&sline, "#ifdef SELFTEST_EXP")) {
      if (str_starts(&sline, "#ifdef SELFTEST_EXP_FNMATCH"))
	cmp = str_diff_fnmatch;
      break;
    }
  }
  if (ibuf_error(&srcbuf))
    die1sys(111, "Error reading from source");
  while (getline(&srcbuf, &sline, "source")) {
    if (str_starts(&sline, "#endif"))
      break;
    if (!getline(&tstbuf, &tline, "test result"))
      die1(1, "EOF on test result");
    escape(&tline, &xline);
    ++lineno;
    if (cmp(&xline, &sline)) {
      obuf_putf(&outbuf, "d{: }S", xline.len, &xline);
      obuf_putf(&outbuf, "d{: }S", sline.len, &sline);
      obuf_flush(&outbuf);
      dief(1, "{Line #}d{ mismatch}", lineno);
    }
  }
  if (getline(&tstbuf, &tline, "test result"))
    die1(1, "EOF on source");
  return 0;
}
