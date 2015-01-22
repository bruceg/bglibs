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

  sed -e 's/-o ${base}.o//' compile | \
  sh -s $1 -DSELFTEST_MAIN -o $t/test.o -include selftest.c || {
    echo "=====> Compile failed! <====="
    return 1
  }

  ./load $t/test .libs/libbg.a || {
    echo "=====> Load failed! <====="
    return 1
  }

  ( cd $t && ./test >./test.out 2>&1; ) || {
    echo "=====> Test output <====="
    cat -v $t/test.out
    echo "=====> Test failed! <====="
    return 1
  }

  ./selftest-cmp $1 $t/test.out || {
    echo "=====> Expected output <====="
    sed -e '1,/^#ifdef SELFTEST_EXP/d' -e '/^#endif/,$d' $1
    echo "=====> Test output <====="
    cat -v $t/test.out
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
