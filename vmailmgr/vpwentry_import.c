#include <ctype.h>
#include <limits.h>
#include <string.h>
#include "systime.h"
#include "misc/misc.h"
#include "vpwentry.h"

static const char* from_uint(const char* ptr, unsigned* u)
{
  const char* tmp;
  if (ptr[0] == '-' && ptr[1] == 0) {
    *u = UINT_MAX;
    return ptr + 2;
  }
  *u = strtou(ptr, &tmp);
  return (*tmp == 0) ? ++tmp : 0;
}

static const char* import_flags(vpwentry* vpw,
				const char* ptr, const char* end)
{
  while (ptr < end) {
    unsigned flag;
    unsigned value;
    if ((flag = *(unsigned char*)ptr++) == 0) return ptr;
    if (ptr >= end) return 0;
    value = *(unsigned char*)ptr++;
    switch(flag) {
    case 10: /* vdomain::ATTR_HAS_MAILBOX */
      vpw->has_mailbox = value; break;
    case 8: /* vdomain::ATTR_MAILBOX_ENABLED */
      vpw->is_mailbox_enabled = value; break;
    default:
      return 0;
    }
  }
  return 0;
}

static const char* import_base(vpwentry* vpw, const char* ptr, const char* end)
{
  const char* start;
  
  str_copys(&vpw->pass, ptr);
  ptr += vpw->pass.len + 1;
  if (ptr >= end) return 0;
  
  str_copys(&vpw->directory, ptr);
  vpw->has_mailbox = (vpw->directory.len != 0);
  ptr += vpw->directory.len + 1;
  if (ptr >= end) return 0;

  start = ptr;
  while (ptr < end && *ptr != 0)
    ptr += strlen(ptr) + 1;
  if (ptr == start)
    str_truncate(&vpw->forwards, 0);
  else
    str_copyb(&vpw->forwards, start, ptr-start-1);
  if (ptr++ >= end) return 0;
  
  str_copys(&vpw->personal, ptr);
  ptr += vpw->personal.len + 1;
  if (ptr >= end) return 0;
  
  return ptr;
}

static const char* import_values(vpwentry* vpw,
				 const char* ptr, const char* end)
{
  if ((ptr = from_uint(ptr, &vpw->hardquota)) == 0 || ptr >= end) return 0;
  if ((ptr = from_uint(ptr, &vpw->softquota)) == 0 || ptr >= end) return 0;
  if ((ptr = from_uint(ptr, &vpw->msgsize)) == 0 || ptr >= end) return 0;
  if ((ptr = from_uint(ptr, &vpw->msgcount)) == 0 || ptr >= end) return 0;
  if ((ptr = from_uint(ptr, &vpw->ctime)) == 0 || ptr >= end) return 0;
  if ((ptr = from_uint(ptr, &vpw->expiry)) == 0 || ptr > end) return 0;
  return ptr;
}

static void set_defaults(vpwentry* vpw, int ctime_now, int set_flags)
{
  /* FIXME: These need to be loaded from config files */
  vpw->softquota = vpw->hardquota = vpw->msgsize = vpw->msgcount = 0;
  vpw->expiry = UINT_MAX;
  if (ctime_now)
    vpw->ctime = time(0);
  else
    vpw->ctime = 0;
  if(set_flags) {
    vpw->is_mailbox_enabled = 1;
  }
}

static int import_old(vpwentry* vpw, const str* data)
{
  const char* end;

  if ((end = strchr(data->s+1, ':')) == 0) return 0;
  str_copyb(&vpw->pass, data->s+1, end-data->s-1);
  ++end;

  if(*end == '.' || *end == '/') {
    vpw->has_mailbox = 1;
    str_copys(&vpw->directory, end);
    str_truncate(&vpw->forwards, 0);
  }
  else {
    if(*end == '&')
      ++end;
    vpw->has_mailbox = 0;
    str_truncate(&vpw->directory, 0);
    str_copys(&vpw->forwards, end);
    str_subst(&vpw->forwards, ',', 0);
  }
  set_defaults(vpw, 0, 1);
  return 1;
}

static int import_ver1(vpwentry* vpw, const str* data)
{
  const char* ptr;
  const char* end;
  
  // Sanity check to ensure that text conversions can't
  // fall off the end of the string.
  if (data->s[data->len-1] != 0) return 0;

  ptr = data->s;
  end = ptr++ + data->len;

  if ((ptr = import_base(vpw, ptr, end)) == 0) return 0;
  if ((ptr = import_values(vpw, ptr, end)) == 0) return 0;
  
  // Set defaults
  vpw->is_mailbox_enabled = 1;
  
  return ptr == end;
}

static int import_ver2(vpwentry* vpw, const str* data)
{
  const char* ptr;
  const char* end;
  
  // Sanity check to ensure that text conversions can't
  // fall off the end of the string.
  if (data->s[data->len-1] != 0) return 0;

  ptr = data->s;
  end = ptr++ + data->len;

  if((ptr = import_flags(vpw, ptr, end)) == 0) return 0;
  if((ptr = import_base(vpw, ptr, end)) == 0) return 0;
  if((ptr = import_values(vpw, ptr, end)) == 0) return 0;
  
  return ptr == end;
}

int vpwentry_import(vpwentry* vpw, const str* name, const str* data)
{
  str_copy(&vpw->name, name);
  switch (data->s[0]) {
  case 2: return import_ver2(vpw, data);
  case 1: return import_ver1(vpw, data);
  case ':': return import_old(vpw, data);
  default: return 0;
  }
}
