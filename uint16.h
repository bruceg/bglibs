#ifndef UINT16_H
#define UINT16_H

typedef unsigned short uint16;

extern void uint16_pack_lsb(uint16, unsigned char[2]);
extern void uint16_pack_msb(uint16, unsigned char[2]);
extern void uint16_unpack_lsb(const unsigned char[2], uint16*);
extern void uint16_unpack_msb(const unsigned char[2], uint16*);
#define uint16_pack uint16_pack_lsb
#define uint16_unpack uint16_unpack_lsb

#endif
