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
#include "dns-responder.c"
DUMP
{
  int i;
  for (i = 0; i < count; ++i) {
    obuf_puts(&outbuf, rr->name[i]);
    NL();
  }
}
RESPONSE response1 = {
  2, 1, 0, {
    { "\300\014", 2, 16, 1, 5, "\050http://www.spamhaus.org/sbl/query/SBL233", 41 },
    { "\300\014", 2, 16, 1, 5, "\055http://www.spamhaus.org/query/bl?ip=127.0.0.2", 46 },
    { "\300\026", 2, 2, 1, 3132, "\001g\002ns\300\036", 7 },
  }
};
RESPONSE response2 = {
  1, 1, 0, {
    { "\300\014", 2, 16, 1, 512, "\037v=spf1 redirect=_spf.google.com", 32 },
    { "\300\026", 2, 2, 1, 123456, "\003ns2\006google\300\022", 13 },
  }
};
MAIN
{
  do_dns_respond_test("2.0.0.127.sbl-xbl.spamhaus.org", &response1, dns_txt);
  do_dns_respond_test("gmail.com", &response2, dns_txt);
}
#endif
#ifdef SELFTEST_EXP
48: ID=XX QR=0 opcode=0 AA=0 TC=0 RD=1 RA=0 Z=0 RCODE=0 QDCOUNT=1 ANCOUNT=0 NSCOUNT=0 ARCOUNT=0
Question: 2.0.0.127.sbl-xbl.spamhaus.org. QTYPE=16 QCLASS=1
result=0
2.0.0.127.sbl-xbl.spamhaus.org: count=2
http://www.spamhaus.org/sbl/query/SBL233
http://www.spamhaus.org/query/bl?ip=127.0.0.2
27: ID=XX QR=0 opcode=0 AA=0 TC=0 RD=1 RA=0 Z=0 RCODE=0 QDCOUNT=1 ANCOUNT=0 NSCOUNT=0 ARCOUNT=0
Question: gmail.com. QTYPE=16 QCLASS=1
result=0
gmail.com: count=1
v=spf1 redirect=_spf.google.com
#endif
