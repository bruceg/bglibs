#include "conf_home.c"
#include "install/installer.h"

#define C(SUB,FILE) c(SUB,#FILE,-1,-1,0644)
#define CF(SUB,FILE) cf(dir,#FILE,-1,-1,0644, #SUB "/" #FILE)
#define DI(SUB) dir = d(inc,#SUB,-1,-1,0755)

void insthier(void)
{
  const int home = d(0, conf_home, -1, -1, 0755);
  int bin;
  int inc;
  int lib;
  int dir;

  bin = d(home, "bin", -1, -1, 0755);

  inc = d(home, "include", -1, -1, 0755);

  cf(inc, "installer.h", -1, -1, 0644, "install/installer.h");
  C(inc, sysdeps.h);
  C(inc, systime.h);
  C(inc, uintnn.h);
  C(inc, uint16.h);
  C(inc, uint32.h);
  C(inc, uint64.h);
  
  DI(adt);
  CF(adt, common.h);
  CF(adt, ghash.h);
  CF(adt, gqueue.h);
  CF(adt, gstack.h);

  DI(base64);
  CF(base64, base64.h);

  DI(cdb);
  CF(cdb, cdb.h);
  CF(cdb, make.h);
  CF(cdb, str.h);

  DI(cli);
  CF(cli, cli.h);

  DI(crc);
  CF(crc, crc16_arc.h);
  CF(crc, crc16_ccitt.h);
  CF(crc, crc16_xmodem.h);
  CF(crc, crc32.h);
  CF(crc, crc32c.h);
  CF(crc, crc64.h);
  CF(crc, gcrc.h);

  DI(crypto);
  CF(crypto, md5.h);
  CF(crypto, sha1.h);
  CF(crypto, sha256.h);
  CF(crypto, sha384.h);
  CF(crypto, sha512.h);
  CF(crypto, surf.h);
  
  DI(dict);
  CF(dict, dict.h);
  CF(dict, load.h);
  
  DI(fmt);
  CF(fmt, number.h);

  DI(iobuf);
  CF(iobuf, iobuf.h);

  DI(misc);
  CF(misc, misc.h);
  
  DI(msg);
  CF(msg, msg.h);
  CF(msg, wrap.h);
  
  DI(net);
  CF(net, ipv4.h);
  CF(net, ipv6.h);
  CF(net, resolve.h);
  CF(net, socket.h);
  
  DI(path);
  CF(path, path.h);

  DI(pwcmp);
  CF(pwcmp, client.h);
  CF(pwcmp, hex.h);
  CF(pwcmp, module.h);
  
  DI(str);
  CF(str, iter.h);
  CF(str, str.h);
  
  DI(unix);
  CF(unix, nonblock.h);
  CF(unix, sig.h);
  
  DI(vmailmgr);
  CF(vmailmgr, client.h);
  CF(vmailmgr, vpwentry.h);
  
  lib = d(home, "lib", -1, -1, 0755);

  c(lib, "libbg.a", -1, -1, 0644);

  c(lib, "libbg-sysdeps.a", -1, -1, 0644);

  cf(lib, "libbg-installer.a", -1, -1, 0644, "install/installer.a");
  cf(lib, "libbg-instcheck.a", -1, -1, 0644, "install/instcheck.a");
  cf(lib, "libbg-instshow.a", -1, -1, 0644,  "install/instshow.a");

#undef L
#define L(BASE) do{ \
  cf(lib, "libbg-" #BASE ".a", -1, -1, 0644, #BASE "/lib.a"); \
}while(0)

  L(adt);
  L(base64);
  L(cdb);
  L(cli);
  L(crc);
  L(crypto);
  L(dict);
  L(fmt);
  L(iobuf);
  L(misc);
  L(msg);
  L(net);
  L(path);
  cf(lib, "libpwcmp.a",        -1, -1, 0644, "pwcmp/client.a");
  cf(lib, "libpwcmp-module.a", -1, -1, 0644, "pwcmp/module.a");
  L(str);
  L(unix);
  cf(lib, "libvmailmgr.a",     -1, -1, 0644, "vmailmgr/lib.a");

#define DL(SUB) dir = d(lib, "lib" #SUB,-1,-1,0755);

#define SL(BASE,LIB) s(dir, #LIB ".a", "../libbg-" #BASE ".a")

  /* Compatibility links */
  s(lib, "libsysdeps.a",   "libbg-sysdeps.a");
  s(lib, "libinstaller.a", "libbg-installer.a");
  s(lib, "libinstcheck.a", "libbg-instcheck.a");
  s(lib, "libinstshow.a",  "libbg-instshow.a");

  DL(base64);
  SL(base64, base64);

  DL(cdb);
  SL(cdb, cdb);
  SL(cdb, make);
  SL(cdb, str);

  DL(cli);
  SL(cli, cli);

  DL(crypto);
  SL(crypto, md5);
  SL(crypto, sha1);
  SL(crypto, sha256);
  SL(crypto, sha512a);

  DL(dict);
  SL(dict, dict);
  SL(dict, load);
  
  DL(iobuf);
  SL(iobuf, iobuf);
  SL(iobuf, str);
  
  DL(misc);
  SL(misc, misc);
  
  DL(msg);
  SL(msg, msg);
  SL(msg, wrap);
  
  DL(net);
  SL(net, ipv4);
  SL(net, resolve);
  SL(net, socket);
  
  DL(path);
  SL(path, path);

  DL(pwcmp);
  s(dir, "client.a", "../libpwcmp.a");
  s(dir, "hex.a",    "../libpwcmp.a");
  s(dir, "module.a", "../libpwcmp-module.a");
  
  DL(str);
  SL(str, iter);
  SL(str, str);
  
  DL(unix);
  SL(unix, nonblock);
  SL(unix, sig);
  
  DL(vmailmgr);
  s(dir, "client.a",   "../libvmailmgr.a");
  s(dir, "vpwentry.a", "../libvmailmgr.a");
}
