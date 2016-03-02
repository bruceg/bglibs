#ifndef DNS_H
#define DNS_H

#include "sysdeps.h"
#include "systime.h"
#include "ipv4.h"
#include "ipv6.h"
#include "str.h"
#include "uint16.h"

/** True if \c struct \c timeval \c A is less than \c B */
#define TV_LESS(A,B) (((A)->tv_sec < (B)->tv_sec) || ((A)->tv_sec == (B)->tv_sec && (A)->tv_usec < (B)->tv_usec))

/** \defgroup dns dns: DNS Resolver Library

@{

The DNS resolver library is a modified copy of the public domain library
used in djbdns (http://cr.yp.to/djbdns.html). Many modifications have
been made, both to make it fit within the bglibs library and to extend
the functionality.

Notable enhancements are:

- support for IPv6 lookups (both forward and reverse)

- qualification support for lookup types other than IPv4 addresses

- support for multiple outstanding requests

\return Functions that return an \c int return -1 on error and set \c errno.

*/

/** Record class: Internet protocol */
#define DNS_C_IN 1
/** Record class: Any */
#define DNS_C_ANY 255

/** Record type: Address (IPv4) */
#define DNS_T_A 1
/** Record type: Name server */
#define DNS_T_NS 2
/** Record type: Canonical name (alias) */
#define DNS_T_CNAME 5
/** Record type: Start of authority */
#define DNS_T_SOA 6
/** Record type: Pointer */
#define DNS_T_PTR 12
/** Record type: Host information */
#define DNS_T_HINFO 13
/** Record type: Mail exchanger */
#define DNS_T_MX 15
/** Record type: Text */
#define DNS_T_TXT 16
/** Record type: Responsible person */
#define DNS_T_RP 17
/** Record type: Signature */
#define DNS_T_SIG 24
/** Record type: Key record */
#define DNS_T_KEY 25
/** Record type: Address (IPv6) */
#define DNS_T_AAAA 28
/** Record type: Authoritative zone transfer */
#define DNS_T_AXFR 252
/** Record type: Any (all known records) */
#define DNS_T_ANY 255

/** Size of data used by \c dns_random_init */
#define DNS_RANDOM_SEED (32*4)
/** Maximum number of IPs returned by \c dns_resolvconfip or used by \c dns_transmit_start */
#define DNS_MAX_IPS 16
/** Maximum output size of \c dns_name4_domain */
#define DNS_NAME4_DOMAIN (4*4+14)
/** Maximum output size of \c dns_name6_domain */
#define DNS_NAME6_DOMAIN (32*2+10)

/** DNS transmission record */
struct dns_transmit {
  /** The transmitted query packet. This will either be \c NULL or dynamically allocated. */
  char *query;
  /** The length of the transmitted query packet. */
  unsigned int querylen;
  /** The received response packet. This will either be \c NULL or dynamically allocated. */
  char *packet;
  /** The length of the received response packet. */
  unsigned int packetlen;
  /** The socket file descriptor used for data transmission.  This will
      either be 0 or 1 + an open file descriptor. The addition is done
      to allow the uninitialized state to be all zeros. */
  int s1;
  /** Current state of the TCP connection.  Valid values are:

      \li 0 is uninitialized (ie using UDP)
      \li 1 is connecting
      \li 2 is connected, sending query
      \li 3 is query sent, waiting for start of response
      \li 4 is reading response length
      \li 5 is read packet length, reading response
  */
  int tcpstate;
  /** Count of number of times the UDP query was sent to each server. */
  unsigned int udploop;
  /** Index of the server we are currently sending to. */
  unsigned int curserver;
  /** Deadline for considering a request timed out. */
  struct timeval deadline;
  /** Current sending/receiving position within the buffer, for TCP transmissions. */
  unsigned int pos;
  /** The list of servers to try. */
  const ipv4addr *servers;
  /** The local IP address to transmit with. */
  ipv4addr localip;
  /** The requested query type. */
  uint16 qtype;
};

/** An individual result from a \c dns_mx query. */
struct dns_mx
{
  /** The distance (or preference) number for this MX. */
  uint16 distance;
  /** The domain name of this MX. */
  char* name;
};

union dns_result_rrs
{
  void* __ptr;
  ipv4addr* ip4;
  ipv6addr* ip6;
  struct dns_mx* mx;
  char** name;
};

/** DNS query results. */
struct dns_result
{
  /** The number of results present. */
  int count;
  /** The record type of the results. */
  int type;
  /** The individual record arrays. */
  union dns_result_rrs rr;
  /** Internal use buffer pointer. */
  void* __buffer;
};

extern int dns_result_alloc(struct dns_result* d, int type, int count, int size);
extern void dns_result_free(struct dns_result* d);

extern void dns_random_init(const char [DNS_RANDOM_SEED]);
extern unsigned int dns_random(unsigned int);

extern void dns_rotate(unsigned char*, unsigned int n, unsigned int shift);
extern void dns_rotateipv4(ipv4addr*, unsigned int);
extern void dns_rotateipv6(ipv6addr*, unsigned int);

extern void dns_sort_mx(struct dns_mx* mxs, int count);

extern void dns_domain_free(char **);
extern int dns_domain_copy(char **,const char *);
extern unsigned int dns_domain_length(const char *);
extern int dns_domain_equal(const char *,const char *);
extern int dns_domain_suffix(const char *,const char *);
extern unsigned int dns_domain_suffixpos(const char *,const char *);
extern int dns_domain_fromdot(char **,const char *,unsigned int);
extern int dns_domain_todot_cat(str *,const char *);

extern unsigned int dns_packet_copy(const char *,unsigned int,unsigned int,unsigned char *,unsigned int);
extern unsigned int dns_packet_getname(const char *,unsigned int,unsigned int,char **);
extern unsigned int dns_packet_skipname(const char *,unsigned int,unsigned int);
extern int dns_packet_extract(struct dns_result* out, const char* buf, unsigned int len, uint16 rrtype, uint16 rrclass,
			      int (*sizefn)(const char* buf, unsigned int len, unsigned int pos, uint16 datalen),
			      int (*copy)(struct dns_result* out, unsigned int index, unsigned int offset,
					  const char* buf, unsigned int len, unsigned int pos, uint16 datalen));

extern int dns_transmit_start(struct dns_transmit *,const ipv4addr [DNS_MAX_IPS],int,const char *,uint16,const ipv4addr *);
extern void dns_transmit_free(struct dns_transmit *);
extern void dns_transmit_io(const struct dns_transmit *,iopoll_fd *,struct timeval *);
extern int dns_transmit_get(struct dns_transmit *,const iopoll_fd *,const struct timeval *);

extern int dns_read_resolvconf(str *out);
extern int dns_resolvconfip(ipv4addr [DNS_MAX_IPS]);
extern int dns_resolve(struct dns_transmit *,const char *,uint16);

extern int dns_ip4_packet(struct dns_result*, const char*, unsigned int);
extern int dns_ip4_r(struct dns_transmit*, struct dns_result*, const char *);
extern int dns_ip4(struct dns_result*, const char *);
extern int dns_ip6_packet(struct dns_result*, const char*, unsigned int);
extern int dns_ip6_r(struct dns_transmit*, struct dns_result*, const char *);
extern int dns_ip6(struct dns_result*, const char*);
extern int dns_name_packet(struct dns_result*, const char *,unsigned int);
extern void dns_name4_domain(char [DNS_NAME4_DOMAIN],const ipv4addr *);
extern void dns_name6_domain(char [DNS_NAME6_DOMAIN],const ipv6addr *);
extern int dns_name4_r(struct dns_transmit*, struct dns_result*, const ipv4addr*);
extern int dns_name4(struct dns_result*, const ipv4addr*);
extern int dns_name6_r(struct dns_transmit*, struct dns_result*, const ipv6addr*);
extern int dns_name6(struct dns_result*, const ipv6addr*);
extern int dns_txt_packet(struct dns_result*, const char *,unsigned int);
extern int dns_txt_r(struct dns_transmit *,struct dns_result*, const char *);
extern int dns_txt(struct dns_result*, const char *);
extern int dns_mx_packet(struct dns_result*, const char *,unsigned int);
extern int dns_mx_r(struct dns_transmit *,struct dns_result*, const char *);
extern int dns_mx(struct dns_result*, const char *);

extern int dns_resolvconfrewrite(str *);
extern int dns_qualify_rules(struct dns_result*,str *,const char *,const str *,
			     int (*)(struct dns_transmit*, struct dns_result*, const char*));
extern int dns_qualify(struct dns_result*,str *,const char *,
		       int (*)(struct dns_transmit*, struct dns_result*, const char*));

extern unsigned fmt_dns_domain(char*, const char*);

/** Wrapper macro to create a non-reentrant function from a \c dns_*_r function. */
#define DNS_R_FN_WRAP(NAME,TYPE)			\
  int dns_##NAME(struct dns_result* out,TYPE in)	\
  {							\
    struct dns_transmit tx = {0};			\
    int r = dns_##NAME##_r(&tx,out,in);			\
    dns_transmit_free(&tx);				\
    return r;						\
  }

/** @} */

#endif
