#ifndef GENERIC_CRC__H__
#define GENERIC_CRC__H__

#include <sysdeps.h>

uint16 gcrc16fwd(uint16, const char*, long len, const uint16[256]);
uint16 gcrc16rfl(uint16, const char*, long len, const uint16[256]);
uint32 gcrc32fwd(uint32, const char*, long len, const uint32[256]);
uint32 gcrc32rfl(uint32, const char*, long len, const uint32[256]);
uint64 gcrc64fwd(uint64, const char*, long len, const uint64[256]);
uint64 gcrc64rfl(uint64, const char*, long len, const uint64[256]);

#endif
