#!/bin/bash
# Runs in a fresh AlmaLinux container; no developer libraries or checkout mounted.
set -euo pipefail
package=$1 expected=$2 commit=$3 producer_commit=$4
rpmfile=/packages/$package
! rpm -qpl "$rpmfile" | grep '/libexec/xfmd/'
dnf --releasever=10.2 -y install epel-release dnf-plugins-core
dnf --releasever=10.2 config-manager --set-enabled crb
rpm --import /packages/RPM-GPG-KEY-beinerud
rpmkeys --checksig /packages/previous-xfmd.rpm
dnf --releasever=10.2 --setopt=localpkg_gpgcheck=1 -y install /packages/previous-xfmd.rpm
[[ $(rpm -q --qf '%{VERSION}-%{RELEASE}' xfmd) == '0.10.0-76.el10_2' ]]
dnf --releasever=10.2 -y upgrade "$rpmfile"
dnf --releasever=10.2 -y install libXfont2 libglvnd-glx libXdmcp libxshmfence xkbcomp xkeyboard-config python3.12 util-linux strace
[[ $(/usr/bin/xfmd --version) == "xfmd $expected" ]]
rpm -V xfmd
[[ ! -e /usr/libexec/xfmd/sdptool ]]
! ldd /usr/bin/xfmd | grep 'not found'
unshare --net runuser -u nobody -- python3.12 /probe/probe.py "$expected" "$commit" "$producer_commit"
# A same-version reinstall must preserve unrelated user files and work normally.
mkdir -p /root/.config/xfmd-verification
printf 'preserve\n' > /root/.config/xfmd-verification/marker
dnf --releasever=10.2 -y reinstall "$rpmfile"
[[ $(/usr/bin/xfmd --version) == "xfmd $expected" ]]
dnf --releasever=10.2 -y remove xfmd
[[ ! -e /usr/bin/xfmd && ! -e /usr/lib64/xfmd/libFOX-1.6.so.0 && ! -e /usr/libexec/xfmd/sdptool ]]
[[ $(cat /root/.config/xfmd-verification/marker) == preserve ]]
[[ -x /shared-engine/gh-sdp ]]
dnf --releasever=10.2 -y install "$rpmfile"
[[ $(/usr/bin/xfmd --version) == "xfmd $expected" ]]
rpm -V xfmd
