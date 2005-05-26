#!/bin/sh
set -e || exit "$?"

DIFF='diff -a -u'
$DIFF $0 $0 >/dev/null 2>&1 || DIFF='diff'

t=$0.tmp.$$
echo "Creating temporary directory $t"
mkdir $t
trap 'echo "Cleaning up $t"; rm -r $t' EXIT

do_test() {
  echo "Testing $1"
  rm -f $t/*
  sed -e '1,/^#ifdef SELFTEST_EXP$/d' -e '/^#endif/,$d' $1 >$t/test.exp

  sed -e 's/-o ${base}.o//' compile | \
  sh -s $1 -DSELFTEST_MAIN -o $t/test.o || {
    echo "=====> Compile failed! <====="
    return 1
  }

  ./load $t/test libbg.a libbg-sysdeps.a || {
    echo "=====> Load failed! <====="
    return 1
  }

  ( cd $t && ./test >./test.out 2>&1; ) || {
    echo "=====> Test failed! <====="
    return 1
  }

  cat -v $t/test.out | $DIFF $t/test.exp - || {
    echo "=====> Output failed! <====="
    return 1
  }
  return 0
}

exitcode=true
if [ $# -gt 0 ]; then
  for c in "$@"; do
    do_test $c || exitcode=false
  done
else
  for c in `fgrep -l '#ifdef SELFTEST_MAIN' */*.c`
  do
    do_test $c || exitcode=false
  done
fi
$exitcode
