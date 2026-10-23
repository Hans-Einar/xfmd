# N3g-001 — ChapterNavigation caption identity

Session0005 T010, 2026-10-09 / PLAN-XFMD-0009 / KB020/028 / FUNC-024.

ChapterNavigation now renders with all 10 nodes and 12 edges. The fork registers
caption-identity exclusion polygons alongside existing attachment obstacles;
strict final validation, routing bounds and deadlines remain unchanged. All ten
actual XFMD VP07 feature details render in the normal Release candidate. Sixteen
fork integration tests, five geometry tests and 14 selected CTests (six GUI) pass.
Native preview and one-page vector PDF were inspected; all declaration labels
are present. The installed 0.7 and authored models remain unchanged.

Renderer commit: 55eff5da93e99420e110a1dce2568423578dbd58. The checksum-pinned archive and all 1,295 regular
files match the clean fork and extracted dependency. The former dependency is
preserved in .deps/mermaid-rs-renderer-before-N3g-001. No source in .deps was patched.

## Cause and selected correction

[Before](before.log) reproduces the exact ambiguity using XFMD's contract round
trip and native Pango metrics converted by 1/0.75 to SVG units. [Diagnostics](diagnostic.log)
show the padded caption 38 units from its owner and about 15 from edge10. Attachment
exclusion did not protect the caption's opposite side from nearer foreign routes.
The fork now adds a Manhattan-distance exclusion polygon using the existing
ambiguity tolerance and router clearance. Chamfered corners preserve owner
clearance; final validation remains necessary when routes move.
[Design](../../../../04--Design/SDL-Navigation/N3g-Routing.md) and
[fork patch](fork.patch) define the adopted correction.

A finer gap-sampling experiment exceeded the unchanged two-second budget
(gap-experiment.log); this does not establish geometry success/failure. A rectangular
halo experiment passed (halo-experiment.log) but was refined to avoid unnecessary
corner blocking. Temporary diagnostics were removed before the clean fork commit.

## Verification and reproduction

- [Fork integrations](fork-tests.log): 16 passed; [geometry checks](attachment-tests.log): 5 passed.
- [Normal build](build.log), [test-path rebuild](build-fixture-path.log).
- [Initial consumer checks](tests-first.log): 13/14 pass, including all six GUI tests.
  The new DiagramLayoutTest used the existing macro for docs/design/mermaid rather
  than tests/fixtures/mermaid and therefore failed to load its input. A dedicated
  fixture definition corrects only this test path. [Retest](tests.log): 1/1 pass.
  Combined disposition: all 14 selected CTests pass; no product change followed
  the native/PDF sweep. Only the failed test was repeated.
- [All ten VP07 details](sweep-results.json): 10/10; source/manifest, SVG, metrics and
  per-diagram logs are preserved below vp07-sweep. Inputs are the unchanged actual
  producer outputs from N3f-002, not hand-edited alternative graphs.
- [Native preview](chapter-navigation.png), [vector PDF](chapter-navigation.pdf),
  [PDF raster](chapter-navigation-pdf.png), [native log](native.log),
  [label check](pdf-content-check.log). Visually inspected all ten node labels,
  caption associations and routes; no caption collision or ambiguity seen.
- [Archive identity](source-verification.log), [candidate](candidate.json),
  [document checks](document-checks.log). Candidate identifies shared dirty source,
  build artifacts, unchanged installed 0.7 and model, not a clean XFMD commit.
- [Sweep script](sweep.py), [native capture](capture_native.py),
  [native probe](native_probe.cpp). Run against copied outputs to preserve evidence.
  Fork commands are cargo test --no-default-features --features libavoid with
  tests xfmd_chapter_navigation, xfmd_document_references, xfmd_vp07, libavoid_suite,
  xfmd_usecase; unit filter --lib layout::routed. Normal build: cmake --build build-ui -j2.

## Remaining scope

Large-overview reading, focused use-case/scenario projections and N4 exact sources
remain. KB020/028 and Session0005 stay active. No release/install, desktop restart,
root-wide commit, push/merge, independent review or owner visual acceptance claimed.
