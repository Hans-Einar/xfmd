# N3b-001 — Adopt the VP07 renderer correction

Session0005 T002, 2026-10-08. PLAN-XFMD-0009 / KB020 → UR039–041,
SR021–024 → FUNC024 / AT059–065. Existing rendering requirements and interfaces
remain accurate; the current SDL-navigation design records source transport and
retry semantics. Authored .design/.sdui sources remain unchanged by this work.

## Delivery

Normal XFMD now pins fork e79d8f9caf649995a5db5e54b0ea5d6b7fab8221, using an exact
7,214,220-byte xz source archive in third_party. The manifest records commit and
SHA-256; bootstrap verifies both the archive checksum and vendor manifest before
extracting unmodified source. No .deps patches, local-checkout dependency or public
fork publication. The previous source is preserved in the ignored
.deps/mermaid-rs-renderer-before-N3b-589517a directory. Cargo.lock did not change.

The normal Release FFI and native application now contain the previously tested
NoSpace classification. Invalid geometry is never accepted; the existing one-time
fresh-placement retry retains Libavoid and the production deadline. No edge loss,
Legacy fallback or frozen-position placement fallback. The original fixture is
now tests/fixtures/mermaid/vp07-configurable-scrolling.mmd, covered by the measured
C++ DiagramLayoutTest with all 12 nodes, 18 SVG edge identities and node labels.

Bootstrap also retains its remote archive route. CMake watches the pin and bootstrap
script so a pin edit re-enters validation rather than silently building old source.

## Verification

- Normal Release build PASS: [build.log](build.log).
- Relevant CTest **13/13 PASS**, including four native GUI tests and the Rust
  workspace, PDF and dependency checks: [tests.log](tests.log). No full-suite claim.
- MermaidBootstrapTest contains six isolated cases: bundled extraction/idempotence,
  corrupt checksum, retained remote transport, failed vendor verification,
  mismatched/unmarked source preservation, and out-of-root archive rejection.
- [Source verification](source-verification.log): all 1,287 regular archive files
  match the clean fork checkout and extracted dependency; archive PAX identity
  matches the selected commit. [Bootstrap](bootstrap.log) verifies 52 vendor files.
- [Native log](native.log), [window](vp07-native.png), [PDF](vp07-native.pdf),
  [PDF raster](vp07-pdf.png) and [extracted text](vp07-native.txt): one diagram scene,
  one PDF page, inspected readable nodes and relations. The probe links the normal
  Release FFI/application libraries, not the previous isolated debug candidate.
  Existing optional label-leader omission remains; all 18 relations are retained.
- [Document checks](document-checks.log) record actual consistency checks.

The root contains prior shared changes and a staged KB020 baseline. No broad commit
or index reset was performed. [Candidate identity](candidate.json) records dirty
HEAD, build-input hashes, source archive and actual binaries. The source archive
contains native/Rust sources, tests, tools, CMake and manifests; the separate
manifest-selected third_party archive supplies exact fork source. It is evidence
of the candidate, not a standalone package of every external build dependency.
The before directory preserves turn baselines; README.md.txt is verbatim evidence
rather than a second live Markdown document. Probe sources and capture_native.py
allow repetition; use a separate output copy to preserve this evidence.

Installed XFMD 0.7 still matches U4-001. No version bump, installation, release,
publication or independent approval. This closes renderer adoption only; N3/N4,
nested containment, proper use-case notation, aggregate producer targets and exact
SDL provenance remain open. Session0005 stays active.
