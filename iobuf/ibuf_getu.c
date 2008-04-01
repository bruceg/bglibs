#include "ibuf.h"

/** Read an unsigned long from the \c ibuf */
int ibuf_getu(ibuf* in, unsigned long* data)
{
  char ch;
  int chars;
  
  *data = 0;
  chars = 0;
  while (ibuf_peek(in, &ch) && ch >= '0' && ch <= '9') {
    *data = (*data * 10) + ch - '0';
    ibuf_getc(in, &ch);
    chars = 1;
  }
  return chars;
}
