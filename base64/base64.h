/* $Id$ */
#ifndef BASE64__H__
#define BASE64__H__

#include "str/str.h"

#define BASE64_PAD '='

extern const char base64_bin2asc[64];
extern void base64_encode_whole(const unsigned char bin[3], char encoded[4]);
extern void base64_encode_part(const unsigned char bin[3], unsigned len,
			       char encoded[4]);
extern int base64_encode_line(const unsigned char* bin, unsigned long len,
			      str* encoded);

extern const char base64_asc2bin[256];
extern int base64_decode_part(const unsigned char encoded[4],
			      unsigned char bin[3]);
extern int base64_decode_line(const char* encoded, str* bin);

#endif
