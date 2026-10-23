# Building and distributing the Libavoid integration

The current commit/SHA-256 is in the [source manifest](../../cmake/mermaid-source.json).
The [support matrix](../../mermaid_coverage.md) describes profiles and integration status.

## Build

XFMD uses Rust 1.92.0, Cargo.lock, C++17, CMake/Ninja, FOX, Cairo/Pango and
librsvg >= 2.46. Mermaid source is an exact, checksummed archive of a commit in
Hans-Einar/mermaid-rs-renderer. An optional repository-relative `archive` field
selects bundled source; otherwise bootstrap downloads the GitHub archive. Both
transports check SHA-256 and the Libavoid source manifest before installation.
See [bundled source provenance](../../third_party/README.md).

Session0005 N3b adopts the local VP07 correction through a bundled archive. It
does not publish the fork. Existing mismatched `.deps/mermaid-rs-renderer` must
be preserved/moved before bootstrap; no source is patched or overwritten.
CMake reconfigures when the source manifest or bootstrap script changes.

No Node, browser or external routing service participates. The fork vendors
Libavoid at `vendor/libavoid`, pinned to Adaptagrams
`840ebcff20dbba36ad03a2160edf7cbaf9859984` with a file manifest.

```sh
python3 tools/bootstrap_mermaid.py
python3 .deps/mermaid-rs-renderer/tools/verify_libavoid.py
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j2
ctest --test-dir build --output-on-failure -j1
```

After source/Cargo dependencies have been acquired, the build can run offline.
Rust build.rs compiles the bundled C++ sources and the small C ABI wrapper.
Each call owns its Router; threads do not share a mutable router. C++ catches
exceptions and Rust panics stop at the existing FFI boundary.

## License and relinking

Libavoid is LGPL-2.1-or-later; the remaining fork code is MIT.
`vendor/libavoid/LICENSE.LGPL` and copyright notices accompany binary distribution.
The build links Libavoid statically into the Rust archive. Distribution must
therefore include materials for rebuilding and relinking with modified Libavoid;
a finished XFMD executable and MIT license alone are insufficient. Supply complete
corresponding XFMD/fork source and build dependencies, or the necessary objects,
archives and relinking instructions under LGPL section 6. Do not impose restrictions
on modification or reverse engineering for debugging those modifications.

To rebuild corresponding source, preserve `.xfmd-pin`, modify Libavoid in the
fork's vendor directory in a separate checkout, rebuild the fork/crate and relink
XFMD. An XFMD source distribution must include the exact fork source, either in
`.deps` with its pin stamp or as the selected bundled archive that bootstrap
verifies and extracts. A reference to an arbitrary newer upstream is insufficient.
Remove Cargo build artifacts if library objects from an older build are reused.
Normal bootstrap never changes existing sources automatically.

## Runtime limits

Libavoid admission rejects more than 256 edges or 2048 candidate ports before
native work. This is stricter than the parser model limit. Errors are displayed
with source and do not switch automatically to Legacy. Native nudging can exceed
the cooperative deadline between progress checkpoints; a hard deadline requires
a different process model and is not implemented here.
