#include <string.h>

#include "dns.h"

static char *q = 0;

int dns_mx_packet(str *out,const char *buf,unsigned int len)
{
  unsigned int pos;
  unsigned char header[12];
  unsigned char pref[2];
  uint16 numanswers;
  uint16 datalen;

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
	if (!str_catb(out,(char*)pref,2)) return -1;
	if (!dns_domain_todot_cat(out,q)) return -1;
	if (!str_catc(out,0)) return -1;
      }
    pos += datalen;
  }

  return 0;
}

int dns_mx_r(struct dns_transmit *tx,str *out,const char *fqdn)
{
  if (!dns_domain_fromdot(&q,fqdn,strlen(fqdn))) return -1;
  if (dns_resolve(tx,q,DNS_T_MX) == -1) return -1;
  if (dns_mx_packet(out,tx->packet,tx->packetlen) == -1) return -1;
  dns_transmit_free(tx);
  dns_domain_free(&q);
  return 0;
}

DNS_R_FN_WRAP2(dns_mx, str*, const char*)
