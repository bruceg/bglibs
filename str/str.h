#ifndef STR__H__
#define STR__H__

/* STR_BLOCKSIZE should be set to the at least size at which the
 * allocator (ie malloc) will align the data it returns.  String data
 * will be allocated in steps of this number. */
#define STR_BLOCKSIZE 16

struct str
{
  char* s;
  unsigned len;
  unsigned size;
};
typedef struct str str;

/* Overhead */
int str_init(str* s);
int str_ready(str* s, unsigned size);
void str_free(str* s);

/* Assignment */
int str_copy(str* s, const str* in);
int str_copys(str* s, const char* in);
int str_copyb(str* s, const char* in, unsigned len);

/* Appending */
int str_cat(str* s, const str* in);
int str_cats(str* s, const char* in);
int str_catc(str* s, char in);
int str_catb(str* s, const char* in, unsigned len);
#define str_cati(S,I) str_catiw((S),(I),0,0)
int str_catiw(str* s, long in, unsigned width, char pad);
#define str_catu(S,I) str_catuw((S),(I),0,0)
int str_catuw(str* s, unsigned long in, unsigned width, char pad);

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

/* Searching */
void str_buildmap(int map[256], const char* list);
unsigned str_count(const str* s, char ch);
unsigned str_countof(const str* s, const char* list);
#define str_findfirst(S,C) str_findnext(S,C,0)
#define str_findfirstof(S,L) str_findnextof(S,L,0)
#define str_findlast(S,C) str_findprev(S,C,-1)
#define str_findlastof(S,C) str_findprevof(S,L,-1)
int str_findnext(const str* s, char ch, unsigned pos);
int str_findnextof(const str* s, const char* list, unsigned pos);
int str_findprev(const str* s, char ch, unsigned pos);
int str_findprevof(const str* s, const char* list, unsigned pos);

#endif
