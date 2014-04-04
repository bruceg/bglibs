#include <string.h>

#include "uint16.h"
#include "dns.h"

int dns_txt_packet(str *out,const char *buf,unsigned int len)
{
  unsigned int pos;
  unsigned char header[12];
  uint16 numanswers;
  uint16 datalen;
  char ch;
  unsigned int txtlen;
  int i;

  if (!str_copys(out,"")) return -1;

  pos = dns_packet_copy(buf,len,0,header,12); if (!pos) return -1;
  numanswers = uint16_get_msb(header + 6);
  pos = dns_packet_skipname(buf,len,pos); if (!pos) return -1;
  pos += 4;

  while (numanswers--) {
    pos = dns_packet_skipname(buf,len,pos); if (!pos) return -1;
    pos = dns_packet_copy(buf,len,pos,header,10); if (!pos) return -1;
    datalen = uint16_get_msb(header + 8);
    if (memcmp(header,DNS_T_TXT,2) == 0)
      if (memcmp(header + 2,DNS_C_IN,2) == 0) {
	if (pos + datalen > len) return -1;
	txtlen = 0;
	for (i = 0;i < datalen;++i) {
	  ch = buf[pos + i];
	  if (!txtlen)
	    txtlen = (unsigned char) ch;
	  else {
	    --txtlen;
	    if (ch < 32) ch = '?';
	    if (ch > 126) ch = '?';
	    if (!str_cats(out,&ch)) return -1;
	  }
	}
      }
    pos += datalen;
  }

  return 0;
}

static char *q = 0;

int dns_txt(str *out,const str *fqdn)
{
  if (!dns_domain_fromdot(&q,fqdn->s,fqdn->len)) return -1;
  if (dns_resolve(q,DNS_T_TXT) == -1) return -1;
  if (dns_txt_packet(out,dns_resolve_tx.packet,dns_resolve_tx.packetlen) == -1) return -1;
  dns_transmit_free(&dns_resolve_tx);
  dns_domain_free(&q);
  return 0;
}
