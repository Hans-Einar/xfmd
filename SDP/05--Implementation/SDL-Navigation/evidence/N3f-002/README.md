# N3f-002 — Measured caption space in the native routing retry

Session0005 T009, 2026-10-09 / PLAN-XFMD-0009 / KB020/028 / FUNC-024.

## Delivery and remaining scope

N3f repairs the reported DocumentReferences VP07 detail by using actual measured
caption extents to bound the existing single expanded-placement retry. Node/edge
semantics, strict attachment validation, four label orders, three route transactions,
engine selection and deadlines remain unchanged. Fork commit
04c6ea7146dab35ef5fca0c204e65763e52f99d7 is adopted through a checksum-pinned
repository archive; all 1,292 regular files match the clean fork and extracted input.

Fifteen affected fork integration tests and three strict attachment unit tests pass.
The normal Release build passes 14 selected CTests, including six native GUI tests.
Actual native/PDF inspection confirms the reported diagram renders all 12 nodes and
15 edges; its vector PDF has one page and all declaration labels are present.

A sweep of all ten existing XFMD VP07 feature details renders nine successfully.
ChapterNavigation still fails with an ambiguous label on edge7; the identical
failure reproduces in the pre-fix normal probe. This is an existing additional
case, not a passed gate or a regression introduced by the new spacing rule.
Prioritize that fixture next, then large-overview reading, focused use-case/scenario
projections and N4 exact source navigation. KB020/028 and Session0005 remain active.

Owner explicitly confirms keeping NormalDesktop for future design evolution.
No authored SDL/SDUI sources changed. No installed XFMD, version or gh change.
A fresh development window launch was attempted, but its process had exited at
closeout without an error log. The older window remains and uses the earlier
build; no automatic replacement or restart is claimed.
No independent review, root-wide commit, push, merge or release claimed.

## Reproduction and causal evidence

[Original owner reproduction](../N3f-001/README.md) precedes the change.
[Normal before probe](normal-before.log) reproduces attachment intrusion edge11.
Its Pango metrics are captured in normal-before.metrics.tsv; convert by 1/0.75
at the existing C++ point-to-SVG boundary. The first direct fork probes mistakenly
used point units (fork-before.log / roundtrip-before.log); those passing results
are not evidence that the actual defect passed. [Exact before](exact-before.log)
corrects units and fails with the owner's exact error. before.log used an older
isolated probe; normal-before supersedes it for current-candidate behavior.

[Placement diagnostics](placement-diagnostic.log) identify edge14 traversing edge11's
attachment corridor. Temporary logging was removed. The prior retry's fixed
spacing did not guarantee room for opposed measured labels. The correction keeps
its existing spacing floor and one-retry bound while adding a measurement-derived
minimum along/across graph direction. No invalid geometry is accepted as a fix.
[Design](../../../../04--Design/SDL-Navigation/N3f-Routing.md) gives the boundary.

## Verification

- [Fork regression/backend tests](fork-tests.log): 15 pass.
- [Attachment unit tests](attachment-tests.log): 3 pass, including intrusion rejection.
- [Normal build](build.log), [consumer tests](tests-final.log): 14/14, six GUI.
- [Native view](document-references.png), [PDF](document-references.pdf),
  [PDF raster](diagram-pdf.png), [native log](native.log),
  [text labels check](pdf-content-check.log).
- [All VP07 detail results](sweep-results.json): 9/10. [Baseline ChapterNavigation
  failure](chapter-before.log) proves the outstanding case predates this change.
  Each generated sweep bundle retains its source/manifest; native.log and SVG
  (when valid) sit beside each diagram.
- [Source verification](source-verification.log), [fork patch](fork.patch),
  [scoped consumer patch](xfmd-scoped.patch), [candidate](candidate.json).
- [Native capture script](capture_native.py) and [sweep script](sweep.py) compile
  actual C++ probes against the normal Release libraries. Run against separate
  output copies to preserve these records. The native probe is [here](native_probe.cpp).
- [Document checks](document-checks.log) and [desktop launch](desktop-launch.json).

Source archives include the shared working tree, not solely this turn's edits.
The renderer archive is exactly the manifest-selected clean local commit; the
previous extracted dependency was preserved under .deps/mermaid-rs-renderer-before-N3f-002.
