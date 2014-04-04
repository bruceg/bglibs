#include <string.h>

#include "dns.h"

int dns_mx_packet(str *out,const char *buf,unsigned int len)
{
  unsigned int pos;
  unsigned char header[12];
  unsigned char pref[2];
  uint16 numanswers;
  uint16 datalen;
  char *q = 0;

  if (!str_copys(out,"")) return -1;

  pos = dns_packet_copy(buf,len,0,header,12); if (!pos) return -1;
  numanswers = uint16_get_msb(header + 6);
  pos = dns_packet_skipname(buf,len,pos); if (!pos) return -1;
  pos += 4;

  while (numanswers--) {
    pos = dns_packet_skipname(buf,len,pos); if (!pos) return -1;
    pos = dns_packet_copy(buf,len,pos,header,10); if (!pos) return -1;
    datalen = uint16_get_msb(header + 8);
    if (uint16_get_msb(header) == DNS_T_MX)
      if (uint16_get_msb(header + 2) == DNS_C_IN) {
	if (!dns_packet_copy(buf,len,pos,pref,2)) return -1;
	if (!dns_packet_getname(buf,len,pos + 2,&q)) return -1;
	if (!str_catb(out,(char*)pref,2)) { free(q); return -1; }
	if (!dns_domain_todot_cat(out,q)) { free(q); return -1; }
	free(q);
	if (!str_catc(out,0)) return -1;
      }
    pos += datalen;
  }

  return 0;
}

int dns_mx_r(struct dns_transmit *tx,str *out,const char *fqdn)
{
  char *q = 0;
  if (!dns_domain_fromdot(&q,fqdn,strlen(fqdn))) return -1;
  if (dns_resolve(tx,q,DNS_T_MX) == -1) { free(q); return -1; }
  free(q);
  if (dns_mx_packet(out,tx->packet,tx->packetlen) == -1)  return -1;
  dns_transmit_free(tx);
  return 0;
}

DNS_R_FN_WRAP2(dns_mx, str*, const char*)

#ifdef SELFTEST_MAIN
#include "str/iter.h"
MAIN
{
  str out = {0};
  striter i;

  dns_mx(&out, "untroubled.org");
  striter_loop(&i, &out, 0) {
    obuf_putf(&outbuf, "d{ }d{: \"}s{\"\n}",
	      uint16_get_msb((unsigned char*)i.startptr),
	      i.len - 2,
	      i.startptr + 2);
  }
  obuf_flush(&outbuf);
}
#endif
#ifdef SELFTEST_EXP
12801 18: "mx.futurequest.net"
#endif
