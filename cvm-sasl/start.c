#include <string.h>
#include "cvm-sasl.h"
#include "internal.h"

saslfn sasl_response = 0;
const char* sasl_mech_cvm = 0;

int sasl_start(const char* mechanism, const str* initresponse, str* challenge)
{
  sasl_mechanism* mech;
  for (mech = sasl_mechanisms; mech != 0; mech = mech->next) {
    if (strcasecmp(mechanism, mech->name) == 0) {
      sasl_mech_cvm = mech->cvm;
      sasl_response = mech->response;
      return mech->start(initresponse, challenge);
    }
  }
  return SASL_NO_MECH;
}
