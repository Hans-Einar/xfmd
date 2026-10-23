# N3c-001 — Native use-case notation

Session0005 T003, 2026-10-08. PLAN-XFMD-0009 / KB020 and KB028 → FUNC023/024.
The selected [profile](../../../../04--Design/SDL-Navigation/UseCase-Profile.md) and
[authoring guide](../../../../../docs/design/mermaid-usecase-authoring.md) define
scope. Authored .design/.sdui files were not changed.

## Delivery and candidate

XFMD parses a bounded usecase-beta subset into family23 / records210–212.
Contracts validate identities, plain labels, memberships and relation endpoint
kinds independently. The renderer maps records to the fork's native UseCase graph;
no source text crosses the layout boundary. The fork renders measured stick actors,
ellipses/rectangles, system boundaries, associations, include/extend stereotypes
and hollow generalization markers. Isolated declarations remain visible.

Final local fork commit: 1db800b8c2fca5d65c16acc87f55c3d65265095e. The manifest selects
an exact bundled source archive with SHA-256
1d627d579fd6d9acb5226544d1843fb9a0b1dd80a33f7ce1f755ae2e75163d07.
[Source verification](source-verification.log) checks all 1,289 regular archive
files against both the clean fork and extracted dependency, including commit PAX
identity. No .deps source patch or public fork publication. The previous source
is retained in ignored .deps/mermaid-rs-renderer-before-attachment-6943424.
Earlier e79d8f9 and 6943424 archives preserve prior candidate reproducibility.

[Candidate identity](candidate.json) records shared dirty HEAD, source hashes,
source archive and exact normal Release binaries. candidate-sources.tar.gz contains
XFMD source/tests/tools/build manifests; the separate manifest-selected archive
supplies fork source. This is candidate evidence, not a release package containing
all external build dependencies. Cargo.lock and installed XFMD0.7 are unchanged.

## Verification

- Normal Release build: [build.log](build.log).
- 13/13 affected CTests PASS: [tests.log](tests.log), including four native GUI
  workflows, the Rust workspace, PDF, dependency and bootstrap checks. No full-suite
  claim. Four added parser/contract tests cover roundtrip, ordering, Unicode,
  invalid relations and rejected syntax; the interpreter has 26 tests.
- 14/14 fork tests PASS: [fork-final.log](fork-final.log), including 12 backend
  regressions, original VP07 and use-case notation/attachments. The latter checks
  four directions with/without boundaries, isolation, glyph geometry and markers.
- C++ DiagramLayoutTest uses actual Pango metrics, verifies family23, all seven
  relationship identities, labels, multiple boundaries and Unicode; [SVG](usecase.svg).
- Native preview and vector PDF: [capture script](capture_native.py),
  [probe source](native_probe.cpp), [native log](native.log),
  [window](usecase-native.png), [PDF](usecase-native.pdf),
  [PDF raster](usecase-pdf.png), [text](usecase-native.txt).
  The probe links normal Release FFI and application libraries. The unbounded
  Unicode/reverse-association/rectangle/isolated fixture has corresponding
  usecase-plain files and [log](plain-native.log).
- [Document checks](document-checks.log) record actual consistency checks.

Commands: `cmake --build build-ui -j2`; sequential CTest selection in tests.log
covers MermaidGuiTest, DiagramLayoutTest, MermaidBootstrapTest, MermaidRustTests,
DiagramPreparationTest, DiagramReadingTest, MermaidPdfTest, DiagramWorkerTest,
MermaidDependencies, StandaloneMermaidTest, StandaloneMermaidGuiTest,
SduiWorkflowGuiTest and SdpNavigationGuiTest. Fork tests use
`cargo test --no-default-features --features libavoid --test xfmd_usecase --test libavoid_suite --test xfmd_vp07`.
Run capture_native.py only in a separate output copy when preserving this evidence.

## Rejected candidate and correction

Initial 6943424 native inspection found detached actor relationships: generic
node-box ports ended in invisible space around the stick glyph. Preserved
before-attachment-usecase files show the defect. Final ports attach to actual
head/arm/leg strokes; captions are separate obstacles even for incident edges.
Boundary diagrams now use Libavoid too. Applying existing graph containment cleanup
to UseCase also corrects an outsider/isolated-member overlap exposed by the new
route validation. No backend switch, endpoint post-repair, dropped edges or deadline
increase. Earlier output is evidence of rejection, not an accepted final render.

superseded-build.log belongs to the interrupted 0c22ab9 build before OpenV marker
correction. before-attachment build/test logs belong to 6943424, not the final
candidate. fork-concurrent-run.log includes one original-VP07 timeout during a
concurrent build; fork-vp07-recheck.log passes unchanged in a quiet rerun. The final
fork suite was run without concurrent Release compilation and passes all 14 tests.
No earlier failure was removed or relabelled as a pass.

## Visual findings and layout limits

Final native/PDF inspection confirms visible actor attachments, labels, isolated
use cases, boundaries and relationship notation. The LR mixed-relation example
remains very wide, making text small at fit-to-page scale. This is a known layout
quality limit, not missing content. The explicit [TB source](usecase-topdown.mmd)
produces a more readable [PDF](usecase-topdown.pdf) and [raster](usecase-topdown-pdf.png);
its native window requires vertical scrolling. Direction is never silently changed.
The [unbounded native view](usecase-plain.png) and [PDF raster](usecase-plain-pdf.png)
show readable Unicode, isolated behavior, rectangle/ellipse and reverse association.
All expected labels are retained in PDF text. No optimal-layout or full-viewpoint
readability claim is made; aggregate producer acceptance must assess its real data.

## Remaining scope

This is a bounded syntax profile and native renderer prerequisite. It does not
implement full Mermaid compatibility or SDL VP01/VP07 aggregate views. See the
[producer handoff](../../../../04--Design/SDL-Navigation/Producer-Handoff.md).
Source-supported scenario/condition/provenance views and nested flowchart geometry
remain open. No upstream SDL/SDPTool files changed, no root-wide commit or index
reset, no release/version bump/install/publication, and no independent approval.
Session0005 and KB020/028 remain active.
