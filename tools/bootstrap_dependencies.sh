#!/usr/bin/env bash
# Explicit cmark bootstrap; configure separately fetches the pinned MicroTeX archive.
set -euo pipefail
xfmd_root=$(cd "$(dirname "$0")/.." && pwd)
xfmd_work=$(mktemp -d)
trap 'rm -rf "$xfmd_work"' EXIT
curl -fL --retry 2 https://codeload.github.com/github/cmark-gfm/tar.gz/refs/tags/0.29.0.gfm.13 -o "$xfmd_work/cmark.tar.gz"
printf '%s  %s\n' 5abc61798ebd9de5660bc076443c07abad2b8d15dbc11094a3a79644b8ad243a "$xfmd_work/cmark.tar.gz" | sha256sum -c -
tar -xzf "$xfmd_work/cmark.tar.gz" -C "$xfmd_work"
cmake -S "$xfmd_work/cmark-gfm-0.29.0.gfm.13" -B "$xfmd_work/build" -DCMAKE_BUILD_TYPE=Release -DCMARK_SHARED=OFF -DCMARK_STATIC=ON -DCMARK_TESTS=OFF -DCMAKE_INSTALL_PREFIX="$xfmd_root/.deps"
cmake --build "$xfmd_work/build" --parallel 4
cmake --install "$xfmd_work/build"
mkdir -p "$xfmd_root/.deps/share/licenses/cmark-gfm"
cp "$xfmd_work/cmark-gfm-0.29.0.gfm.13/COPYING" "$xfmd_root/.deps/share/licenses/cmark-gfm/COPYING"
