#ifndef DICT__H__
#define DICT__H__

#include "str/str.h"
#include "uint32.h"

#define DICT_HASHSTART 5381
uint32 dict_hashadd(uint32 h, const unsigned char* data, unsigned long len);
#define dict_hash(BUF,LEN) dict_hashadd(DICT_HASHSTART,BUF,LEN)
#define dict_hashstr(STR) dict_hashadd(DICT_HASHSTART,(STR)->s,(STR)->len)

struct dict_entry
{
  uint32 hash;
  str key;
  void* data;
};
typedef struct dict_entry dict_entry;

struct dict
{
  unsigned size;
  unsigned count;
  dict_entry** table;
};
typedef struct dict dict;

extern int dict_init(dict*);
extern void dict_free(dict*, void (*free_data)(void*));
extern int dict_add(dict*, const str*, void*);
extern dict_entry* dict_get(dict*, const str*);

#endif
