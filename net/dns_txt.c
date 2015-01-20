#include <string.h>

#include "dns.h"

static int sizeit(const char* buf, unsigned int len, unsigned int pos, uint16 datalen)
{
  if (pos + datalen > len) return -1;
  return datalen + 1;
  (void)buf;
}

static int getit(struct dns_result* out, unsigned int i, unsigned int offset,
		 const char* buf, unsigned int len, unsigned int pos, uint16 datalen)
{
  unsigned int txtlen;
  char ch;
  unsigned int j;
  unsigned int k;
  char* name = out->rr.name[i] = out->__buffer + offset;

  for (txtlen = j = 0; j < datalen; ) {
    ch = buf[pos + j];
    txtlen += (unsigned char)ch;
    j += (unsigned char)ch + 1;
  }
  txtlen = 0;
  for (j = k = 0;j < datalen;++j) {
    ch = buf[pos + j];
    if (!txtlen)
      txtlen = (unsigned char) ch;
    else {
      --txtlen;
      if (ch < 32) ch = '?';
      if (ch > 126) ch = '?';
      name[k++] = ch;
    }
  }
  name[k++] = 0;
  return k;
  (void)len;
}

/** Extract text (TXT) records from a DNS response packet. */
int dns_txt_packet(struct dns_result* out, const char* buf, unsigned int len)
{
  return dns_packet_extract(out, buf, len, DNS_T_TXT, DNS_C_IN, sizeit, getit);
}

/** Request the text (TXT) records for a domain name. */
int dns_txt_r(struct dns_transmit* tx, struct dns_result* out, const char* fqdn)
{
  char *q = 0;
  if (!dns_domain_fromdot(&q,fqdn,strlen(fqdn))) return -1;
  if (dns_resolve(tx,q,DNS_T_TXT) == -1) { free(q); return -1; }
  if (dns_txt_packet(out,tx->packet,tx->packetlen) == -1)  { free(q); return -1; }
  free(q);
  return 0;
}

/** \fn dns_txt(struct dns_result*, const char*)
    Request the text (TXT) records for a domain name.
*/
DNS_R_FN_WRAP(txt, const char*)

#ifdef SELFTEST_MAIN
struct dns_result out = {0};
void doit(const char* fqdn)
{
  int i;
  debugfn(dns_txt(&out, fqdn));
  obuf_putf(&outbuf, "d{ record\n}", out.count);
  for (i = 0; i < out.count; ++i) {
    obuf_puts(&outbuf, out.rr.name[i]);
    NL();
  }
}
MAIN
{
  doit("gmail.com");
  doit("2.0.0.127.sbl-xbl.spamhaus.org");
}
#endif
#ifdef SELFTEST_EXP
result=0
1 record
v=spf1 redirect=_spf.google.com
result=0
2 record
http://www.spamhaus.org/sbl/query/SBL233
http://www.spamhaus.org/query/bl?ip=127.0.0.2
#endif
