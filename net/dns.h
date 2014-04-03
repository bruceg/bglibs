#ifndef DNS_H
#define DNS_H

#include <sysdeps.h>
#include <systime.h>
#include <net/ipv4.h>
#include <str/str.h>

#define TV_LESS(A,B) (((A)->tv_sec < (B)->tv_sec) || ((A)->tv_sec == (B)->tv_sec && (A)->tv_usec < (B)->tv_usec))

#define DNS_C_IN "\0\1"
#define DNS_C_ANY "\0\377"

#define DNS_T_A "\0\1"
#define DNS_T_NS "\0\2"
#define DNS_T_CNAME "\0\5"
#define DNS_T_SOA "\0\6"
#define DNS_T_PTR "\0\14"
#define DNS_T_HINFO "\0\15"
#define DNS_T_MX "\0\17"
#define DNS_T_TXT "\0\20"
#define DNS_T_RP "\0\21"
#define DNS_T_SIG "\0\30"
#define DNS_T_KEY "\0\31"
#define DNS_T_AAAA "\0\34"
#define DNS_T_AXFR "\0\374"
#define DNS_T_ANY "\0\377"

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
  char qtype[2];
} ;

extern void dns_random_init(const char *);
extern unsigned int dns_random(unsigned int);

extern void dns_sortip(ipv4addr *,unsigned int);

extern void dns_domain_free(char **);
extern int dns_domain_copy(char **,const char *);
extern unsigned int dns_domain_length(const char *);
extern int dns_domain_equal(const char *,const char *);
extern int dns_domain_suffix(const char *,const char *);
extern unsigned int dns_domain_suffixpos(const char *,const char *);
extern int dns_domain_fromdot(char **,const char *,unsigned int);
extern int dns_domain_todot_cat(str *,const char *);

extern unsigned int dns_packet_copy(const char *,unsigned int,unsigned int,char *,unsigned int);
extern unsigned int dns_packet_getname(const char *,unsigned int,unsigned int,char **);
extern unsigned int dns_packet_skipname(const char *,unsigned int,unsigned int);

extern int dns_transmit_start(struct dns_transmit *,const ipv4addr [16],int,const char *,const char *,const ipv4addr *);
extern void dns_transmit_free(struct dns_transmit *);
extern void dns_transmit_io(struct dns_transmit *,iopoll_fd *,struct timeval *);
extern int dns_transmit_get(struct dns_transmit *,const iopoll_fd *,const struct timeval *);

extern int dns_resolvconfip(ipv4addr [16]);
extern int dns_resolve(const char *,const char *);
extern struct dns_transmit dns_resolve_tx;

extern int dns_ip4_packet(str *,const char *,unsigned int);
extern int dns_ip4(str *,const str *);
extern int dns_name4_packet(str *,const char *,unsigned int);
extern void dns_name4_domain(char *,const ipv4addr *);
#define DNS_NAME4_DOMAIN 31
extern int dns_name4(str *,const ipv4addr *);
extern int dns_txt_packet(str *,const char *,unsigned int);
extern int dns_txt(str *,const str *);
extern int dns_mx_packet(str *,const char *,unsigned int);
extern int dns_mx(str *,const str *);

extern int dns_resolvconfrewrite(str *);
extern int dns_ip4_qualify_rules(str *,str *,const str *,const str *);
extern int dns_ip4_qualify(str *,str *,const str *);

#endif
