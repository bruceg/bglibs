#ifndef UINT64_H
#define UINT64_H

#include "sysdeps.h"
#include "uintnn.h"
__UINTNN_DECL(64,8)
#define uint64_get uint64_get_lsb
#define uint64_pack uint64_pack_lsb
#define uint64_unpack uint64_unpack_lsb

#ifdef ENDIAN_LSB
#define uint64_get_native uint64_get_lsb
#define uint64_pack_native uint64_pack_lsb
#define uint64_unpack_native uint64_unpack_lsb
#else
#define uint64_get_native uint64_get_msb
#define uint64_pack_native uint64_pack_msb
#define uint64_unpack_native uint64_unpack_msb
#endif

#endif
