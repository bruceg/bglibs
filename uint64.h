#ifndef UINT64_H
#define UINT64_H

#include <sysdeps.h>
__UINTNN_DECL(64,8)
#define uint64_get uint64_get_lsb
#define uint64_pack uint64_pack_lsb
#define uint64_unpack uint64_unpack_lsb

#endif
