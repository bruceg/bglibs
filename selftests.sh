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

exitcode=true
for c in `fgrep -l '#ifdef SELFTEST_MAIN' */*.c`
do
  echo "Testing $c"
  sed -e '1,/^#ifdef SELFTEST_EXP$/d' -e '/^#endif/,$d' $c >$t/test.exp

  ./compile $c -DSELFTEST_MAIN -o $t/test.o
  if [ $? -ne 0 ]
  then
    echo "=====> Compile failed! <====="
    exitcode=false
    continue
  fi

  ./load $t/test */lib.a libbg-sysdeps.a
  if [ $? -ne 0 ]
  then
    echo "=====> Load failed! <====="
    exitcode=false
    continue
  fi

  ( cd $t && ./test >./test.out 2>&1; )
  if [ $? -ne 0 ]
  then
    echo "=====> Test failed! <====="
    exitcode=false
    continue
  fi

  $DIFF $t/test.exp $t/test.out
  if [ $? -ne 0 ]
  then
    echo "=====> Output failed! <====="
    exitcode=false
    continue
  fi

  rm -f $t/*
done
$exitcode
