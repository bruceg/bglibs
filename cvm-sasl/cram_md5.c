#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "systime.h"
#include "cvm-sasl.h"
#include "internal.h"

static str init;

int sasl_cram_md5_start(const str* response, str* challenge)
{
  struct timeval tv;
  const char* hostname;
  
  if (response) return SASL_RESP_NOTALLOWED;
  if ((hostname = getenv("TCPLOCALHOST"))) hostname = "unknown";
  if (gettimeofday(&tv, 0) == -1 ||
      !str_copys(&init, "<") ||
      !str_cati(&init, getpid()) ||
      !str_catc(&init, '.') ||
      !str_catu(&init, tv.tv_sec) ||
      !str_catc(&init, '.') ||
      !str_catuw(&init, tv.tv_usec, 6, '0') ||
      !str_catc(&init, '@') ||
      !str_cats(&init, hostname) ||
      !str_catc(&init, '>') ||
      !str_copy(challenge, &init)) return SASL_TEMP_FAIL;
  return SASL_CHALLENGE;
}

int sasl_cram_md5_response(const str* response, str* challenge)
{
  const char* creds[3];
  unsigned i;
  if (response->len == 0) return SASL_RESP_BAD;
  if ((i = str_findfirst(response, ' ')) == (unsigned)-1) return SASL_RESP_BAD;
  response->s[i] = 0;
  creds[0] = init.s;
  creds[1] = response->s+i+1;
  creds[2] = 0;
  return sasl_authenticate(response->s, creds);
  (void)challenge;
}
