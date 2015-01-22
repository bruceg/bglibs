#ifndef BGLIBS__GENERIC_HASH__H__
#define BGLIBS__GENERIC_HASH__H__

#include "adt_common.h"

/** \defgroup adt_ghash adt ghash: Generic hash tables.
 *
 * The actual hash table manipulation functions use a simple linear
 * probing algorithm with a dynamic table size.  Since many more slots
 * are allocated than are in use at any given time, there are always
 * plenty of empty slots available to make searches short.  Since each
 * shot is a single pointer, these extra slots don't consume a
 * significant amount of memory.
 *
 * The structure at the center of the \c ghash implementation contains
 * pointers to the actual data (including all relevant metadata) plus
 * function pointers to the functions needed to manipulate the data.
 */

/** @{ */

/** \example adt/ghash_test.c */

/** The core generic hash table structure. */
struct ghash
{
  /** The hash table itself is an array of pointers, the actual type of
   * which is dependant on the declaration of the table. */
  void** table;
  /** The count is the number of items stored in the table. */
  unsigned count;
  /** The size is the number of slots available in the table. */
  unsigned size;

  /** The size, in bytes, of the key structure in each table entry. */
  unsigned long keysize;
  /** The total size, in bytes, of an entry (key and data). */
  unsigned long entrysize;

  /** Pointer to the function which produces a hash of a key. */
  adt_hash_fn* hashfn;
  /** Pointer to the function which compares two keys. */
  adt_cmp_fn* keycmp;
  /** Pointer to the function which copies a key. */
  adt_copy_fn* keycopy;
  /** Pointer to the function which copies a data item. */
  adt_copy_fn* datacopy;
  /** Pointer to the function which frees a key. */
  adt_free_fn* keyfree;
  /** Pointer to the function which fressa a data item. */
  adt_free_fn* datafree;
};

/** The hash value stored within an entry \c P . */
#define ghash_entry_hash(P) (*(adt_hash_t*)(P))
/** The key structure address within an entry \c P . */
#define ghash_entry_keyptr(P) ((P)+sizeof(adt_hash_t))
/** The data structure address within an entry \c P . The offset
 * parameter \c L is the size of the key structure. */
#define ghash_entry_dataptr(P,L) ((P)+sizeof(adt_hash_t)+(L))

/** The \c adt_hashb function also works for ghash */
#define ghash_hashb adt_hashb
/** The \c adt_hashs function also works for ghash */
#define ghash_hashs adt_hashs
/** The \c adt_hashsp function also works for ghash */
#define ghash_hashsp adt_hashsp

void ghash_insert(struct ghash* d, void* e);
void* ghash_add(struct ghash* d, const void* key, const void* data);
void* ghash_set(struct ghash* d, const void* key, const void* data);
void ghash_free(struct ghash* d);
void** ghash_find(struct ghash* d, const void* key);
void* ghash_get(struct ghash* d, const void* key);
void ghash_init(struct ghash* d,
		unsigned long keysize,
		unsigned long entrysize,
		adt_hash_fn* hashfn,
		adt_cmp_fn* keycmp,
		adt_copy_fn* keycopy,
		adt_copy_fn* datacopy,
		adt_free_fn* keyfree,
		adt_free_fn* datafree);
int ghash_rebuild(struct ghash* d);
int ghash_rehash(struct ghash* d);
int ghash_remove(struct ghash* d, const void* key);
void ghash_foreach(struct ghash* d, void (*fn)(void* entry));
void* ghash_search(struct ghash* d, int (*fn)(const void* entry));

/** Prototype for the \c ghash entry structure, containing a key of type
 * \c KTYPE and data of type \c DTYPE . */
#define GHASH_STRUCT_ENTRY(PREFIX,KTYPE,DTYPE) \
struct PREFIX##_entry { \
  adt_hash_t hash; \
  KTYPE key; \
  DTYPE data; \
}

/** The offset of the key into a specialized \c ghash entry. */
#define GHASH_KEYOFFSET(PREFIX) ((unsigned long)&((struct PREFIX##_entry*)0)->key)
/** The offset of the data into a specialized \c ghash entry. */
#define GHASH_DATAOFFSET(PREFIX) ((unsigned long)&((struct PREFIX##_entry*)0)->data)
/** The actual size of the key data in a specialized \c ghash entry. */
#define GHASH_KEYSIZE(PREFIX) ( \
  GHASH_DATAOFFSET(PREFIX)-GHASH_KEYOFFSET(PREFIX) \
)

/** Declare all the prototypes for a \c ghash table, specialized to
 * particular key and data types. */
#define GHASH_DECL(PREFIX,KTYPE,DTYPE) \
GHASH_STRUCT_ENTRY(PREFIX,KTYPE,DTYPE); \
extern void PREFIX##_init(struct ghash* d); \
extern void PREFIX##_free(struct ghash* d); \
extern struct PREFIX##_entry* PREFIX##_add(struct ghash* d, \
                                           KTYPE const* key, \
                                           DTYPE const* data); \
extern struct PREFIX##_entry* PREFIX##_set(struct ghash* d, \
                                           KTYPE const* key, \
                                           DTYPE const* data); \
extern struct PREFIX##_entry* PREFIX##_get(struct ghash* d, \
                                           KTYPE const* key); \
extern int PREFIX##_rebuild(struct ghash* d); \
extern int PREFIX##_rehash(struct ghash* d); \
extern int PREFIX##_remove(struct ghash* d, KTYPE const* key); \
extern void PREFIX##_foreach(struct ghash* d, \
                             void (*fn)(struct PREFIX##_entry*)); \
extern struct PREFIX##_entry* PREFIX##_search(struct ghash* d, \
                                              int (*fn)(const struct PREFIX##_entry*));

/** Define a specialized \c ghash table init function. */
#define GHASH_INIT_DEFN(PREFIX,KTYPE,DTYPE,HASHFN,CMP,KCOPY,DCOPY,KFREE,DFREE)\
void PREFIX##_init(struct ghash* d) { \
  ghash_init(d, \
	     GHASH_KEYSIZE(PREFIX), \
	     sizeof(struct PREFIX##_entry), \
	     (adt_hash_fn*)HASHFN, \
	     (adt_cmp_fn*)CMP, \
	     (adt_copy_fn*)KCOPY, \
	     (adt_copy_fn*)DCOPY, \
	     (adt_free_fn*)KFREE, \
	     (adt_free_fn*)DFREE); \
}

/** Define a specialized \c ghash table free function. */
#define GHASH_FREE_DEFN(PREFIX) \
void PREFIX##_free(struct ghash* d) { \
  ghash_free(d); \
}

/** Define a specialized \c ghash table add function. */
#define GHASH_ADD_DEFN(PREFIX,KTYPE,DTYPE) \
struct PREFIX##_entry* PREFIX##_add(struct ghash* d, \
                                    KTYPE const* key, DTYPE const* data) { \
  return ghash_add(d, key, data); \
}

/** Define a specialized \c ghash table add function. */
#define GHASH_SET_DEFN(PREFIX,KTYPE,DTYPE) \
struct PREFIX##_entry* PREFIX##_set(struct ghash* d, \
                                    KTYPE const* key, DTYPE const* data) { \
  return ghash_set(d, key, data); \
}

/** Define a specialized \c ghash table get function. */
#define GHASH_GET_DEFN(PREFIX,KTYPE) \
struct PREFIX##_entry* PREFIX##_get(struct ghash* d, KTYPE const* key) { \
  return ghash_get(d, key); \
}

/** Define a specialized \c ghash table rebuild function. */
#define GHASH_REBUILD_DEFN(PREFIX) \
int PREFIX##_rebuild(struct ghash* d) { \
  return ghash_rebuild(d); \
}

/** Define a specialized \c ghash table rehash function. */
#define GHASH_REHASH_DEFN(PREFIX) \
int PREFIX##_rehash(struct ghash* d) { \
  return ghash_rehash(d); \
}

/** Define a specialized \c ghash table remove function. */
#define GHASH_REMOVE_DEFN(PREFIX,KTYPE) \
extern int PREFIX##_remove(struct ghash* d, KTYPE const* key) { \
  return ghash_remove(d, (void*)key); \
}

/** Define a specialized \c ghash table iterator function. */
#define GHASH_FOREACH_DEFN(PREFIX) \
void PREFIX##_foreach(struct ghash* d, void (*fn)(struct PREFIX##_entry*)) { \
  ghash_foreach(d, (void (*)(void*))fn); \
}

/** Define a specialized \c ghash table search function. */
#define GHASH_SEARCH_DEFN(PREFIX) \
struct PREFIX##_entry* PREFIX##_search(struct ghash* d, int (*fn)(const struct PREFIX##_entry*)) { \
  return ghash_search(d, (int (*)(const void*))fn); \
}

/** Define all necessary functions for a specialized \c ghash table. If
 * either of the copy functions \c KCOPY or \c DCOPY are \c NULL, a
 * simple memcpy is used instead.  If either of the free functions \c
 * KFREE or \c DFREE are NULL, no data freeing is attempted. */
#define GHASH_DEFN(PREFIX,KTYPE,DTYPE,HASHFN,CMPFN,KCOPY,DCOPY,KFREE,DFREE) \
GHASH_INIT_DEFN(PREFIX,KTYPE,DTYPE,HASHFN,CMPFN,KCOPY,DCOPY,KFREE,DFREE) \
GHASH_FREE_DEFN(PREFIX) \
GHASH_ADD_DEFN(PREFIX,KTYPE,DTYPE) \
GHASH_SET_DEFN(PREFIX,KTYPE,DTYPE) \
GHASH_GET_DEFN(PREFIX,KTYPE) \
GHASH_REBUILD_DEFN(PREFIX) \
GHASH_REHASH_DEFN(PREFIX) \
GHASH_REMOVE_DEFN(PREFIX,KTYPE) \
GHASH_FOREACH_DEFN(PREFIX) \
GHASH_SEARCH_DEFN(PREFIX)

/** The iterator data necessary to loop over a \c ghash table. */
struct ghashiter
{
  /** A pointer to the source \c ghash table. */
  const struct ghash* ghashp;
  /** The index of the current entry. */
  unsigned index;
  /** A pointer to the current entry, for convenience. */
  void* entry;
};

void ghashiter_first(struct ghashiter*, const struct ghash*);
int ghashiter_valid(const struct ghashiter*);
void ghashiter_next(struct ghashiter*);
/** A convenience macro which expands to a \c for loop using the \c
 * ghashiter I to iterate over the table \c G . */
#define ghashiter_loop(I,G) \
  for(ghashiter_first(I,G);ghashiter_valid(I);ghashiter_next(I))

/** @} */

#endif
