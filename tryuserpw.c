#include <userpw.h>

void main()
{
  struct userpw *upw;

  upw = getuserpw("");
  puts(upw->upw_passwd);
}
