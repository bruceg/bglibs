#ifndef BGLIBS__ADT__COMMON__H__
#define BGLIBS__ADT__COMMON__H__

/** \defgroup adt adt: Abstract Data Types. */

/** @{ */

/** The hash value type. All hash functions must output a value of this
 * type, and all functions that use a hash store it in this type. */
typedef unsigned long adt_hash_t;
/** A function prototype used for freeing something. */
typedef void adt_free_fn(void*);
/** A function prototype for copying something. The function is expected
 * to return false (zero) if the copy failed for any reason (ie could
 * not allocate memory). */
typedef int adt_copy_fn(void*, const void*);
/** A function prototype for comparing two items. Such functions must
 * return zero if the two items are identical, positive if the first is
 * larger, and negative if the first is smaller. */
typedef int adt_cmp_fn(const void*, const void*);
/** A function prototype for hasing an item (typically a key). */
typedef adt_hash_t adt_hash_fn(const void*);

/** A sample function for hasing a block of memory. */
adt_hash_t adt_hashb(const unsigned char*, unsigned long);

/** A sample function for hashing a C string. */
adt_hash_t adt_hashs(const unsigned char*);

/** A sample function for hashing a C string pointer. */
adt_hash_t adt_hashsp(const char* const*);
/** A sample function for comparing two C string pointers. */
int adt_cmpsp(const char* const* a, const char* const* b);
/** A sample function for copying a C string pointer. */
int adt_copysp(const char** a, const char* const* b);
/** A sample function for freeing a C string pointer. */
void adt_freesp(const char** a);

/** @} */

#endif
