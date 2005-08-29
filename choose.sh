mode="$1"
try="sys/try$2"
h="sys/has$2.h"
result="${h}1"
shift 2

case "$mode" in
  *c*) ./compile "$try".c >/dev/null 2>&1 || result="${h}0" ;;
esac

case "$mode" in
  *l*) ./load "$try" "$@" >/dev/null 2>&1 || result="${h}0" ;;
esac

case "$mode" in
  *r*) ./"$try" >/dev/null 2>&1 || result="${h}0" ;;
esac

rm -f "$try".o "$try"

exec cat "$result"
