#!/bin/sh
set -e
exitcode=true
t=$0.tmp.$$
mkdir -v $t
trap 'rm -r $t' EXIT
for c in `fgrep -l '#ifdef SELFTEST_MAIN' */*.c`
do
  echo "Testing $c"
  sed -e '1,/^#ifdef SELFTEST_EXP$/d' -e '/^#endif/,$d' $c >$t/test.exp
  if ! ./compile $c -DSELFTEST_MAIN -o $t/test.o
  then
    echo "=====> Compile failed! <====="
    exitcode=false
  elif ! ./load $t/test */lib.a
  then
    echo "=====> Load failed! <====="
    exitcode=false
  elif ! $t/test >$t/test.out 2>&1
  then
    echo "=====> Test failed! <====="
    exitcode=false
  elif ! diff -a -U 9999 $t/test.exp $t/test.out
  then
    echo "=====> Output failed! <====="
    exitcode=false
  fi
done
$exitcode
