#ifndef STR__H__
#define STR__H__

#define STR_MINSIZE 16
#define STR_ROUNDSIZE 16

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
int str_copy(str* s, str* in);
int str_copys(str* s, const char* in);
int str_copyb(str* s, const char* in, unsigned len);

/* Appending */
int str_cat(str* s, str* in);
int str_cats(str* s, const char* in);
int str_catc(str* s, char in);
int str_catb(str* s, const char* in, unsigned len);
#define str_cati(S,I) str_catiw((S),(I),0,0)
int str_catiw(str* s, long in, unsigned width, char pad);
#define str_catu(S,I) str_catuw((S),(I),0,0)
int str_catuw(str* s, unsigned long in, unsigned width, char pad);

/* Searching */
void str_buildmap(int map[256], const char* list);
int str_findfirst(str* s, char ch);
int str_findfirstof(str* s, const char* list);
int str_findlast(str* s, char ch);
int str_findlastof(str* s, const char* list);
int str_findnext(str* s, char ch, unsigned pos);
int str_findnextof(str* s, const char* list, unsigned pos);
int str_findprev(str* s, char ch, unsigned pos);
int str_findprevof(str* s, const char* list, unsigned pos);

#endif
