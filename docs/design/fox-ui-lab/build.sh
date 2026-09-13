#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
out=${1:-/tmp/xfmd-fox-ui-lab}
read -r -a fox_flags <<< "$(pkg-config --cflags --libs fox cairo x11)"
c++ -std=c++17 -O2 -Wall -Wextra "$lab_dir/main.cpp" "$lab_dir/UiButton.cpp" \
    "$lab_dir/DocumentFixture.cpp" "${fox_flags[@]}" -o "$out"
printf '%s\n' "$out"
