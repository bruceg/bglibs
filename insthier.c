#include "conf_home.c"
#include "installer.h"

#define C(SUB,FILE) c(SUB,#FILE,-1,-1,0644)
#define CF(SUB,FILE) cf(dir,#FILE,-1,-1,0644, #SUB "/" #FILE)
#define DI(SUB) dir = d(inc,#SUB,-1,-1,0755)
#define DL(SUB) dir = d(lib,"lib" #SUB,-1,-1,0755)

void insthier(void)
{
  int home = opendir(conf_home);
  int bin;
  int inc;
  int lib;
  int dir;

  bin = d(home, "bin", -1, -1, 0755);

  inc = d(home, "include", -1, -1, 0755);

  C(inc, installer.h);
  C(inc, sysdeps.h);
  C(inc, systime.h);
  C(inc, uint32.h);
  C(inc, uint64.h);
  
  DI(base64);
  CF(base64, base64.h);

  DI(cdb);
  CF(cdb, cdb.h);
  CF(cdb, make.h);
  CF(cdb, str.h);

  DI(cli);
  CF(cli, cli.h);
  
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

  C(lib, libinstaller.a);
  C(lib, libinstcheck.a);
  C(lib, libinstshow.a);
  C(lib, libsysdeps.a);
  
  DL(base64);
  CF(base64, base64.a);
 
  DL(cdb);
  CF(cdb, cdb.a);
  CF(cdb, make.a);
  CF(cdb, str.a);

  DL(cli);
  CF(cli, cli.a);
  
  DL(cvm);
  CF(cvm, client.a);
  CF(cvm, command.a);
  CF(cvm, local.a);
  CF(cvm, udp.a);

  DL(cvm-sasl);
  CF(cvm-sasl,cvm-sasl.a);

  DL(dict);
  CF(dict, dict.a);
  CF(dict, load.a);
  
  DL(iobuf);
  CF(iobuf, iobuf.a);
  CF(iobuf, str.a);
  
  DL(misc);
  CF(misc, misc.a);
  
  DL(msg);
  CF(msg, msg.a);
  CF(msg, wrap.a);
  
  DL(net);
  CF(net, ipv4.a);
  CF(net, resolve.a);
  CF(net, socket.a);
  
  DL(path);
  CF(path, path.a);
  
  DL(pwcmp);
  CF(pwcmp, client.a);
  CF(pwcmp, hex.a);
  CF(pwcmp, module.a);
  
  DL(str);
  CF(str, iter.a);
  CF(str, str.a);
  
  DL(unix);
  CF(unix, nonblock.a);
  CF(unix, sig.a);
  
  DL(vmailmgr);
  CF(vmailmgr, client.a);
  CF(vmailmgr, vpwentry.a);
}
