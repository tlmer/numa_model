#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
OUT=${1:-/dev/shm/numa_model_verify}
rm -rf "$OUT"; mkdir -p "$OUT"
CXX=${CXX:-g++}
"$CXX" -std=c++17 -O2 -I"$ROOT/include" "$ROOT/example/numa_example.cpp" \
    "$ROOT/lib/libnumacml.a" -lpthread -ldl -o "$OUT/numa_example"
"$CXX" -std=c++17 -O2 -I"$ROOT/include" "$ROOT/console/numa_console.cpp" \
    "$ROOT/lib/libnumacml.a" -lpthread -ldl -o "$OUT/numa_console"
"$OUT/numa_example" > "$OUT/example.log"
grep -q '^NUMA_EXAMPLE: PASS' "$OUT/example.log"
printf 'help\nquit\n' | "$OUT/numa_console" > "$OUT/console.log"
grep -q 'type help' "$OUT/console.log"
echo "VERIFY: PASS"
