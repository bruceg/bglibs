#ifndef UINT16_H
#define UINT16_H

#include <sysdeps.h>
#include <uintnn.h>
__UINTNN_DECL(16,2)
#define uint16_get uint16_get_lsb
#define uint16_pack uint16_pack_lsb
#define uint16_unpack uint16_unpack_lsb

#endif
