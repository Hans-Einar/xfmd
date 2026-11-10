#!/bin/bash
# Run on Beinerud as root after copying this directory to a reviewed staging area.
set -euo pipefail
[[ $EUID == 0 ]]
cd -- "$(dirname -- "$0")"
[[ -x /usr/local/sbin/beinerud-repo-refresh && -s /var/lib/beinerud-repo/signing-fingerprint ]]
getent passwd xfmd-publish >/dev/null || useradd --system --home-dir /var/lib/xfmd-publish --shell /sbin/nologin xfmd-publish
install -d -m 0750 -o xfmd-publish -g "$(id -gn xfmd-publish)" /var/lib/xfmd-publish /var/lib/xfmd-publish/incoming
install -m 0755 -o root -g root xfmd-receive /usr/local/libexec/xfmd-receive
install -m 0755 -o root -g root xfmd-promote /usr/local/sbin/xfmd-promote
sudoers=$(mktemp)
trap 'rm -f "$sudoers"' EXIT
printf 'xfmd-publish ALL=(root) NOPASSWD: /usr/local/sbin/xfmd-promote\n' > "$sudoers"
visudo -cf "$sudoers"
install -m 0440 -o root -g root "$sudoers" /etc/sudoers.d/xfmd-publish
