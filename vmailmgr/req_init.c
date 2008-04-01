#include "client.h"

int vclient_req_init(vclient_req* vr, const str* cmd)
{
  if (1 + 2 + cmd->len > VCLIENT_MAXLEN) return 0;
  if (!str_truncate(&vr->data, 0)) return 0;
  if (!vclient_req_arg(vr, cmd)) return 0;
  vr->args = 0;
  return 1;
}
