#ifndef BGLIBS__ADT__COMMON__H__
#define BGLIBS__ADT__COMMON__H__

typedef void adt_free_fn(void*);
typedef int adt_copy_fn(void*, const void*);
typedef int adt_cmp_fn(const void*, const void*);
typedef unsigned long adt_hash_fn(const void*);

#endif
