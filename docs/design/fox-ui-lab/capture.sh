#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
repo_dir=$(cd -- "$lab_dir/../../.." && pwd)
out_dir=${1:-"$lab_dir/../fox-ui-examples"}
mkdir -p -- "$out_dir"
binary=$(mktemp /tmp/xfmd-ui-study.XXXXXX)
trap 'rm -f -- "$binary"' EXIT
"$lab_dir/build.sh" "$binary"
for name in light graphite compact controls; do
  profile=$name
  mode=workspace
  if [[ $name == compact ]]; then mode=reader; fi
  if [[ $name == controls ]]; then profile=light; mode=controls; fi
  python3 "$repo_dir/tools/run_with_xvfb.py" "$binary" "$lab_dir/profiles.ini" \
    "$profile" "$mode" "$out_dir/$name.png"
done
