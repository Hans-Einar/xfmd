# N3d-001 — VP01 overview from producer to native XFMD

Session0005 T004, PLAN-XFMD-0009, KB028/020 and producer KB-SDP-032.
This is the first aggregate projection increment; VP07, focused use-case/scenario
views and exact SDL source navigation are still open.

## Delivered behavior

SDL lazily selects `projection=overview` for VP01. It retains every declared actor
and use case, including isolated declarations, plus only modeled pursues
associations. Declaration metadata maps generated IDs to source IDs/spans; the
bundle retains the source registry and revision. Functionality realization is a
separate supports/contributes-to table. No System boundary, UML include/extend,
scenario order or pre/postcondition is invented. Empty input produces a truthful
inventory/gap page, not a fictitious diagram. Unsupported overview filters and
oversized native-profile aggregates are rejected without silent truncation.

SDPTool advertises optional overviewTarget only for this implemented projection.
Existing targets and detail behavior remain unchanged. XFMD validates target kind,
operation, project, discovered model and revision, then passes the exact URI to
its existing request/bundle workflow. Label/Enter activates the overview; expansion
is generation-free. Older producers, including empty viewpoint nodes, show an
unavailable-overview message rather than executing broad selection.

The real XFMD model at revision
e2a2f39e5c78476bb1bba08e406114367eb59953f02a63e952e435c880d24007
supplies one actor, eleven use cases and eleven pursues relations. All are retained.
The source model was not modified. See [discovery](live-discovery.json),
[selection result](live-selection.json), [generated page](live-view/entry.md),
[manifest](live-view/manifest.json) and [source](live-view/diagrams/VP01-overview.mmd).

## Verification and visual corrections

- [SDL tests](sdl-tests.log): complete viewpoint and documents packages pass,
  including 30 use cases, isolated items, empty inventory, unsupported filters,
  explicit traversal rejection and unchanged old detail selection.
- [SDPTool tests](sdptool-tests.log): facade package passes, including returned-target
  generation, exact revision, foreign/stale rejection and no invalid publication.
- [Normal build](build-final.log) and [final consumer tests](tests-final.log) record
  14/14 passing tests (six GUI) on the final candidate, including native interaction, protocol, runtime, SDUI
  regression, diagram layout/reading and vector PDF. No whole-repository test claim.
- [Initial suite](tests-initial.log) retained one new GUI failure: the harness lacked
  explicit X11 input focus. Its corrected native-focus/current-row test passes in
  the final suite. Existing workflow tests passed on the initial run too.
- [Native fixture](vp01-native.png) and [PDF](vp01-native.pdf) exercise actual
  SDPTool discovery/selection through XFMD. The fixture includes an isolated actor
  and isolated use case. The GUI test checks Enter and label clicks, exact target,
  no selection on Right, a native use-case scene and generated-page content.
- [Real-model window](live-vp01.png), [PDF](live-vp01.pdf),
  [extracted PDF text](live-vp01.txt), [native log](native.log) use the normal
  Release libraries. [Capture script](capture_native.py) and [probe](native_probe.cpp)
  preserve the reproduction; use a separate output copy to keep these records.
  The real export has seven pages; page two contains the complete diagram.
  All twelve actor/use-case source IDs are present in extracted PDF text.
- [Document checks](document-checks.log) and
  [producer checks](producer-document-checks.log) record consistency validation.

Initial TB projection produced an unreadable horizontal fan-out for the real
model. before-orientation artifacts preserve it. LR improves native reading through
vertical scrolling. Transport query JSON stays in selection.json and is omitted
from the overview's reading page. This is a presentation choice, not a model edit.
Large diagrams still require scrolling/zoom; no optimal-layout claim is made.

The tall LR diagram exposed a PDF rounding defect: scale-to-height multiplication
could exceed the printable boundary by one ULP. pdf-height-failure-native.log and
pdf-height-failure-capture.log preserve the rejection. DiagramPlacement now limits
the final height shared by drawing and pagination. DiagramReadingTest includes a
2065.6-point synthetic A4 regression; the real-model PDF is checked after the fix.
PageBreaker's oversized-line rule and continuous layout remain unchanged.

## Candidate and scope

[Candidate identity](candidate.json) records both dirty shared HEADs, captured
source hashes/archives, exact binaries and the unchanged installed XFMD0.7 hash.
Source archives include prior shared work and are not claims that this turn authored
all those files. Scoped patches compare preserved before bytes for files captured
at entry; new files are in the archives. Before Markdown copies use .md.txt to
preserve historical bytes without presenting them as live linked documents.
The Mermaid pin is unchanged from N3c. External dependencies are not a release
package; the Go module manifests and normal build configuration identify them.

Producer work unit:
file:///home/warloc/git/SDP-vNow/SDP/05--Implementation/SDL/ViewpointNavigation/Plan.md

No root-wide commit/index reset, independent review, release, installation or gh
extension upgrade. No authored XFMD system model changes. Producer KB032 is active
for remaining viewpoint work; XFMD Session0005 and KB028 remain active. The next
increment is VP07 architecture/feature overview with explicit mode scope; proper
focused use-case notation and scenario/condition/source work follow.
