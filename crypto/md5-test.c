#include <string.h>
#include "md5.h"

static void MDString(const char* str)
{
  struct md5_ctx ctx;
  unsigned char digest[16];
  unsigned int len = strlen(str);
  unsigned i;
  
  md5_init_ctx(&ctx);
  md5_process_bytes(str, len, &ctx);
  md5_finish_ctx(&ctx, digest);
  printf("MD5 (\"%s\") = ", str);
  for (i = 0; i < 16; i++) printf("%02x", digest[i]);
  printf("\n");
}

int main(void)
{
  printf("MD5 test suite:\n");
  MDString("");
  MDString("a");
  MDString("abc");
  MDString("message digest");
  MDString("abcdefghijklmnopqrstuvwxyz");
  MDString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
  
  MDString("1234567890123456789012345678901234567890"
	   "1234567890123456789012345678901234567890");
  return 0;
}
