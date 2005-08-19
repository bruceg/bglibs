/* $Id$ */
#ifndef STR_ITER__H__
#define STR_ITER__H__

struct str;

/** \defgroup striter striter: Iterator over substrings of a str.

\par Calling Convention
All non-void functions return \c 0 (false) if the function failed or the
iterator is no longer valid, and non-zero (true) otherwise.

@{ */

/** The basic striter data type. */
typedef struct
{
  /** A pointer to the str being iterated over. */
  const struct str* str;
  /** The starting offset of the current substring. */
  unsigned start;
  /** Pointer to the current substring. */
  const char* startptr;
  /** Length of the current substring. */
  unsigned len;
  /** The seperator used to delimit the substrings. */
  char sep;
} striter;

void striter_start(striter* iter, const struct str* s, char sep);
int striter_valid(striter*);
int striter_advance(striter*);

/** Assign the current striter substring to a str. */
#define str_copyiter(STR,ITER) str_copyb(STR,(ITER)->startptr,(ITER)->len)

/** Append the current striter substring to a str. */
#define str_catiter(STR,ITER) str_catb(STR,(ITER)->startptr,(ITER)->len)

/** Output the current striter substring to an obuf. */
#define obuf_putiter(OBUF,ITER) obuf_write(OBUF,(ITER)->startptr,(ITER)->len)

/** Loop macro; use in place of \c for(...) */
#define striter_loop(ITER,STR,SEP) for(striter_start(ITER,STR,SEP);striter_valid(ITER);striter_advance(ITER))

/** @} */

#endif
