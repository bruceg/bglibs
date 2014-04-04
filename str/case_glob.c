#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include <str/str.h>

/* GLOB patterns:
 * *      matches everything
 * ?      matches any single character
 * [seq]  matches any sing echaracter in seq
 * [!seq] matches any single character not in seq
 */

static unsigned make_set(const char* pptr, unsigned plen, char set[256])
{
  unsigned orig_plen = plen;
  char value;
  if (plen == 0 || *pptr != '[') return 0;
  ++pptr; --plen;
  if (*pptr == '!' || *pptr == '^') {
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
    if (isupper(p))
      set[tolower(p)] = value;
    else if (islower(p))
      set[toupper(p)] = value;
  }
  return 0;
}

static int glob_match(const str* s, unsigned offset,
		      const char* pptr, unsigned plen);

static int glob_search(const str* s, unsigned offset,
		       const char* pptr, unsigned plen)
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
    if ((len = make_set(pptr, plen, set)) == 0) return -1;
    ++len;
    pptr += len; plen -= len;
    for (; offset < s->len; ++offset) {
      if (set[(unsigned char)s->s[offset]] &&
	  (r = glob_match(s, offset+1, pptr, plen)) != 0)
	return r;
    }
  }
  /* Otherwise search for a normal character. */
  else {
    ++pptr, --plen;
    if (isupper(start)) start = tolower(start);
    do {
      char c = s->s[offset++];
      if (isupper(c)) c = tolower(c);
      if (c == start &&
	  (r = glob_match(s, offset, pptr, plen)) != 0)
	return r;
    } while (offset < s->len);
#if 0
    do {
      if ((r = glob_match(s, offset, pptr, plen)) != 0)
	return r;
      ++offset;
    } while (offset < s->len);
#endif
  }
#if 0
  else {
    if (isupper(start)) start = tolower(start);
    for (; offset < s->len; ++offset) {
      char c = s->s[offset];
      if (isupper(c)) c = tolower(c);
      if (c == start &&
	  (r = glob_match(s, offset+1, pptr+1, plen-1)) != 0)
	return r;
    }
  }
#endif
  return 0;
}

static int glob_match(const str* s, unsigned offset,
		      const char* pptr, unsigned plen)
{
  for (; plen > 0; ++pptr, --plen, ++offset) {
    char p = *pptr;
    if (p == '*')
      return glob_search(s, offset, pptr+1, plen-1);
    if (offset >= s->len) break;
    switch (p) {
    case '?':
      continue;
    case '[':
      {
	char set[256];
	unsigned len;
	if ((len = make_set(pptr, plen, set)) == 0) return -1;
	pptr += len; plen -= len;
	if (!set[(unsigned char)s->s[offset]]) return 0;
	continue;
      }
    default:
      {
	char c = s->s[offset];
	if (isupper(c)) c = tolower(c);
	if (isupper(p)) p = tolower(p);
	if (p != c) return 0;
      }
    }
  }
  return offset == s->len && plen == 0;
}

int str_case_globb(const str* s, const char* pptr, unsigned plen)
{
  return glob_match(s, 0, pptr, plen);
}

int str_case_globs(const str* s, const char* pattern)
{
  return glob_match(s, 0, pattern, strlen(pattern));
}

int str_case_glob(const str* s, const str* pattern)
{
  return glob_match(s, 0, pattern->s, pattern->len);
}

#ifdef SELFTEST_MAIN
void t(const char* string, const char* pattern)
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
  debugfn(str_case_glob(&s, &p));
}
MAIN
{
  /* Empty string */
  t("", "");
  t("", "*");
  t("", "**");
  t("", "?");
  t("", "[abc]");
  /* Case sensitivity: single char */
  t("a", "a");
  t("a", "A");
  t("A", "a");
  t("A", "A");
  /* Case sensitivity: normal set */
  t("a", "[a]");
  t("a", "[A]");
  t("A", "[a]");
  t("A", "[A]");
  /* Case sensitivity: negated set */
  t("a", "[!a]");
  t("a", "[!A]");
  t("A", "[!a]");
  t("A", "[!A]");
  t("a", "[^a]");
  t("a", "[^A]");
  t("A", "[^a]");
  t("A", "[^A]");
  /* Case sensitivity: search */
  t("a", "*a");
  t("a", "*A");
  t("A", "*a");
  t("A", "*A");
  /* Miscelaneous matches */
  t("abc", "");
  t("abc", "*");
  t("abc", "**");
  t("abc", "?");
  t("abc", "??");
  t("abc", "???");
  t("abc", "????");
  t("abc", "abc");
  t("abc", "*abc");
  t("abc", "*bc");
  t("abc", "*c");
  t("abc", "?bc");
  t("abc", "a?c");
  t("abc", "ab?");
  t("abc", "[axy][bxy][cxy]");
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
  t("abc", "a*");
  t("abc", "*a*");
  t("abc", "*a");
  t("abc", "b*");
  t("abc", "*b*");
  t("abc", "*b");
  t("abc", "c*");
  t("abc", "*c*");
  t("abc", "*c");
  t("abc", "x*");
  t("abc", "*x*");
  t("abc", "*x");
  /* Second case: character in a set */
  t("abc", "[axy]*");
  t("abc", "*[axy]*");
  t("abc", "*[axy]");
  t("abc", "[bxy]*");
  t("abc", "*[bxy]*");
  t("abc", "*[bxy]");
  t("abc", "[cxy]*");
  t("abc", "*[cxy]*");
  t("abc", "*[cxy]");
  t("abc", "[xyz]*");
  t("abc", "*[xyz]*");
  t("abc", "*[xyz]");
  /* Third case: negated set */
  t("abc", "[!a]*");
  t("abc", "*[!a]*");
  t("abc", "*[!a]");
  t("abc", "[!b]*");
  t("abc", "*[!b]*");
  t("abc", "*[!b]");
  t("abc", "[!c]*");
  t("abc", "*[!c]*");
  t("abc", "*[!c]");
  t("abc", "[!x]*");
  t("abc", "*[!x]*");
  t("abc", "*[!x]");
  t("abc", "[^a]*");
  t("abc", "*[^a]*");
  t("abc", "*[^a]");
  t("abc", "[^b]*");
  t("abc", "*[^b]*");
  t("abc", "*[^b]");
  t("abc", "[^c]*");
  t("abc", "*[^c]*");
  t("abc", "*[^c]");
  t("abc", "[^x]*");
  t("abc", "*[^x]*");
  t("abc", "*[^x]");
  /* Fourth case: preceding wildcard */
  t("abc", "?a*");
  t("abc", "*?a*");
  t("abc", "*?a");
  t("abc", "?b*");
  t("abc", "*?b*");
  t("abc", "*?b");
  t("abc", "?c*");
  t("abc", "*?c*");
  t("abc", "*?c");
  t("abc", "?x*");
  t("abc", "*?x*");
  t("abc", "*?x");
  /* Fifth case: following wildcard */
  t("abc", "a?*");
  t("abc", "*a?*");
  t("abc", "*a?");
  t("abc", "b?*");
  t("abc", "*b?*");
  t("abc", "*b?");
  t("abc", "c?*");
  t("abc", "*c?*");
  t("abc", "*c?");
  t("abc", "x?*");
  t("abc", "*x?*");
  t("abc", "*x?");
}
#endif
#ifdef SELFTEST_EXP
  result=1
 * result=1
 ** result=1
 ? result=0
 [abc] result=0
a a result=1
a A result=1
A a result=1
A A result=1
a [a] result=1
a [A] result=1
A [a] result=1
A [A] result=1
a [!a] result=0
a [!A] result=0
A [!a] result=0
A [!A] result=0
a [^a] result=0
a [^A] result=0
A [^a] result=0
A [^A] result=0
a *a result=1
a *A result=1
A *a result=1
A *A result=1
abc  result=0
abc * result=1
abc ** result=1
abc ? result=0
abc ?? result=0
abc ??? result=1
abc ???? result=0
abc abc result=1
abc *abc result=1
abc *bc result=1
abc *c result=1
abc ?bc result=1
abc a?c result=1
abc ab? result=1
abc [axy][bxy][cxy] result=1
abc a* result=1
abc *a* result=1
abc *a result=0
abc b* result=0
abc *b* result=1
abc *b result=0
abc c* result=0
abc *c* result=1
abc *c result=1
abc x* result=0
abc *x* result=0
abc *x result=0
abc [axy]* result=1
abc *[axy]* result=1
abc *[axy] result=0
abc [bxy]* result=0
abc *[bxy]* result=1
abc *[bxy] result=0
abc [cxy]* result=0
abc *[cxy]* result=1
abc *[cxy] result=1
abc [xyz]* result=0
abc *[xyz]* result=0
abc *[xyz] result=0
abc [!a]* result=0
abc *[!a]* result=1
abc *[!a] result=1
abc [!b]* result=1
abc *[!b]* result=1
abc *[!b] result=1
abc [!c]* result=1
abc *[!c]* result=1
abc *[!c] result=0
abc [!x]* result=1
abc *[!x]* result=1
abc *[!x] result=1
abc [^a]* result=0
abc *[^a]* result=1
abc *[^a] result=1
abc [^b]* result=1
abc *[^b]* result=1
abc *[^b] result=1
abc [^c]* result=1
abc *[^c]* result=1
abc *[^c] result=0
abc [^x]* result=1
abc *[^x]* result=1
abc *[^x] result=1
abc ?a* result=0
abc *?a* result=0
abc *?a result=0
abc ?b* result=1
abc *?b* result=1
abc *?b result=0
abc ?c* result=0
abc *?c* result=1
abc *?c result=1
abc ?x* result=0
abc *?x* result=0
abc *?x result=0
abc a?* result=1
abc *a?* result=1
abc *a? result=0
abc b?* result=0
abc *b?* result=1
abc *b? result=1
abc c?* result=0
abc *c?* result=0
abc *c? result=0
abc x?* result=0
abc *x?* result=0
abc *x? result=0
#endif
