#ifndef BGLIBS__GENERIC_HASH__H__
#define BGLIBS__GENERIC_HASH__H__

#include <adt/common.h>

struct ghash
{
  void** table;
  unsigned count;
  unsigned size;

  unsigned long keysize;
  unsigned long entrysize;

  adt_hash_fn* hashfn;
  adt_cmp_fn* keycmp;
  adt_copy_fn* keycopy;
  adt_copy_fn* datacopy;
  adt_free_fn* keyfree;
  adt_free_fn* datafree;
};

#define ghash_entry_hash(P) (*(unsigned long*)(P))
#define ghash_entry_keyptr(P) ((P)+sizeof(unsigned long))
#define ghash_entry_dataptr(P,L) ((P)+sizeof(unsigned long)+(L))
#define ghash_entry_ptr(P,O) ((P)+O)

unsigned long ghash_hashb(const unsigned char*, unsigned long);
unsigned long ghash_hashs(const unsigned char*);
unsigned long ghash_hashsp(const char* const* p);

void ghash_insert(struct ghash* d, void* e);
int ghash_add(struct ghash* d, const void* key, const void* data);
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
void ghash_foreach(struct ghash* d, void (*fn)(void* entry));
void* ghash_search(struct ghash* d, int (*fn)(const void* entry));

#define GHASH_STRUCT_ENTRY(PREFIX,KTYPE,DTYPE) \
struct PREFIX##_entry { \
  unsigned long hash; \
  KTYPE key; \
  DTYPE data; \
}

#define GHASH_KEYOFFSET(PREFIX) ((unsigned)&((struct PREFIX##_entry*)0)->key)
#define GHASH_DATAOFFSET(PREFIX) ((unsigned)&((struct PREFIX##_entry*)0)->data)
#define GHASH_KEYSIZE(PREFIX) ( \
  GHASH_DATAOFFSET(PREFIX)-GHASH_KEYOFFSET(PREFIX) \
)

#define GHASH_DECL(PREFIX,KTYPE,DTYPE) \
GHASH_STRUCT_ENTRY(PREFIX,KTYPE,DTYPE); \
extern void PREFIX##_init(struct ghash* d); \
extern void PREFIX##_free(struct ghash* d); \
extern int PREFIX##_add(struct ghash* d, \
                        KTYPE const* key, DTYPE const* data); \
extern struct PREFIX##_entry* PREFIX##_get(struct ghash* d, \
                                           KTYPE const* key); \
extern int PREFIX##_rebuild(struct ghash* d); \
extern int PREFIX##_rehash(struct ghash* d); \
extern void PREFIX##_foreach(struct ghash* d, \
                             void (*fn)(struct PREFIX##_entry*)); \
extern struct PREFIX##_entry* PREFIX##_search(struct ghash* d, \
                                              int (*fn)(const struct PREFIX##_entry*));

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

#define GHASH_FREE_DEFN(PREFIX) \
void PREFIX##_free(struct ghash* d) { \
  ghash_free(d); \
}

#define GHASH_ADD_DEFN(PREFIX,KTYPE,DTYPE) \
int PREFIX##_add(struct ghash* d, KTYPE const* key, DTYPE const* data) { \
  return ghash_add(d, key, data); \
}

#define GHASH_GET_DEFN(PREFIX,KTYPE) \
struct PREFIX##_entry* PREFIX##_get(struct ghash* d, KTYPE const* key) { \
  return ghash_get(d, key); \
}

#define GHASH_REBUILD_DEFN(PREFIX) \
int PREFIX##_rebuild(struct ghash* d) { \
  return ghash_rebuild(d); \
}

#define GHASH_REHASH_DEFN(PREFIX) \
int PREFIX##_rehash(struct ghash* d) { \
  return ghash_rehash(d); \
}

#define GHASH_FOREACH_DEFN(PREFIX) \
void PREFIX##_foreach(struct ghash* d, void (*fn)(struct PREFIX##_entry*)) { \
  ghash_foreach(d, (void (*)(void*))fn); \
}

#define GHASH_SEARCH_DEFN(PREFIX) \
struct PREFIX##_entry* PREFIX##_search(struct ghash* d, int (*fn)(const struct PREFIX##_entry*)) { \
  return ghash_search(d, (int (*)(const void*))fn); \
}

#define GHASH_DEFN(PREFIX,KTYPE,DTYPE,HASHFN,CMPFN,KCOPY,DCOPY,KFREE,DFREE) \
GHASH_INIT_DEFN(PREFIX,KTYPE,DTYPE,HASHFN,CMPFN,KCOPY,DCOPY,KFREE,DFREE) \
GHASH_FREE_DEFN(PREFIX) \
GHASH_ADD_DEFN(PREFIX,KTYPE,DTYPE) \
GHASH_GET_DEFN(PREFIX,KTYPE) \
GHASH_REHASH_DEFN(PREFIX) \
GHASH_FOREACH_DEFN(PREFIX) \
GHASH_SEARCH_DEFN(PREFIX)

struct ghashiter
{
  const struct ghash* ghashp;
  unsigned index;
  void* entry;
};

void ghashiter_first(struct ghashiter*, const struct ghash*);
int ghashiter_valid(const struct ghashiter*);
void ghashiter_next(struct ghashiter*);
#define ghashiter_loop(I,G) \
  for(ghashiter_first(I,G);ghashiter_valid(I);ghashiter_next(I))

#endif
