#include <userpw.h>

int main()
{
  struct userpw *upw;

  upw = getuserpw("");
  puts(upw->upw_passwd);
}
