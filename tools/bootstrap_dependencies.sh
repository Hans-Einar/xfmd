#!/usr/bin/env bash
# Explicit network/bootstrap step; configure/build never downloads dependencies.
set -euo pipefail
xfmd_root=$(cd "$(dirname "$0")/.." && pwd)
xfmd_work=$(mktemp -d)
trap 'rm -rf "$xfmd_work"' EXIT
curl -fL --retry 2 https://codeload.github.com/commonmark/cmark/tar.gz/refs/tags/0.31.1 -o "$xfmd_work/cmark.tar.gz"
printf '%s  %s\n' 3da93db5469c30588cfeb283d9d62edfc6ded9eb0edc10a4f5bbfb7d722ea802 "$xfmd_work/cmark.tar.gz" | sha256sum -c -
tar -xzf "$xfmd_work/cmark.tar.gz" -C "$xfmd_work"
cmake -S "$xfmd_work/cmark-0.31.1" -B "$xfmd_work/build" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DBUILD_TESTING=OFF -DCMAKE_INSTALL_PREFIX="$xfmd_root/.deps"
cmake --build "$xfmd_work/build" --parallel 4
cmake --install "$xfmd_work/build"
mkdir -p "$xfmd_root/.deps/share/licenses/cmark"
cp "$xfmd_work/cmark-0.31.1/COPYING" "$xfmd_root/.deps/share/licenses/cmark/COPYING"
