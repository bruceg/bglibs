#include <string.h>

#include "dns.h"

int dns_name4_packet(str *out,const char *buf,unsigned int len)
{
  unsigned int pos;
  unsigned char header[12];
  uint16 numanswers;
  uint16 datalen;
  char *q = 0;
  int r;

  if (!str_copys(out,"")) return -1;

  pos = dns_packet_copy(buf,len,0,header,12); if (!pos) return -1;
  numanswers = uint16_get_msb(header + 6);
  pos = dns_packet_skipname(buf,len,pos); if (!pos) return -1;
  pos += 4;

  while (numanswers--) {
    pos = dns_packet_skipname(buf,len,pos); if (!pos) return -1;
    pos = dns_packet_copy(buf,len,pos,header,10); if (!pos) return -1;
    datalen = uint16_get_msb(header + 8);
    if (uint16_get_msb(header) == DNS_T_PTR)
      if (uint16_get_msb(header + 2) == DNS_C_IN) {
	if (!dns_packet_getname(buf,len,pos,&q)) return -1;
	r = dns_domain_todot_cat(out,q);
	free(q);
	return r ? 0 : -1;
      }
    pos += datalen;
  }

  return 0;
}

int dns_name4_r(struct dns_transmit *tx,str *out,const ipv4addr *ip)
{
  char name[DNS_NAME4_DOMAIN];

  dns_name4_domain(name,ip);
  if (dns_resolve(tx,name,DNS_T_PTR) == -1) return -1;
  if (dns_name4_packet(out,tx->packet,tx->packetlen) == -1) return -1;
  dns_transmit_free(tx);
  return 0;
}

DNS_R_FN_WRAP2(dns_name4, str*, const ipv4addr*)
