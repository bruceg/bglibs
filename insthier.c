#include "conf_home.c"
#include "installer.h"

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

#define CL(SUB,BASE,LINK) do{ \
  cf(dir, #BASE ".a", -1, -1, 0644, #SUB "/" #BASE ".a"); \
  s(lib, #LINK, "lib" #SUB "/" #BASE ".a"); \
}while(0)

  L(sysdeps);
  L(installer);
  L(instcheck);
  L(instshow);

  DL(base64);
  CL(base64, base64, base64);
 
  DL(cdb);
  CL(cdb, cdb,  cdb);
  CL(cdb, make, cdb-make);
  CL(cdb, str,  cdb-str);

  DL(cli);
  CL(cli, cli, cli);

  DL(crypto);
  CL(crypto, md5,     md5);
  CL(crypto, sha1,    sha1);
  CL(crypto, sha256,  sha256);
  CL(crypto, sha512a, sha512a);
  
  DL(cvm);
  CL(cvm, client,  cvm-client);
  CL(cvm, command, cvm-command);
  CL(cvm, local,   cvm-local);
  CL(cvm, udp,     cvm-udp);

  DL(cvm-sasl);
  CL(cvm-sasl, cvm-sasl, cvm-sasl);

  DL(dict);
  CL(dict, dict, dict);
  CL(dict, load, dict-load);
  
  DL(iobuf);
  CL(iobuf, iobuf, iobuf);
  CL(iobuf, str,   iobuf-str);
  
  DL(misc);
  CL(misc, misc, misc);
  
  DL(msg);
  CL(msg, msg,  msg);
  CL(msg, wrap, msg-wrap);
  
  DL(net);
  CL(net, ipv4,    net-ipv4);
  CL(net, resolve, net-resolve);
  CL(net, socket,  net-socket);
  
  DL(path);
  CL(path, path,   path);
  
  DL(pwcmp);
  CL(pwcmp, client, pwcmp-client);
  CL(pwcmp, hex,    pwcmp-hex);
  CL(pwcmp, module, pwcmp-module);
  
  DL(str);
  CL(str, iter, str-iter);
  CL(str, str,  str);
  
  DL(unix);
  CL(unix, nonblock, unix-nonblock);
  CL(unix, sig,      unix-sig);
  
  DL(vmailmgr);
  CL(vmailmgr, client,   vmailmgr-client);
  CL(vmailmgr, vpwentry, vmailmgr-vpwentry);
}
