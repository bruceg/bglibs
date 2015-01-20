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

    \param out The result buffer into which to put the output.
    \param buf The packet buffer to parse.
    \param len The length of \c buf .
    \param rrtype The resource record type, one of \c DNS_T_*.
    \param rrclass The resource record class, one of \c DNS_C_*.
    \param sizefn The function to call to determine the required
    buffer size for each record.
    \param copy The function to call to copy each record into the result.
*/
int dns_packet_extract(struct dns_result* out, const char* buf, unsigned int len, uint16 rrtype, uint16 rrclass,
		       int (*sizefn)(const char* buf, unsigned int len, unsigned int pos, uint16 datalen),
		       int (*copy)(struct dns_result* out, unsigned int index, unsigned int offset,
				   const char* buf, unsigned int len, unsigned int pos, uint16 datalen))
{
  unsigned int pos;
  unsigned int start;
  unsigned char header[12];
  uint16 numanswers;
  uint16 datalen;
  unsigned int i;
  unsigned int size;
  int r;

  pos = dns_packet_copy(buf,len,0,header,12); if (!pos) return -1;
  numanswers = uint16_get_msb(header + 6);
  pos = dns_packet_skipname(buf,len,pos); if (!pos) return -1;
  start = pos += 4;

  /* Count the number of records */
  for (i = size = 0; numanswers > 0; pos += datalen, --numanswers) {
    pos = dns_packet_skipname(buf,len,pos); if (!pos) return -1;
    pos = dns_packet_copy(buf,len,pos,header,10); if (!pos) return -1;
    datalen = uint16_get_msb(header + 8);
    if (uint16_get_msb(header) == rrtype && uint16_get_msb(header + 2) == rrclass) {
      if (sizefn) {
	if ((r = sizefn(buf, len, pos, datalen)) < 0) {
	  errno = EPROTO;
	  return -1;
	}
	size += r;
      }
      ++i;
    }
  }

  if (dns_result_alloc(out, rrtype, i, size) < 0)
    return -1;

  /* Copy the records into out */
  pos = start;
  numanswers = i;
  for (i = size = 0; i < numanswers; pos += datalen) {
    pos = dns_packet_skipname(buf,len,pos); if (!pos) return -1;
    pos = dns_packet_copy(buf,len,pos,header,10); if (!pos) return -1;
    datalen = uint16_get_msb(header + 8);
    if (uint16_get_msb(header) == rrtype && uint16_get_msb(header + 2) == rrclass) {
      if ((r = copy(out, i, size, buf, len, pos, datalen)) < 0)
	return -1;
      size += r;
      ++i;
    }
  }
  return 0;
}
