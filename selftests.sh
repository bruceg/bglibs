#!/bin/sh

DIFF='diff -a -u'
$DIFF $0 $0 >/dev/null 2>&1
if [ $? -ne 0 ]
then
  DIFF='diff'
fi

t=$0.tmp.$$
echo "Creating temporary directory $t"
mkdir $t
trap 'echo "Cleaning up $t"; rm -r $t' EXIT

do_test() {
  local c="$1"
  echo "Testing $c"
  rm -f $t/*
  sed -e '1,/^#ifdef SELFTEST_EXP$/d' -e '/^#endif/,$d' $c >$t/test.exp

  ./compile $c -DSELFTEST_MAIN -o $t/test.o
  if [ $? -ne 0 ]
  then
    echo "=====> Compile failed! <====="
    return 1
  fi

  ./load $t/test */lib.a libbg-sysdeps.a
  if [ $? -ne 0 ]
  then
    echo "=====> Load failed! <====="
    return 1
  fi

  ( cd $t && ./test >./test.out 2>&1; )
  if [ $? -ne 0 ]
  then
    echo "=====> Test failed! <====="
    return 1
  fi

  $DIFF $t/test.exp $t/test.out
  if [ $? -ne 0 ]
  then
    echo "=====> Output failed! <====="
    return 1
  fi
  return 0
}

exitcode=true
if [ $# -gt 0 ]; then
  for c in "$@"; do
    do_test $c
    if [ $? -ne 0 ]; then
      exitcode=false;
    fi
  done
else
  for c in `fgrep -l '#ifdef SELFTEST_MAIN' */*.c`
  do
    do_test $c
    if [ $? -ne 0 ]; then
      exitcode=false;
    fi
  done
fi
$exitcode
