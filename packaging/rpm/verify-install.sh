#!/bin/bash
# Runs in a fresh AlmaLinux container; no developer libraries or checkout mounted.
set -euo pipefail
package=$1 expected=$2
rpmfile=/packages/$package
dnf -y install epel-release dnf-plugins-core
dnf config-manager --set-enabled crb
dnf -y install /packages/upgrade-fixture/xfmd-0.0.0-1.el10_2.x86_64.rpm
dnf -y upgrade "$rpmfile"
dnf -y install libXfont2 libglvnd-glx libXdmcp libxshmfence xkbcomp xkeyboard-config python3.12
[[ $(/usr/bin/xfmd --version) == "xfmd $expected" ]]
rpm -V xfmd
! ldd /usr/bin/xfmd | grep 'not found'
python3.12 /probe/probe.py "$expected"
# A same-version reinstall must preserve unrelated user files and work normally.
mkdir -p /root/.config/xfmd-verification
printf 'preserve\n' > /root/.config/xfmd-verification/marker
dnf -y reinstall "$rpmfile"
[[ $(/usr/bin/xfmd --version) == "xfmd $expected" ]]
dnf -y remove xfmd
[[ ! -e /usr/bin/xfmd && ! -e /usr/lib64/xfmd/libFOX-1.6.so.0 ]]
[[ $(cat /root/.config/xfmd-verification/marker) == preserve ]]
dnf -y install "$rpmfile"
[[ $(/usr/bin/xfmd --version) == "xfmd $expected" ]]
rpm -V xfmd
