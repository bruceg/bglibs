try="sys/$2"
h1="sys/$3.h1"
h2="sys/$3.h2"
result="$h2"

case "$1" in
  *c*) ./compile "$try".c >/dev/null 2>&1 || result="$h1" ;;
esac

case "$1" in
  *l*) ./load "$try" >/dev/null 2>&1 || result="$h1" ;;
esac

case "$1" in
  *r*) ./"$try" >/dev/null 2>&1 || result="$h1" ;;
esac

rm -f "$try".o "$try"

exec cat "$result"
