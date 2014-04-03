#include <string.h>

#include "uint16.h"
#include "dns.h"

int dns_ip4_packet(str *out,const char *buf,unsigned int len)
{
  unsigned int pos;
  char header[12];
  uint16 numanswers;
  uint16 datalen;

  if (!str_copys(out,"")) return -1;

  pos = dns_packet_copy(buf,len,0,header,12); if (!pos) return -1;
  numanswers = uint16_get_msb((unsigned char*)header + 6);
  pos = dns_packet_skipname(buf,len,pos); if (!pos) return -1;
  pos += 4;

  while (numanswers--) {
    pos = dns_packet_skipname(buf,len,pos); if (!pos) return -1;
    pos = dns_packet_copy(buf,len,pos,header,10); if (!pos) return -1;
    datalen = uint16_get_msb((unsigned char*)header + 8);
    if (memcmp(header,DNS_T_A,2) == 0)
      if (memcmp(header + 2,DNS_C_IN,2) == 0)
        if (datalen == 4) {
	  if (!dns_packet_copy(buf,len,pos,header,4)) return -1;
	  if (!str_catb(out,header,4)) return -1;
	}
    pos += datalen;
  }

  dns_sortip(out->s,out->len);
  return 0;
}

static char *q = 0;

int dns_ip4(str *out,const str *fqdn)
{
  unsigned int i;
  char code;
  char ch;

  if (!str_copys(out,"")) return -1;
  code = 0;
  for (i = 0;i <= fqdn->len;++i) {
    if (i < fqdn->len)
      ch = fqdn->s[i];
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

    if (!dns_domain_fromdot(&q,fqdn->s,fqdn->len)) return -1;
    if (dns_resolve(q,DNS_T_A) == -1) return -1;
    if (dns_ip4_packet(out,dns_resolve_tx.packet,dns_resolve_tx.packetlen) == -1) return -1;
    dns_transmit_free(&dns_resolve_tx);
    dns_domain_free(&q);
    return 0;
  }

  out->len &= ~3;
  return 0;
}
