/* $Id$ */
#ifndef BGLIBS__ADT__COMMON__H__
#define BGLIBS__ADT__COMMON__H__

typedef unsigned long adt_hash_t;
typedef void adt_free_fn(void*);
typedef int adt_copy_fn(void*, const void*);
typedef int adt_cmp_fn(const void*, const void*);
typedef adt_hash_t adt_hash_fn(const void*);

adt_hash_t adt_hashb(const unsigned char*, unsigned long);

adt_hash_t adt_hashs(const unsigned char*);

adt_hash_t adt_hashsp(const char* const*);
int adt_cmpsp(const char* const* a, const char* const* b);
int adt_copysp(const char** a, const char* const* b);
void adt_freesp(const char** a);

#endif
