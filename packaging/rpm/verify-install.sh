#!/bin/bash
# Runs in a fresh AlmaLinux container; no developer libraries or checkout mounted.
set -euo pipefail
package=$1 expected=$2 commit=$3
rpmfile=/packages/$package
dnf --releasever=10.2 -y install epel-release dnf-plugins-core
dnf --releasever=10.2 config-manager --set-enabled crb
dnf --releasever=10.2 -y install /packages/upgrade-fixture/xfmd-0.0.0-1.el10_2.x86_64.rpm
dnf --releasever=10.2 -y upgrade "$rpmfile"
dnf --releasever=10.2 -y install libXfont2 libglvnd-glx libXdmcp libxshmfence xkbcomp xkeyboard-config python3.12 util-linux
[[ $(/usr/bin/xfmd --version) == "xfmd $expected" ]]
rpm -V xfmd
! ldd /usr/bin/xfmd | grep 'not found'
runuser -u nobody -- python3.12 /probe/probe.py "$expected" "$commit"
# A same-version reinstall must preserve unrelated user files and work normally.
mkdir -p /root/.config/xfmd-verification
printf 'preserve\n' > /root/.config/xfmd-verification/marker
dnf --releasever=10.2 -y reinstall "$rpmfile"
[[ $(/usr/bin/xfmd --version) == "xfmd $expected" ]]
dnf --releasever=10.2 -y remove xfmd
[[ ! -e /usr/bin/xfmd && ! -e /usr/lib64/xfmd/libFOX-1.6.so.0 ]]
[[ $(cat /root/.config/xfmd-verification/marker) == preserve ]]
dnf --releasever=10.2 -y install "$rpmfile"
[[ $(/usr/bin/xfmd --version) == "xfmd $expected" ]]
rpm -V xfmd
