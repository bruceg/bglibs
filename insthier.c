#include "conf_home.c"
#include "install/installer.h"

#define C(SUB,FILE) c(SUB,#FILE,-1,-1,0644)
#define CF(SUB,FILE) cf(dir,#FILE,-1,-1,0644, #SUB "/" #FILE)
#define DI(SUB) dir = d(inc,#SUB,-1,-1,0755)
#define DL(SUB) dir = d(lib, "lib" #SUB,-1,-1,0755);

void insthier(void)
{
  int home = opendir(conf_home);
  int bin;
  int inc;
  int lib;
  int dir;

  bin = d(home, "bin", -1, -1, 0755);

  inc = d(home, "include", -1, -1, 0755);

  cf(inc, "installer.h", -1, -1, 0644, "install/installer.h");
  C(inc, sysdeps.h);
  C(inc, systime.h);
  C(inc, uint16.h);
  C(inc, uint32.h);
  C(inc, uint64.h);
  
  DI(adt);
  CF(adt, ghash.h);
  
  DI(base64);
  CF(base64, base64.h);

  DI(cdb);
  CF(cdb, cdb.h);
  CF(cdb, make.h);
  CF(cdb, str.h);

  DI(cli);
  CF(cli, cli.h);

  DI(crc);
  CF(crc, crc32.h);
  CF(crc, crc64.h);

  DI(crypto);
  CF(crypto, md5.h);
  CF(crypto, sha1.h);
  CF(crypto, sha256.h);
  CF(crypto, sha512a.h);
  
  DI(cvm);
  CF(cvm, client.h);
  CF(cvm, errors.h);
  CF(cvm, facts.h);
  CF(cvm, module.h);
  
  DI(cvm-sasl);
  CF(cvm-sasl, cvm-sasl.h);
  
  DI(dict);
  CF(dict, dict.h);
  CF(dict, load.h);
  
  DI(iobuf);
  CF(iobuf, iobuf.h);

  DI(misc);
  CF(misc, misc.h);
  
  DI(msg);
  CF(msg, msg.h);
  CF(msg, wrap.h);
  
  DI(net);
  CF(net, ipv4.h);
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

#define L(BASE) do{ \
  c(lib, "lib" #BASE ".a", -1, -1, 0644); \
  s(lib, #BASE, "lib" #BASE ".a"); \
}while(0)

#define SL(BASE,LIB) s(dir, #LIB ".a", "../lib" #BASE ".a")

#define CL(SUB,BASE,LINK) do{ \
  cf(dir, #BASE ".a", -1, -1, 0644, #SUB "/" #BASE ".a"); \
  s(lib, #LINK, "lib" #SUB "/" #BASE ".a"); \
}while(0)

  L(sysdeps);

#undef L
#define L(BASE) do{ \
  cf(lib, "lib" #BASE ".a", -1, -1, 0644, "install/" #BASE ".a"); \
  s(lib, #BASE, "lib" #BASE ".a"); \
}while(0)

  L(installer);
  L(instcheck);
  L(instshow);

#undef L
#define L(BASE) do{ \
  cf(lib, "lib" #BASE ".a", -1, -1, 0644, #BASE "/lib.a"); \
  s(lib, #BASE, "lib" #BASE ".a"); \
}while(0)

  L(adt);

  L(base64);
  DL(base64);
  SL(base64, base64);

  L(cdb);
  DL(cdb);
  SL(cdb, cdb);
  SL(cdb, make);
  SL(cdb, str);

  L(cli);
  DL(cli);
  SL(cli, cli);

  L(crc);

  L(crypto);
  DL(crypto);
  SL(crypto, md5);
  SL(crypto, sha1);
  SL(crypto, sha256);
  SL(crypto, sha512a);

  cf(lib, "libcvm-client.a",  -1, -1, 0644, "cvm/client.a");
  cf(lib, "libcvm-command.a", -1, -1, 0644, "cvm/command.a");
  cf(lib, "libcvm-local.a",   -1, -1, 0644, "cvm/local.a");
  cf(lib, "libcvm-udp.a",     -1, -1, 0644, "cvm/udp.a");
  DL(cvm);
  SL(cvm-client,  client);
  SL(cvm-command, command);
  SL(cvm-local,   local);
  SL(cvm-udp,     udp);
  s(lib, "cvm-client",  "libcvm-client.a");
  s(lib, "cvm-command", "libcvm-command.a");
  s(lib, "cvm-local",   "libcvm-local.a");
  s(lib, "cvm-udp",     "libcvm-udp.a");

  L(cvm-sasl);
  DL(cvm-sasl);
  SL(cvm-sasl, cvm-sasl);

  L(dict);
  DL(dict);
  SL(dict, dict);
  SL(dict, load);
  
  L(iobuf);
  DL(iobuf);
  SL(iobuf, iobuf);
  SL(iobuf, str);
  
  L(misc);
  DL(misc);
  SL(misc, misc);
  
  L(msg);
  DL(msg);
  SL(msg, msg);
  SL(msg, wrap);
  
  L(net);
  DL(net);
  SL(net, ipv4);
  SL(net, resolve);
  SL(net, socket);
  
  L(path);
  DL(path);
  SL(path, path);

  cf(lib, "libpwcmp.a",        -1, -1, 0644, "pwcmp/client.a");
  cf(lib, "libpwcmp-module.a", -1, -1, 0644, "pwcmp/module.a");
  DL(pwcmp);
  SL(pwcmp, client);
  SL(pwcmp, hex);
  SL(pwcmp-module, module);
  s(lib, "pwcmp",        "libpwcmp.a");
  s(lib, "pwcmp-module", "libpwcmp-module.a");
  
  L(str);
  DL(str);
  SL(str, iter);
  SL(str, str);
  
  L(unix);
  DL(unix);
  SL(unix, nonblock);
  SL(unix, sig);
  
  L(vmailmgr);
  DL(vmailmgr);
  SL(vmailmgr, client);
  SL(vmailmgr, vpwentry);
}
