#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include <str/str.h>

/* GLOB patterns:
 * *      matches everything
 * ?      matches any single character
 * [seq]  matches any sing echaracter in seq
 * [~seq] matches any single character not in seq
 */

static unsigned make_set(const char* pptr, unsigned plen,
			 char set[256], int ncase)
{
  unsigned orig_plen = plen;
  char value;
  if (plen == 0 || *pptr != '[') return 0;
  ++pptr; --plen;
  if (*pptr == '~') {
    memset(set, 1, 256);
    value = 0;
    ++pptr; --plen;
  }
  else {
    value = 1;
    memset(set, 0, 256);
  }
  while (plen > 0) {
    unsigned char p = *pptr++; --plen;
    if (p == ']') return orig_plen - plen - 1;
    if (p == '\\') { p = *pptr++; --plen; }
    set[p] = value;
    if (ncase) {
      if (isupper(p))
	set[tolower(p)] = value;
      else if (islower(p))
	set[toupper(p)] = value;
    }
  }
  return 0;
}

static int glob_match(const str* s, unsigned offset,
		      const char* pptr, unsigned plen, int ncase);

static int glob_search(const str* s, unsigned offset,
		       const char* pptr, unsigned plen, int ncase)
{
  char start;
  int r;
  while (plen > 0) {
    /* Strip off leading "*"s. */
    if ((start = *pptr) == '*')
      ++pptr, --plen;
    /* Leading "?"s always match a single character, so just advance past. */
    else if (start == '?')
      ++pptr, --plen, ++offset;
    else
      break;
  }
  /* Searching at end of pattern always succeeds. */
  if (plen == 0) return 1;
  /* Searching at end of string (with non-empty pattern) always fails. */
  if (offset >= s->len) return 0;
  /* Optimize for searching for a set by only computing the set once. */
  if ((start = *pptr) == '[') {
    char set[256];
    unsigned len;
    if ((len = make_set(pptr, plen, set, ncase)) == 0) return -1;
    ++len;
    pptr += len; plen -= len;
    for (; offset < s->len; ++offset) {
      if (set[(unsigned char)s->s[offset]] &&
	  (r = glob_match(s, offset+1, pptr, plen, ncase)) != 0)
	return r;
    }
  }
  /* Otherwise search for a normal character. */
  else {
    ++pptr, --plen;
    if (ncase && isupper(start)) start = tolower(start);
    do {
      char c = s->s[offset++];
      if (ncase && isupper(c)) c = tolower(c);
      if (c == start &&
	  (r = glob_match(s, offset, pptr, plen, ncase)) != 0)
	return r;
    } while (offset < s->len);
#if 0
    do {
      if ((r = glob_match(s, offset, pptr, plen, ncase)) != 0)
	return r;
      ++offset;
    } while (offset < s->len);
#endif
  }
#if 0
  else {
    if (ncase && isupper(start)) start = tolower(start);
    for (; offset < s->len; ++offset) {
      char c = s->s[offset];
      if (ncase && isupper(c)) c = tolower(c);
      if (c == start &&
	  (r = glob_match(s, offset+1, pptr+1, plen-1, ncase)) != 0)
	return r;
    }
  }
#endif
  return 0;
}

static int glob_match(const str* s, unsigned offset,
		      const char* pptr, unsigned plen, int ncase)
{
  for (; plen > 0; ++pptr, --plen, ++offset) {
    char p = *pptr;
    if (p == '*')
      return glob_search(s, offset, pptr+1, plen-1, ncase);
    if (offset >= s->len) break;
    switch (p) {
    case '?':
      continue;
    case '[':
      {
	char set[256];
	unsigned len;
	if ((len = make_set(pptr, plen, set, ncase)) == 0) return -1;
	pptr += len; plen -= len;
	if (!set[(unsigned char)s->s[offset]]) return 0;
	continue;
      }
    default:
      {
	char c = s->s[offset];
	if (ncase) {
	  if (isupper(c)) c = tolower(c);
	  if (isupper(p)) p = tolower(p);
	}
	if (p != c) return 0;
      }
    }
  }
  return offset == s->len && plen == 0;
}

int str_globb(const str* s, const char* pptr, unsigned plen, int ncase)
{
  return glob_match(s, 0, pptr, plen, ncase);
}

int str_globs(const str* s, const char* pattern, int ncase)
{
  return glob_match(s, 0, pattern, strlen(pattern), ncase);
}

int str_glob(const str* s, const str* pattern, int ncase)
{
  return glob_match(s, 0, pattern->s, pattern->len, ncase);
}

#ifdef SELFTEST_MAIN
#include "selftest.c"
void t(const char* string, const char* pattern, int ncase)
{
  str s;
  str p;
  s.s = (char*)string;
  s.len = strlen(string);
  s.size = 0;
  p.s = (char*)pattern;
  p.len = strlen(pattern);
  p.size = 0;
  obuf_putstr(&outbuf, &s);
  obuf_putc(&outbuf, ' ');
  obuf_putstr(&outbuf, &p);
  obuf_putc(&outbuf, ' ');
  obuf_puti(&outbuf, ncase);
  obuf_putc(&outbuf, ' ');
  debugfn(str_glob(&s, &p, ncase));
}
MAIN
{
  /* Empty string */
  t("", "", 0);
  t("", "*", 0);
  t("", "**", 0);
  t("", "?", 0);
  t("", "[abc]", 0);
  /* Case sensitivity: single char */
  t("a", "a", 0);
  t("a", "A", 0);
  t("A", "a", 0);
  t("A", "A", 0);
  t("a", "a", 1);
  t("a", "A", 1);
  t("A", "a", 1);
  t("A", "A", 1);
  /* Case sensitivity: normal set */
  t("a", "[a]", 0);
  t("a", "[A]", 0);
  t("A", "[a]", 0);
  t("A", "[A]", 0);
  t("a", "[a]", 1);
  t("a", "[A]", 1);
  t("A", "[a]", 1);
  t("A", "[A]", 1);
  /* Case sensitivity: negated set */
  t("a", "[~a]", 0);
  t("a", "[~A]", 0);
  t("A", "[~a]", 0);
  t("A", "[~A]", 0);
  t("a", "[~a]", 1);
  t("a", "[~A]", 1);
  t("A", "[~a]", 1);
  t("A", "[~A]", 1);
  /* Case sensitivity: search */
  t("a", "*a", 0);
  t("a", "*A", 0);
  t("A", "*a", 0);
  t("A", "*A", 0);
  t("a", "*a", 1);
  t("a", "*A", 1);
  t("A", "*a", 1);
  t("A", "*A", 1);
  /* Miscelaneous matches */
  t("abc", "", 0);
  t("abc", "*", 0);
  t("abc", "**", 0);
  t("abc", "?", 0);
  t("abc", "??", 0);
  t("abc", "???", 0);
  t("abc", "????", 0);
  t("abc", "abc", 0);
  t("abc", "*abc", 0);
  t("abc", "*bc", 0);
  t("abc", "*c", 0);
  t("abc", "?bc", 0);
  t("abc", "a?c", 0);
  t("abc", "ab?", 0);
  t("abc", "[axy][bxy][cxy]", 0);
  /* Three patterns for each case:
   * 1 at start of filename
   * 2 in filename
   * 3 at end of filename
   * Four characters to test:
   * 1 First filename character "a"
   * 2 Second filename character "b"
   * 3 Third filename character "c"
   * 4 Non-filename character "x"
   */
  /* First case: single character */
  t("abc", "a*", 0);
  t("abc", "*a*", 0);
  t("abc", "*a", 0);
  t("abc", "b*", 0);
  t("abc", "*b*", 0);
  t("abc", "*b", 0);
  t("abc", "c*", 0);
  t("abc", "*c*", 0);
  t("abc", "*c", 0);
  t("abc", "x*", 0);
  t("abc", "*x*", 0);
  t("abc", "*x", 0);
  /* Second case: character in a set */
  t("abc", "[axy]*", 0);
  t("abc", "*[axy]*", 0);
  t("abc", "*[axy]", 0);
  t("abc", "[bxy]*", 0);
  t("abc", "*[bxy]*", 0);
  t("abc", "*[bxy]", 0);
  t("abc", "[cxy]*", 0);
  t("abc", "*[cxy]*", 0);
  t("abc", "*[cxy]", 0);
  t("abc", "[xyz]*", 0);
  t("abc", "*[xyz]*", 0);
  t("abc", "*[xyz]", 0);
  /* Third case: negated set */
  t("abc", "[~a]*", 0);
  t("abc", "*[~a]*", 0);
  t("abc", "*[~a]", 0);
  t("abc", "[~b]*", 0);
  t("abc", "*[~b]*", 0);
  t("abc", "*[~b]", 0);
  t("abc", "[~c]*", 0);
  t("abc", "*[~c]*", 0);
  t("abc", "*[~c]", 0);
  t("abc", "[~x]*", 0);
  t("abc", "*[~x]*", 0);
  t("abc", "*[~x]", 0);
  /* Fourth case: preceding wildcard */
  t("abc", "?a*", 0);
  t("abc", "*?a*", 0);
  t("abc", "*?a", 0);
  t("abc", "?b*", 0);
  t("abc", "*?b*", 0);
  t("abc", "*?b", 0);
  t("abc", "?c*", 0);
  t("abc", "*?c*", 0);
  t("abc", "*?c", 0);
  t("abc", "?x*", 0);
  t("abc", "*?x*", 0);
  t("abc", "*?x", 0);
  /* Fifth case: following wildcard */
  t("abc", "a?*", 0);
  t("abc", "*a?*", 0);
  t("abc", "*a?", 0);
  t("abc", "b?*", 0);
  t("abc", "*b?*", 0);
  t("abc", "*b?", 0);
  t("abc", "c?*", 0);
  t("abc", "*c?*", 0);
  t("abc", "*c?", 0);
  t("abc", "x?*", 0);
  t("abc", "*x?*", 0);
  t("abc", "*x?", 0);
}
#endif
#ifdef SELFTEST_EXP
  0 result=1
 * 0 result=1
 ** 0 result=1
 ? 0 result=0
 [abc] 0 result=0
a a 0 result=1
a A 0 result=0
A a 0 result=0
A A 0 result=1
a a 1 result=1
a A 1 result=1
A a 1 result=1
A A 1 result=1
a [a] 0 result=1
a [A] 0 result=0
A [a] 0 result=0
A [A] 0 result=1
a [a] 1 result=1
a [A] 1 result=1
A [a] 1 result=1
A [A] 1 result=1
a [~a] 0 result=0
a [~A] 0 result=1
A [~a] 0 result=1
A [~A] 0 result=0
a [~a] 1 result=0
a [~A] 1 result=0
A [~a] 1 result=0
A [~A] 1 result=0
a *a 0 result=1
a *A 0 result=0
A *a 0 result=0
A *A 0 result=1
a *a 1 result=1
a *A 1 result=1
A *a 1 result=1
A *A 1 result=1
abc  0 result=0
abc * 0 result=1
abc ** 0 result=1
abc ? 0 result=0
abc ?? 0 result=0
abc ??? 0 result=1
abc ???? 0 result=0
abc abc 0 result=1
abc *abc 0 result=1
abc *bc 0 result=1
abc *c 0 result=1
abc ?bc 0 result=1
abc a?c 0 result=1
abc ab? 0 result=1
abc [axy][bxy][cxy] 0 result=1
abc a* 0 result=1
abc *a* 0 result=1
abc *a 0 result=0
abc b* 0 result=0
abc *b* 0 result=1
abc *b 0 result=0
abc c* 0 result=0
abc *c* 0 result=1
abc *c 0 result=1
abc x* 0 result=0
abc *x* 0 result=0
abc *x 0 result=0
abc [axy]* 0 result=1
abc *[axy]* 0 result=1
abc *[axy] 0 result=0
abc [bxy]* 0 result=0
abc *[bxy]* 0 result=1
abc *[bxy] 0 result=0
abc [cxy]* 0 result=0
abc *[cxy]* 0 result=1
abc *[cxy] 0 result=1
abc [xyz]* 0 result=0
abc *[xyz]* 0 result=0
abc *[xyz] 0 result=0
abc [~a]* 0 result=0
abc *[~a]* 0 result=1
abc *[~a] 0 result=1
abc [~b]* 0 result=1
abc *[~b]* 0 result=1
abc *[~b] 0 result=1
abc [~c]* 0 result=1
abc *[~c]* 0 result=1
abc *[~c] 0 result=0
abc [~x]* 0 result=1
abc *[~x]* 0 result=1
abc *[~x] 0 result=1
abc ?a* 0 result=0
abc *?a* 0 result=0
abc *?a 0 result=0
abc ?b* 0 result=1
abc *?b* 0 result=1
abc *?b 0 result=0
abc ?c* 0 result=0
abc *?c* 0 result=1
abc *?c 0 result=1
abc ?x* 0 result=0
abc *?x* 0 result=0
abc *?x 0 result=0
abc a?* 0 result=1
abc *a?* 0 result=1
abc *a? 0 result=0
abc b?* 0 result=0
abc *b?* 0 result=1
abc *b? 0 result=1
abc c?* 0 result=0
abc *c?* 0 result=0
abc *c? 0 result=0
abc x?* 0 result=0
abc *x?* 0 result=0
abc *x? 0 result=0
#endif
