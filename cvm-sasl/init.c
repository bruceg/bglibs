#include <stdlib.h>
#include "cvm-sasl.h"
#include "internal.h"

static sasl_mechanism mechanisms[4] = {
  { "LOGIN",    "CVM_SASL_PLAIN",    0,
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

static void set_mech(sasl_mechanism* mech, const char* cvm,
		     sasl_mechanism** prev, sasl_mechanism** first)
{
  mech->cvm = cvm;
  if (*prev != 0)
    (*prev)->next = mech;
  else if (*first == 0)
    *prev = *first = mech;
}

int sasl_init(void)
{
  sasl_mechanism* prev;
  sasl_mechanism* first;
  sasl_mechanism* mech;
  const char* tmp;
  for (mech = mechanisms, first = 0, prev = 0; mech->name != 0; ++mech) {
    if ((tmp = getenv(mech->var)) != 0)
      set_mech(mech, tmp, &prev, &first);
  }
  /* backwards compatibility for $CVM_SASL_LOGIN */
  if (!mechanisms[0].cvm && (tmp = getenv("CVM_SASL_LOGIN")) != 0) {
    prev = 0;
    first = 0;
    set_mech(&mechanisms[0], tmp, &prev, &first);
  }
  sasl_mechanisms = first;
  sasl_domain = getenv("TCPLOCALHOST");
  return 1;
}
