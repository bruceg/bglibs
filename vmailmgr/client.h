/* $Id$ */
#ifndef VMAILMGR__CLIENT__H__
#define VMAILMGR__CLIENT__H__

#include "str/str.h"

#define VCLIENT_MAXLEN 65535
struct vclient_req
{
  unsigned args;
  str data;
};
typedef struct vclient_req vclient_req;

struct vclient_resp
{
  unsigned code;
  str message;
};
typedef struct vclient_resp vclient_resp;

int vclient_req_init(vclient_req* vr, const str* cmd);
int vclient_req_arg(vclient_req* vr, const str* arg);
int vclient_req_write(const vclient_req* vr, int fd);
int vclient_resp_read(vclient_resp* vr, int fd);

#endif
