#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
OUT=${1:-$ROOT/out}
mkdir -p "$OUT"
CXX=${CXX:-g++}
"$CXX" -std=c++17 -O2 -I"$ROOT/include" "$ROOT/example/numa_example.cpp" \
    "$ROOT/lib/libnumacml.a" -lpthread -ldl -o "$OUT/numa_example"
"$CXX" -std=c++17 -O2 -I"$ROOT/include" "$ROOT/console/numa_console.cpp" \
    "$ROOT/lib/libnumacml.a" -lpthread -ldl -o "$OUT/numa_console"
echo "built $OUT/numa_example and $OUT/numa_console"
