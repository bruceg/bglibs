#include <stdlib.h>
#include "cvm-sasl.h"
#include "internal.h"

static sasl_mechanism mechanisms[4] = {
  { "LOGIN",    "CVM_SASL_LOGIN",    0,
    sasl_login_start,    sasl_login_response,    0 },
  { "PLAIN",    "CVM_SASL_PLAIN",    0,
    sasl_plain_start,    sasl_plain_response,    0 },
  { "CRAM-MD5", "CVM_SASL_CRAM_MD5", 0,
    sasl_cram_md5_start, sasl_cram_md5_response, 0 },
  { 0,           0,                  0,
    0,                   0,                      0 },
};

const sasl_mechanism* sasl_mechanisms;
const char* sasl_domain = 0;

int sasl_init(void)
{
  sasl_mechanism* prev;
  sasl_mechanism* first;
  sasl_mechanism* mech;
  const char* tmp;
  for (mech = mechanisms, first = 0, prev = 0; mech->name != 0; ++mech) {
    if ((tmp = getenv(mech->var)) != 0) {
      mech->cvm = tmp;
      if (prev != 0)
	prev->next = mech;
      else if (first == 0)
	prev = first = mech;
    }
  }
  sasl_mechanisms = first;
  sasl_domain = getenv("TCPLOCALHOST");
  return 1;
}
