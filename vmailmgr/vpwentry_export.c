#include "vpwentry.h"
#include "str.h"

static int str_catunul(str* result, unsigned long u)
{
  if (u == (unsigned)-1)
    return str_catb(result, "-", 2);
  return str_catu(result, u) && str_catc(result, 0);
}

int vpwentry_export(const vpwentry* vpw, str* result)
{
  char flagstmp[] = {
    2,
    VPWENTRY_HAS_MAILBOX, vpw->has_mailbox && (vpw->directory.len > 0),
    VPWENTRY_MAILBOX_ENABLED, vpw->is_mailbox_enabled,
    0
  };
  if (!str_copyb(result, flagstmp, 6)) return 0;
  if (!str_catb(result, vpw->pass.s, vpw->pass.len+1)) return 0;
  if (!str_catb(result, vpw->directory.s, vpw->directory.len+1)) return 0;
  if (vpw->forwards.len > 0)
    if (!str_catb(result, vpw->forwards.s, vpw->forwards.len+1)) return 0;
  if (!str_catc(result, 0)) return 0;
  if (!str_catb(result, vpw->personal.s, vpw->personal.len+1)) return 0;

  if (!str_catunul(result, vpw->hardquota)) return 0;
  if (!str_catunul(result, vpw->softquota)) return 0;
  if (!str_catunul(result, vpw->msgsize)) return 0;
  if (!str_catunul(result, vpw->msgcount)) return 0;
  if (!str_catunul(result, vpw->ctime)) return 0;
  if (!str_catunul(result, vpw->expiry)) return 0;

  return 1;
}
