#ifndef STR__H__
#define STR__H__

/* STR_BLOCKSIZE should be set to the at least size at which the
 * allocator (ie malloc) will align the data it returns.  String data
 * will be allocated in steps of this number.  Values smaller than the
 * alignment will only cause a small amount of space to be wasted, and
 * will not trigger bugs. */
#define STR_BLOCKSIZE 16

struct str
{
  char* s;
  unsigned len;
  unsigned size;
};
typedef struct str str;

struct str_sortentry
{
  const char* str;
  unsigned long len;
};
typedef struct str_sortentry str_sortentry;

/* Overhead */
int str_init(str* s);
int str_alloc(str* s, unsigned size, int copy);
#define str_ready(S,SZ) str_alloc(S,SZ,0)
#define str_realloc(S,SZ) str_alloc(S,SZ,1)
void str_free(str* s);
int str_truncate(str* s, unsigned len);

/* Assignment */
int str_copy(str* s, const str* in);
int str_copys(str* s, const char* in);
int str_copyb(str* s, const char* in, unsigned len);
int str_copy2s(str* s, const char* a, const char* b);
int str_copy3s(str* s, const char* a, const char* b, const char* c);
int str_copy4s(str* s, const char* a, const char* b, const char* c, const char* d);
int str_copy5s(str* s, const char* a, const char* b, const char* c, const char* d, const char* e);
int str_copy6s(str* s, const char* a, const char* b, const char* c, const char* d, const char* e, const char* f);

/* Appending */
int str_cat(str* s, const str* in);
int str_cats(str* s, const char* in);
int str_catc(str* s, char in);
int str_catb(str* s, const char* in, unsigned len);
#define str_cati(S,I) str_catiw((S),(I),0,0)
int str_catiw(str* s, long in, unsigned width, char pad);
#define str_catu(S,I) str_catuw((S),(I),0,0)
int str_catuw(str* s, unsigned long in, unsigned width, char pad);
#define str_catx(S,I) str_catxw((S),(I),0,0)
int str_catxw(str* s, unsigned long in, unsigned width, char pad);
#define str_catill(S,I) str_catiw((S),(I),0,0)
int str_catiwll(str* s, long long in, unsigned width, char pad);
#define str_catull(S,I) str_catuw((S),(I),0,0)
int str_catuwll(str* s, unsigned long long in, unsigned width, char pad);
int str_cat2s(str* s, const char* a, const char* b);
int str_cat3s(str* s, const char* a, const char* b, const char* c);
int str_cat4s(str* s, const char* a, const char* b, const char* c, const char* d);
int str_cat5s(str* s, const char* a, const char* b, const char* c, const char* d, const char* e);
int str_cat6s(str* s, const char* a, const char* b, const char* c, const char* d, const char* e, const char* f);

int str_join(str* s, char sep, const str* t);
int str_joins(str* s, char sep, const char* in);
int str_joinb(str* s, char sep, const char* in, unsigned len);

/* In-place modifications */
void str_lower(str* s);
void str_upper(str* s);
void str_subst(str* s, char from, char to);
void str_lstrip(str* s);
void str_rstrip(str* s);
#define str_strip(S) (str_rstrip(S), str_lstrip(S))
void str_lcut(str* s, unsigned count);
void str_rcut(str* s, unsigned count);
int str_sort(str* s, char sep, long count,
	     int (*fn)(const str_sortentry* a, const str_sortentry* b));

/* Comparison */
int str_cmp(const str* a, unsigned aoffset, const str* b, unsigned boffset);
int str_cmps(const str* a, unsigned offset, const char* b);
int str_cmpb(const str* a, unsigned offset, const char* b, unsigned len);

int str_diff(const str* a, const str* b);
int str_diffs(const str* a, const char* b);
int str_diffb(const str* a, const char* b, unsigned len);

/* Searching */
void str_buildmap(int map[256], const char* list);
unsigned str_count(const str* s, char ch);
unsigned str_countof(const str* s, const char* list);
#define str_findfirst(S,C) str_findnext(S,C,0)
#define str_findfirstof(S,L) str_findnextof(S,L,0)
#define str_findfirstnot(S,L) str_findnextnot(S,L,0)
#define str_findlast(S,C) str_findprev(S,C,-1)
#define str_findlastof(S,L) str_findprevof(S,L,-1)
#define str_findlastnot(S,L) str_findprevof(S,L,-1)
int str_findnext(const str* s, char ch, unsigned pos);
int str_findnextof(const str* s, const char* list, unsigned pos);
int str_findnextnot(const str* s, const char* list, unsigned pos);
int str_findprev(const str* s, char ch, unsigned pos);
int str_findprevof(const str* s, const char* list, unsigned pos);
int str_findprevnot(const str* s, const char* list, unsigned pos);

/* Pattern match */
int str_match(const str* s, const char* pattern);

#endif
