#!/bin/bash
set -euo pipefail
version=$1 release=$2
mkdir -p /work /cache/cargo
cp -a /source /work/source
cd /work/source
export SOURCE_DATE_EPOCH=$(git show -s --format=%ct HEAD)
export CARGO_HOME=/cache/cargo
export PKG_CONFIG_PATH=/opt/xfmd-fox/lib/pkgconfig
./tools/bootstrap_dependencies.sh
cmake -S . -B /work/build -G Ninja -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON \
  -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_INSTALL_LIBDIR=lib64 -DCMAKE_INSTALL_RPATH=/usr/lib64/xfmd \
  -DXFMD_CARGO_TARGET=/cache/target
cargo fmt -p xfmd-diagram-contracts -p xfmd-mermaid-interpreter -p xfmd-diagram-layout -p xfmd-mermaid-ffi -- --check
cmake --build /work/build --parallel "${BUILD_JOBS:-4}"
ctest --test-dir /work/build --output-on-failure -j 2 | tee /output/ctest.log
python3 packaging/rpm/test-local-ci.py
python3 packaging/rpm/test-watch-main.py
python3 packaging/repository/test-publisher.py
python3 tools/check_blueprint_symbols.py
python3 SDP/KanBan/verify_board.py
DESTDIR=/work/stage cmake --install /work/build
mkdir -p /work/stage/usr/lib64/xfmd /work/stage/usr/share/doc/xfmd/sources
cp -a /opt/xfmd-fox/lib/libFOX-1.6.so.0* /work/stage/usr/lib64/xfmd/
cp /opt/xfmd-fox/LICENSE /work/stage/usr/share/doc/xfmd/FOX-LICENSE
cp /opt/fox-1.6.57.tar.gz /work/stage/usr/share/doc/xfmd/sources/
# Preserve corresponding sources for the bundled native libraries.
git archive HEAD | gzip -n > /work/stage/usr/share/doc/xfmd/sources/xfmd-source.tar.gz
tar --sort=name --mtime="@$SOURCE_DATE_EPOCH" --owner=0 --group=0 --numeric-owner -czf /work/stage/usr/share/doc/xfmd/sources/mermaid-source.tar.gz -C .deps mermaid-rs-renderer
tar --sort=name --mtime="@$SOURCE_DATE_EPOCH" --owner=0 --group=0 --numeric-owner -czf /work/stage/usr/share/doc/xfmd/sources/microtex-source.tar.gz -C /work/build/_deps microtex-src
find /work/stage -exec touch -h -d "@$SOURCE_DATE_EPOCH" {} +
cp /work/build/generated/xfmd-build.json /output/build-metadata.json
rpm -qa | sort > /output/build-packages.txt
mkdir -p /work/rpmbuild/{BUILD,RPMS,SOURCES,SPECS,SRPMS}
cat > /work/rpmbuild/SPECS/xfmd.spec <<SPEC
%global debug_package %{nil}
%global _build_id_links none
%global __provides_exclude ^libFOX-1\\.6.*$
%global __requires_exclude ^libFOX-1\\.6.*$
Name: xfmd
Version: $version
Release: $release
Summary: Native FOX Markdown viewer and editor
License: LicenseRef-Unspecified
URL: https://github.com/Hans-Einar/xfmd
BuildArch: x86_64
Requires: gdk-pixbuf2-modules
Requires: dejavu-sans-fonts
Requires: dejavu-sans-mono-fonts

%description
Native Markdown viewer and editor. Includes private FOX 1.6.57, notices and
corresponding bundled-library sources. The project owner has not selected a
project license. Optional SDPTool and SDUI executables are installed separately.

%install
mkdir -p %{buildroot}
cp -a /work/stage/. %{buildroot}/

%files
/usr/bin/xfmd
/usr/lib64/xfmd
/usr/share/applications/xfmd.desktop
/usr/share/icons/hicolor/*/apps/xfmd.*
/usr/share/man/man1/xfmd.1*
/usr/share/xfmd
%doc /usr/share/doc/xfmd
SPEC
rpmbuild -bb --define '_topdir /work/rpmbuild' /work/rpmbuild/SPECS/xfmd.spec
cp /work/rpmbuild/RPMS/x86_64/xfmd-*.rpm /output/

# Synthetic older package tests RPM upgrade mechanics; it is never published.
mkdir -p /output/upgrade-fixture
sed 's/^Version: .*/Version: 0.0.0/; s/^Release: .*/Release: 1.el10_2/' /work/rpmbuild/SPECS/xfmd.spec > /work/rpmbuild/SPECS/upgrade.spec
rpmbuild -bb --define '_topdir /work/rpmbuild' /work/rpmbuild/SPECS/upgrade.spec
cp /work/rpmbuild/RPMS/x86_64/xfmd-0.0.0-1.el10_2.x86_64.rpm /output/upgrade-fixture/
