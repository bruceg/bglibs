#ifndef UINT32_H
#define UINT32_H

#include "sysdeps.h"
#include "uintnn.h"
__UINTNN_DECL(32,4)
#define uint32_get uint32_get_lsb
#define uint32_pack uint32_pack_lsb
#define uint32_unpack uint32_unpack_lsb

#ifdef ENDIAN_LSB
#define uint32_get_native uint32_get_lsb
#define uint32_pack_native uint32_pack_lsb
#define uint32_unpack_native uint32_unpack_lsb
#else
#define uint32_get_native uint32_get_msb
#define uint32_pack_native uint32_pack_msb
#define uint32_unpack_native uint32_unpack_msb
#endif

#endif
