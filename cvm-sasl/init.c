#include <stdlib.h>
#include "cvm-sasl.h"
#include "internal.h"

sasl_mechanism sasl_mechanisms[4] = {
  { "LOGIN",    0, sasl_login_start,    sasl_login_response    },
  { "PLAIN",    0, sasl_plain_start,    sasl_plain_response    },
  { "CRAM_MD5", 0, sasl_cram_md5_start, sasl_cram_md5_response },
  { 0,          0, 0,                   0,                     },
};

const char* sasl_domain = 0;

int sasl_init(void)
{
  const char* tmp;
  if ((tmp = getenv("SASL_CVM_LOGIN")) != 0)    sasl_mechanisms[0].cvm = tmp;
  if ((tmp = getenv("SASL_CVM_PLAIN")) != 0)    sasl_mechanisms[1].cvm = tmp;
  if ((tmp = getenv("SASL_CVM_CRAM_MD5")) != 0) sasl_mechanisms[2].cvm = tmp;
  sasl_domain = getenv("TCPLOCALHOST");
  return 1;
}
