#include <string.h>

#include "dns.h"

int dns_ip4_packet(str *out,const char *buf,unsigned int len)
{
  unsigned int pos;
  unsigned char header[12];
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
    if (uint16_get_msb(header) == DNS_T_A)
      if (uint16_get_msb(header + 2) == DNS_C_IN)
        if (datalen == 4) {
	  if (!dns_packet_copy(buf,len,pos,header,4)) return -1;
	  if (!str_catb(out,(char*)header,4)) return -1;
	}
    pos += datalen;
  }

  dns_sortip((ipv4addr*)out->s,out->len/4);
  return 0;
}

static char *q = 0;

int dns_ip4_r(struct dns_transmit *tx,str *out,const char *fqdn)
{
  unsigned int i;
  char code;
  char ch;
  unsigned int len;

  if (!str_copys(out,"")) return -1;
  len = strlen(fqdn);
  code = 0;
  for (i = 0;i <= len;++i) {
    if (i < len)
      ch = fqdn[i];
    else
      ch = '.';

    if ((ch == '[') || (ch == ']')) continue;
    if (ch == '.') {
      if (!str_catc(out,code)) return -1;
      code = 0;
      continue;
    }
    if ((ch >= '0') && (ch <= '9')) {
      code *= 10;
      code += ch - '0';
      continue;
    }

    if (!dns_domain_fromdot(&q,fqdn,len)) return -1;
    if (dns_resolve(tx,q,DNS_T_A) == -1) return -1;
    if (dns_ip4_packet(out,tx->packet,tx->packetlen) == -1) return -1;
    dns_transmit_free(tx);
    dns_domain_free(&q);
    return 0;
  }

  out->len &= ~3;
  return 0;
}

DNS_R_FN_WRAP2(dns_ip4, str*, const char*)
