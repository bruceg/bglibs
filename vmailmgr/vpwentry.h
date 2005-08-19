/* $Id$ */
#ifndef VMAILMGR__VPWENTRY__H__
#define VMAILMGR__VPWENTRY__H__

#include "str/str.h"

struct vpwentry 
{
  str name;
  str pass;
  str directory;
  str forwards;
  str personal;
  unsigned hardquota;
  unsigned softquota;
  unsigned msgsize;
  unsigned msgcount;
  unsigned ctime;
  unsigned expiry;
  int has_mailbox;
  int is_mailbox_enabled;
};
typedef struct vpwentry vpwentry;

int vpwentry_import(vpwentry* vpw, const str* name, const str* data);
int vpwentry_export(const vpwentry* vpw, str* result);
void vpwentry_free(vpwentry* vpw);

#define VPWENTRY_HAS_MAILBOX 10
#define VPWENTRY_MAILBOX_ENABLED 8

#endif
