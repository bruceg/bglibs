#ifndef UINT16_H
#define UINT16_H

typedef unsigned short uint16;

extern void uint16_pack_lsb(uint16, unsigned char*);
extern void uint16_pack_msb(uint16, unsigned char*);
extern void uint16_unpack_lsb(const unsigned char*, uint16*);
extern void uint16_unpack_msb(const unsigned char*, uint16*);
#define uint16_pack_lsb uint16_pack
#define uint16_unpack_lsb uint16_unpack

#endif
