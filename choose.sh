try="sys/try$2"
h="sys/${3:-has$2}.h"
result="${h}2"

case "$1" in
  *c*) ./compile "$try".c >/dev/null 2>&1 || result="${h}1" ;;
esac

case "$1" in
  *l*) ./load "$try" >/dev/null 2>&1 || result="${h}1" ;;
esac

case "$1" in
  *r*) ./"$try" >/dev/null 2>&1 || result="${h}1" ;;
esac

rm -f "$try".o "$try"

exec cat "$result"
