/*
DNS should have used LZ77 instead of its own sophomoric compression algorithm.
*/

#include <errno.h>

#include "dns.h"

/** Copy a block of data out of a packet.
    \returns the offset of the next byte of data, or \c 0 if the packet was too short.
*/
unsigned int dns_packet_copy(const char *buf,unsigned int len,unsigned int pos,unsigned char *out,unsigned int outlen)
{
  while (outlen) {
    if (pos >= len) { errno = EPROTO; return 0; }
    *out = buf[pos++];
    ++out; --outlen;
  }
  return pos;
}

/** Skip over a domain name within a packet. */
unsigned int dns_packet_skipname(const char *buf,unsigned int len,unsigned int pos)
{
  unsigned char ch;

  for (;;) {
    if (pos >= len) break;
    ch = buf[pos++];
    if (ch >= 192) return pos + 1;
    if (ch >= 64) break;
    if (!ch) return pos;
    pos += ch;
  }

  errno = EPROTO;
  return 0;
}

/** Extract a domain name out of a packet, handling name compression. */
unsigned int dns_packet_getname(const char *buf,unsigned int len,unsigned int pos,char **d)
{
  unsigned int loop = 0;
  unsigned int state = 0;
  unsigned int firstcompress = 0;
  unsigned int where;
  unsigned char ch;
  char name[255];
  unsigned int namelen = 0;

  for (;;) {
    if (pos >= len) goto PROTO; ch = buf[pos++];
    if (++loop >= 1000) goto PROTO;

    if (state) {
      if (namelen + 1 > sizeof name) goto PROTO; name[namelen++] = ch;
      --state;
    }
    else {
      while (ch >= 192) {
	where = ch; where -= 192; where <<= 8;
	if (pos >= len) goto PROTO; ch = buf[pos++];
	if (!firstcompress) firstcompress = pos;
	pos = where + ch;
	if (pos >= len) goto PROTO; ch = buf[pos++];
	if (++loop >= 1000) goto PROTO;
      }
      if (ch >= 64) goto PROTO;
      if (namelen + 1 > sizeof name) goto PROTO; name[namelen++] = ch;
      if (!ch) break;
      state = ch;
    }
  }

  if (!dns_domain_copy(d,name)) return 0;

  if (firstcompress) return firstcompress;
  return pos;

  PROTO:
  errno = EPROTO;
  return 0;
}

/** Extract a series of records from a packet.

    \param out The \c str buffer into which to put the output.
    \param buf The result packet buffer.
    \param len The length of \c buf .
    \param rrtype The resource record type, one of \c DNS_T_*.
    \param rrclass The resource record class, one of \c DNS_C_*.
    \param fn The function to call for each record found in the
    packet. It is responsible to sanity check the record length and
    append it to \c out.
*/
int dns_packet_extract(str* out, const char* buf, unsigned int len, uint16 rrtype, uint16 rrclass,
		       int (*fn)(str* out, const char* buf, unsigned int len, unsigned int pos, uint16 datalen))
{
  unsigned int pos;
  unsigned char header[12];
  uint16 numanswers;
  uint16 datalen;
  int r;

  if (!str_copys(out,"")) return -1;

  pos = dns_packet_copy(buf,len,0,header,12); if (!pos) return -1;
  numanswers = uint16_get_msb(header + 6);
  pos = dns_packet_skipname(buf,len,pos); if (!pos) return -1;
  pos += 4;

  while (numanswers--) {
    pos = dns_packet_skipname(buf,len,pos); if (!pos) return -1;
    pos = dns_packet_copy(buf,len,pos,header,10); if (!pos) return -1;
    datalen = uint16_get_msb(header + 8);
    if (uint16_get_msb(header) == rrtype)
      if (uint16_get_msb(header + 2) == rrclass)
	if ((r = fn(out, buf, len, pos, datalen)) <= 0)
	  return r;
    pos += datalen;
  }

  return 0;
}
