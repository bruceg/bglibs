#ifndef GENERIC_HASH__H__
#define GENERIC_HASH__H__

typedef void ghash_free_fn(void*);
typedef int ghash_copy_fn(void*, const void*);
typedef int ghash_cmp_fn(const void*, const void*);
typedef unsigned long ghash_hash_fn(const void*);

struct ghash
{
  void** table;
  unsigned count;
  unsigned size;
};

#define ghash_entry_hash(P) (*(unsigned long*)(P))
#define ghash_entry_keyptr(P) ((P)+sizeof(unsigned long))
#define ghash_entry_dataptr(P,L) ((P)+sizeof(unsigned long)+(L))
#define ghash_entry_ptr(P,O) ((P)+O)

unsigned long ghash_hashb(const unsigned char*, unsigned long);
unsigned long ghash_hashs(const unsigned char*);

void ghash_insert(struct ghash* d, void* e);
int ghash_add(struct ghash* d, unsigned keysize, unsigned totalsize,
	      unsigned long hash, const void* key, const void* data,
	      ghash_copy_fn* keycopy,
	      ghash_copy_fn* datacopy,
	      ghash_free_fn* keyfree);
void ghash_free(struct ghash* d,
		ghash_free_fn* keyfree, unsigned keysize,
		ghash_free_fn* datafree);
void* ghash_get(struct ghash* d, const void* key, unsigned long hash,
		ghash_cmp_fn* keycmp);
void ghash_init(struct ghash* d);
int ghash_rehash(struct ghash* d, ghash_hash_fn* hash);
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
extern int PREFIX##_rehash(struct ghash* d); \
extern void PREFIX##_foreach(struct ghash* d, \
                             void (*fn)(struct PREFIX##_entry*)); \
extern struct PREFIX##_entry* PREFIX##_search(struct ghash* d, \
                                              int (*fn)(const struct PREFIX##_entry*));

#define GHASH_INIT_DEFN(PREFIX) \
void PREFIX##_init(struct ghash* d) { \
  ghash_init(d); \
}

#define GHASH_FREE_DEFN(PREFIX,KFREE,KTYPE,DFREE) \
void PREFIX##_free(struct ghash* d) { \
  ghash_free(d,(ghash_free_fn*)KFREE, \
	     GHASH_KEYSIZE(PREFIX),(ghash_free_fn*)DFREE); \
}

#define GHASH_ADD_DEFN(PREFIX,KTYPE,DTYPE,HASHFN,KCOPY,DCOPY,KFREE) \
int PREFIX##_add(struct ghash* d, KTYPE const* key, DTYPE const* data) { \
  return ghash_add(d, GHASH_KEYSIZE(PREFIX), sizeof(struct PREFIX##_entry), \
                   HASHFN(key), key, data, \
                   (ghash_copy_fn*)KCOPY, (ghash_copy_fn*)DCOPY, \
		   (ghash_free_fn*)KFREE); \
}

#define GHASH_GET_DEFN(PREFIX,KTYPE,HASHFN,CMPFN) \
struct PREFIX##_entry* PREFIX##_get(struct ghash* d, KTYPE const* key) { \
  return ghash_get(d, key, HASHFN(key), (ghash_cmp_fn*)CMPFN); \
}

#define GHASH_REHASH_DEFN(PREFIX,HASHFN) \
int PREFIX##_rehash(struct ghash* d) { \
  return ghash_rehash(d, (ghash_hash_fn*)HASHFN); \
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
GHASH_INIT_DEFN(PREFIX) \
GHASH_FREE_DEFN(PREFIX,KFREE,KTYPE,DFREE) \
GHASH_ADD_DEFN(PREFIX,KTYPE,DTYPE,HASHFN,KCOPY,DCOPY,KFREE) \
GHASH_GET_DEFN(PREFIX,KTYPE,HASHFN,CMPFN) \
GHASH_REHASH_DEFN(PREFIX,HASHFN) \
GHASH_FOREACH_DEFN(PREFIX) \
GHASH_SEARCH_DEFN(PREFIX)

#endif
