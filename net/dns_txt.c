#include <string.h>

#include "dns.h"

static int getit(str* out, const char* buf, unsigned int len, unsigned int pos, uint16 datalen)
{
  unsigned int txtlen;
  char ch;
  unsigned int i;

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
      if (!str_catc(out,ch)) return -1;
    }
  }
  return 1;
}

/** Extract text (TXT) records from a DNS response packet. */
int dns_txt_packet(str *out,const char *buf,unsigned int len)
{
  return dns_packet_extract(out, buf, len, DNS_T_TXT, DNS_C_IN, getit);
}

/** Request the text (TXT) records for a domain name. */
int dns_txt_r(struct dns_transmit *tx,str *out,const char *fqdn)
{
  char *q = 0;
  if (!dns_domain_fromdot(&q,fqdn,strlen(fqdn))) return -1;
  if (dns_resolve(tx,q,DNS_T_TXT) == -1) { free(q); return -1; }
  if (dns_txt_packet(out,tx->packet,tx->packetlen) == -1)  { free(q); return -1; }
  free(q);
  return 0;
}

/** \fn dns_txt(str*, const char*)
    Request the text (TXT) records for a domain name.
*/
DNS_R_FN_WRAP2(dns_txt, str*, const char*)

#ifdef SELFTEST_MAIN
MAIN
{
  str out = {0};

  dns_txt(&out, "gmail.com");
  obuf_putf(&outbuf, "d{: \"}S{\"\n}", out.len, &out);
  obuf_flush(&outbuf);
}
#endif
#ifdef SELFTEST_EXP
31: "v=spf1 redirect=_spf.google.com"
#endif
