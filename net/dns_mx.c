#include <string.h>

#include "dns.h"

static int sizeit(const char* buf, unsigned int len, unsigned int pos, uint16 datalen)
{
  char* q = 0;
  if (!dns_packet_getname(buf,len,pos + 2,&q)) return -1;
  len = fmt_dns_domain(0, q);
  free(q);
  return len + 1;
  (void)datalen;
}

static int getit(struct dns_result* out, unsigned int i, unsigned int offset,
		 const char* buf, unsigned int len, unsigned int pos, uint16 datalen)
{
  unsigned char pref[2];
  char* q = 0;
  struct dns_mx* mx = &out->rr.mx[i];

  if (!dns_packet_copy(buf,len,pos,pref,2)) return -1;
  mx->distance = uint16_get_msb(pref);
  if (!dns_packet_getname(buf,len,pos + 2,&q)) return -1;
  mx->name = out->__buffer + offset;
  len = fmt_dns_domain(mx->name, q);
  mx->name[len] = 0;
  free(q);
  return len + 1;
  (void)datalen;
}

/** Extract mail exchanger (MX) records from a DNS response packet. */
int dns_mx_packet(struct dns_result* out, const char* buf, unsigned int len)
{
  return dns_packet_extract(out, buf, len, DNS_T_MX, DNS_C_IN, sizeit, getit);
}

/** Request the mail exchanger (MX) records for a domain name. */
int dns_mx_r(struct dns_transmit* tx, struct dns_result* out, const char* fqdn)
{
  char *q = 0;
  if (!dns_domain_fromdot(&q,fqdn,strlen(fqdn))) return -1;
  if (dns_resolve(tx,q,DNS_T_MX) == -1) { free(q); return -1; }
  free(q);
  if (dns_mx_packet(out,tx->packet,tx->packetlen) == -1)  return -1;
  dns_transmit_free(tx);
  return 0;
}

/** \fn dns_mx(struct dns_result*, const char*)
    Request the mail exchanger (MX) records for a domain name.
*/
DNS_R_FN_WRAP(mx, const char*)

#ifdef SELFTEST_MAIN
#include "dns-responder.c"
DUMP
{
  int i;

  dns_sort_mx(rr->mx, count);
  for (i = 0; i < count; ++i)
    obuf_putf(&outbuf, "d{ }d{: \"}s{\"\n}", i, rr->mx[i].distance, rr->mx[i].name);
}
RESPONSE response1 = {
  1, 3, 0, {
    { "\300\014", 2, 15, 1, 86400, "2\1\2mx\vfuturequest\3net\0", 22 },
    { "\300\014", 2, 1, 1, 123456, "\3ns2\300\061", 6 },
    { "\300\014", 2, 1, 1, 123456, "\3ns1\300\061", 6 },
    { "\300\014", 2, 1, 1, 123456, "\3ns3\300\061", 6 },
  }
};
RESPONSE response2 = {
  5, 0, 0, {
    { "\300\014", 2, 15, 1, 86400, "\0\024\4alt1\5aspmx\1l\6google\3com\0", 27 },
    { "\300\014", 2, 15, 1, 86400, "\0\012\5aspmx\1l\6google\3com\0", 22, },
    { "\300\014", 2, 15, 1, 86400, "\0\036\4alt2\5aspmx\1l\6google\3com\0", 27 },
    { "\300\014", 2, 15, 1, 86400, "\0\050\4alt3\5aspmx\1l\6google\3com\0", 27 },
    { "\300\014", 2, 15, 1, 86400, "\0\062\4alt4\5aspmx\1l\6google\3com\0", 27 },
  }
};
MAIN
{
  do_dns_respond_test("untroubled.org", &response1, dns_mx);
  do_dns_respond_test("google.com", &response2, dns_mx);
}
#endif
#ifdef SELFTEST_EXP
32: ID=XX QR=0 opcode=0 AA=0 TC=0 RD=1 RA=0 Z=0 RCODE=0 QDCOUNT=1 ANCOUNT=0 NSCOUNT=0 ARCOUNT=0
Question: untroubled.org. QTYPE=15 QCLASS=1
result=0
untroubled.org: count=1
0 12801: "mx.futurequest.net"
28: ID=XX QR=0 opcode=0 AA=0 TC=0 RD=1 RA=0 Z=0 RCODE=0 QDCOUNT=1 ANCOUNT=0 NSCOUNT=0 ARCOUNT=0
Question: google.com. QTYPE=15 QCLASS=1
result=0
google.com: count=5
0 10: "aspmx.l.google.com"
1 20: "alt1.aspmx.l.google.com"
2 30: "alt2.aspmx.l.google.com"
3 40: "alt3.aspmx.l.google.com"
4 50: "alt4.aspmx.l.google.com"
#endif
