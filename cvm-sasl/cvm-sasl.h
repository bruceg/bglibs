#ifndef CVM_SASL__H__
#define CVM_SASL__H__

#include "str/str.h"

typedef int (*saslfn)(const str* response, str* challenge);

struct sasl_mechanism 
{
  const char* name;
  const char* var;
  const char* cvm;
  saslfn start;
  saslfn response;
  struct sasl_mechanism* next;
};
typedef struct sasl_mechanism sasl_mechanism;

extern const sasl_mechanism* sasl_mechanisms;
extern const char* sasl_domain;
extern int sasl_init(void);
extern int sasl_start(const char* mechanism, const str* initresponse,
		      str* challenge);
extern saslfn sasl_response;

#define SASL_AUTH_OK 0		/* Authentication is complete */
#define SASL_AUTH_FAILED 1	/* Authentication failed (permanently) */
#define SASL_NO_MECH 2		/* Invalid or unknown mechanism name */
#define SASL_TEMP_FAIL 3	/* Temporary or internal failure */
#define SASL_CHALLENGE 4	/* Send challenge to client, wait for response */
#define SASL_RESP_REQUIRED 5	/* A response was required but not given */
#define SASL_RESP_NOTALLOWED 6	/* A (initial) response was given but not allowed */
#define SASL_RESP_BAD 7		/* The response was invalid */

#endif
