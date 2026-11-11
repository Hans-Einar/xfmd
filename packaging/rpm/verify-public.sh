#!/bin/bash
set -euo pipefail
package_version=$1 expected=$2 commit=$3
dnf -y install epel-release dnf-plugins-core
dnf config-manager --set-enabled crb
install -m 0644 /probe/repository/RPM-GPG-KEY-beinerud /etc/pki/rpm-gpg/RPM-GPG-KEY-beinerud
curl -fsSL https://beinerud.ddns.net/repo/AlmaLinux-10.2-apps/beinerud-apps.repo -o /etc/yum.repos.d/beinerud-apps.repo
# Verify the public metadata and RPM with the checked-in trust anchor.
dnf -y --refresh --setopt=beinerud-apps.gpgcheck=1 --setopt=beinerud-apps.repo_gpgcheck=1 \
 --setopt=beinerud-apps.gpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-beinerud install "xfmd-$package_version" libXfont2 libglvnd-glx libXdmcp libxshmfence xkbcomp xkeyboard-config python3.12 util-linux
[[ $(rpm -q --qf '%{VERSION}-%{RELEASE}' xfmd) == "$package_version" ]]
[[ $(xfmd --version) == "xfmd $expected" ]]
rpm -V xfmd
runuser -u nobody -- python3.12 /probe/rpm/probe.py "$expected" "$commit"
