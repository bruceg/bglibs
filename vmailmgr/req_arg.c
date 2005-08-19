/* $Id$ */
#include "client.h"

int vclient_req_arg(vclient_req* vr, const str* arg)
{
  if (1 + vr->data.len + 2 + arg->len > VCLIENT_MAXLEN) return 0;
  if (!str_catc(&vr->data, (unsigned char)(arg->len >> 8))) return 0;
  if (!str_catc(&vr->data, (unsigned char)(arg->len))) return 0;
  if (!str_cat(&vr->data, arg)) return 0;
  ++vr->args;
  return 1;
}
