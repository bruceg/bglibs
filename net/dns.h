#ifndef DNS_H
#define DNS_H

#include <sysdeps.h>
#include <systime.h>
#include <net/ipv4.h>
#include <net/ipv6.h>
#include <str/str.h>
#include <uint16.h>

#define TV_LESS(A,B) (((A)->tv_sec < (B)->tv_sec) || ((A)->tv_sec == (B)->tv_sec && (A)->tv_usec < (B)->tv_usec))

#define DNS_C_IN 1
#define DNS_C_ANY 255

#define DNS_T_A 1
#define DNS_T_NS 2
#define DNS_T_CNAME 5
#define DNS_T_SOA 6
#define DNS_T_PTR 12
#define DNS_T_HINFO 13
#define DNS_T_MX 15
#define DNS_T_TXT 16
#define DNS_T_RP 17
#define DNS_T_SIG 24
#define DNS_T_KEY 25
#define DNS_T_AAAA 28
#define DNS_T_AXFR 252
#define DNS_T_ANY 255

struct dns_transmit {
  char *query; /* 0, or dynamically allocated */
  unsigned int querylen;
  char *packet; /* 0, or dynamically allocated */
  unsigned int packetlen;
  int s1; /* 0, or 1 + an open file descriptor */
  int tcpstate;
  unsigned int udploop;
  unsigned int curserver;
  struct timeval deadline;
  unsigned int pos;
  const ipv4addr *servers;
  ipv4addr localip;
  uint16 qtype;
} ;

#define DNS_RANDOM_SEED (32*4)
#define DNS_MAX_IPS 16

extern void dns_random_init(const char [DNS_RANDOM_SEED]);
extern unsigned int dns_random(unsigned int);

extern void dns_rotate(unsigned char*, unsigned int n, unsigned int shift);
extern void dns_rotateipv4(ipv4addr*, unsigned int);
extern void dns_rotateipv6(ipv6addr*, unsigned int);

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
extern int dns_packet_extract(str* out, const char* buf, unsigned int len, uint16 rrtype, uint16 rrclass,
			      int (*fn)(str*, const char*, unsigned int, unsigned int, uint16));

extern int dns_transmit_start(struct dns_transmit *,const ipv4addr [DNS_MAX_IPS],int,const char *,uint16,const ipv4addr *);
extern void dns_transmit_free(struct dns_transmit *);
extern void dns_transmit_io(const struct dns_transmit *,iopoll_fd *,struct timeval *);
extern int dns_transmit_get(struct dns_transmit *,const iopoll_fd *,const struct timeval *);

extern int dns_read_resolvconf(str *out);
extern int dns_resolvconfip(ipv4addr [DNS_MAX_IPS]);
extern int dns_resolve(struct dns_transmit *,const char *,uint16);

extern int dns_ip4_packet(str *,const char *,unsigned int);
extern int dns_ip4_r(struct dns_transmit *,str *,const char *);
extern int dns_ip4(str *,const char *);
extern int dns_ip6_packet(str *,const char *,unsigned int);
extern int dns_ip6_r(struct dns_transmit *,str *,const char *);
extern int dns_ip6(str *,const char *);
extern int dns_name_packet(str *,const char *,unsigned int);
#define DNS_NAME4_DOMAIN (4*4+14)
extern void dns_name4_domain(char [DNS_NAME4_DOMAIN],const ipv4addr *);
#define DNS_NAME6_DOMAIN (32*2+10)
extern void dns_name6_domain(char [DNS_NAME6_DOMAIN],const ipv6addr *);
extern int dns_name4_r(struct dns_transmit *,str *,const ipv4addr *);
extern int dns_name4(str *,const ipv4addr *);
extern int dns_name6_r(struct dns_transmit *,str *,const ipv6addr *);
extern int dns_name6(str *,const ipv6addr *);
extern int dns_txt_packet(str *,const char *,unsigned int);
extern int dns_txt_r(struct dns_transmit *,str *,const char *);
extern int dns_txt(str *,const char *);
extern int dns_mx_packet(str *,const char *,unsigned int);
extern int dns_mx_r(struct dns_transmit *,str *,const char *);
extern int dns_mx(str *,const char *);

extern int dns_resolvconfrewrite(str *);
extern int dns_qualify_rules(str *,str *,const char *,const str *, int (*)(str*, const char*));
extern int dns_qualify(str *,str *,const char *, int (*)(str*, const char*));

#define DNS_R_FN_WRAP2(FN,TYPE1,TYPE2)		\
  int FN(TYPE1 p1,TYPE2 p2)			\
  {						\
    struct dns_transmit tx = {0};		\
    int r = FN##_r(&tx,p1,p2);			\
    dns_transmit_free(&tx);			\
    return r;					\
  }

#endif
