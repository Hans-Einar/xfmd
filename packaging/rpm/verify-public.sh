#!/bin/bash
set -euo pipefail
package_version=$1 expected=$2 commit=$3 producer_commit=$4
dnf --releasever=10.2 -y install epel-release dnf-plugins-core python3.12
dnf --releasever=10.2 config-manager --set-enabled crb
install -m 0644 /probe/repository/RPM-GPG-KEY-beinerud /etc/pki/rpm-gpg/RPM-GPG-KEY-beinerud
curl -fsSL https://beinerud.ddns.net/repo/AlmaLinux-10.2-apps/beinerud-apps.repo -o /etc/yum.repos.d/beinerud-apps.repo
predecessor_version=$(python3.12 -c 'import json; print(json.load(open("/probe/rpm/predecessor.json"))["version"])')
public_dnf=(dnf --releasever=10.2 -y --refresh --setopt=beinerud-apps.gpgcheck=1 --setopt=beinerud-apps.repo_gpgcheck=1 --setopt=beinerud-apps.gpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-beinerud)
# Exercise the actual public signed predecessor-to-candidate upgrade transaction.
"${public_dnf[@]}" install "xfmd-$predecessor_version" libXfont2 libglvnd-glx libXdmcp libxshmfence xkbcomp xkeyboard-config util-linux strace
[[ $(rpm -q --qf '%{VERSION}-%{RELEASE}' xfmd) == "$predecessor_version" ]]
rpm -V xfmd
"${public_dnf[@]}" upgrade "xfmd-$package_version"
[[ ! -e /usr/libexec/xfmd/sdptool ]]
[[ $(rpm -q --qf '%{VERSION}-%{RELEASE}' xfmd) == "$package_version" ]]
[[ $(/usr/bin/xfmd --version) == "xfmd $expected" ]]
rpm -V xfmd
unshare --net runuser -u nobody -- python3.12 /probe/rpm/probe.py "$expected" "$commit" "$producer_commit"
printf 'Verified signed public upgrade: %s -> %s; producer %s\n' "$predecessor_version" "$package_version" "$producer_commit"
# Also prove a fresh installation from the same authenticated public repository.
dnf --releasever=10.2 -y remove xfmd
[[ ! -e /usr/bin/xfmd && ! -e /usr/libexec/xfmd/sdptool ]]
"${public_dnf[@]}" install "xfmd-$package_version"
[[ $(rpm -q --qf '%{VERSION}-%{RELEASE}' xfmd) == "$package_version" ]]
rpm -V xfmd
unshare --net runuser -u nobody -- python3.12 /probe/rpm/probe.py "$expected" "$commit" "$producer_commit"
printf 'Verified fresh signed public installation: %s; producer %s\n' "$package_version" "$producer_commit"
