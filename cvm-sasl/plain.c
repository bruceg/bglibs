#include <string.h>
#include "cvm-sasl.h"
#include "internal.h"

int sasl_plain_start(const str* response, str* challenge)
{
  if (response) return sasl_plain_response(response, challenge);
  if (!str_truncate(challenge, 0)) return SASL_TEMP_FAIL;
  return SASL_CHALLENGE;
}

int sasl_plain_response(const str* response, str* challenge)
{
  const char* creds[2];
  unsigned i;
  unsigned j;
  if (response->len == 0) return SASL_RESP_BAD;
  if ((i = str_findfirst(response, 0)) == (unsigned)-1 ||
      (j = str_findnext(response, 0, ++i)) == (unsigned)-1)
    return SASL_RESP_BAD;
  ++j;
  if (i != j-i || memcmp(response->s, response->s+i, i) != 0)
    return SASL_RESP_BAD;
  creds[0] = response->s+j;
  creds[1] = 0;
  return sasl_authenticate(response->s+i, creds);
  (void)challenge;
}
