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
python3 packaging/rpm/build-producer.py --source /work/source --work /work/producer-source --output /work/producer
mkdir -p /work/build/libexec/xfmd
cp /work/producer/sdptool /work/build/libexec/xfmd/
cmake -S . -B /work/build -G Ninja -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON \
  -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_INSTALL_LIBDIR=lib64 -DCMAKE_INSTALL_RPATH=/usr/lib64/xfmd \
  -DXFMD_CARGO_TARGET=/cache/target -DSDP_READING_TOOL_EXECUTABLE=/work/producer/sdptool
cargo fmt -p xfmd-diagram-contracts -p xfmd-mermaid-interpreter -p xfmd-diagram-layout -p xfmd-mermaid-ffi -- --check
cmake --build /work/build --parallel "${BUILD_JOBS:-4}"
ctest --test-dir /work/build --output-on-failure -j 2 | tee /output/ctest.log
mkdir -p /output/native-probes
cp /work/build/SdpReadingGuiTest /work/build/SdpActivityGuiTest /output/native-probes/
python3 - <<'PROBES'
import hashlib, json
from pathlib import Path
root = Path('/output/native-probes')
Path('/output/native-probes.json').write_text(json.dumps({p.name: hashlib.sha256(p.read_bytes()).hexdigest() for p in root.iterdir()}, indent=2)+'\n')
PROBES
python3 packaging/rpm/test-local-ci.py
python3 packaging/rpm/test-producer-pin.py
runuser -u nobody -- python3 packaging/rpm/test-watch-main.py
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
cp /work/build/generated/xfmd-build.json /output/build-metadata.json
cp /work/producer/sdptool-build.json /work/stage/usr/share/doc/xfmd/
cp /work/producer/sdptool-build.json /output/
cp /work/producer/modules.json /work/stage/usr/share/doc/xfmd/sdptool-modules.json
cp -a /work/producer/licenses /work/stage/usr/share/doc/xfmd/sdptool-licenses
cp /work/producer/sdptool-*.tar.gz /work/stage/usr/share/doc/xfmd/sources/
find /work/stage -exec touch -h -d "@$SOURCE_DATE_EPOCH" {} +
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
project license. Includes its exact matching private SDPTool; SDUI executables
are optional separate installations.

%install
mkdir -p %{buildroot}
cp -a /work/stage/. %{buildroot}/

%files
/usr/bin/xfmd
/usr/libexec/xfmd/sdptool
/usr/lib64/xfmd
/usr/share/applications/xfmd.desktop
/usr/share/icons/hicolor/*/apps/xfmd.*
/usr/share/man/man1/xfmd.1*
/usr/share/xfmd
%doc /usr/share/doc/xfmd
SPEC
rpmbuild -bb --define '_topdir /work/rpmbuild' /work/rpmbuild/SPECS/xfmd.spec
cp /work/rpmbuild/RPMS/x86_64/xfmd-*.rpm /output/
