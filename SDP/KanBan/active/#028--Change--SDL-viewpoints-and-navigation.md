# SDL navigation cleanup and meaningful viewpoint overviews

| Field | Value |
| --- | --- |
| id | KB-XFMD-028 |
| project | XFMD |
| type | Change |
| CardState | in-progress |
| created | 2026-10-06T21:49:41.481757+00:00 |
| source | Owner Session 0003 T001, 2026-10-06 |
| next_review | Review/integrate stacked VP02–VP06 and VP08–VP11 candidates; prioritize retained KB020/KB025 and legacy eager-export work. Release, shared-history reconciliation and large-print acceptance remain |

## Owner priority and desired behavior

This work precedes SDUI in Session 0003. Remove A0–Ax grouping from the normal
SDL tree; do not change SDL abstraction semantics. Show a viewpoint once per
selected model/System, with readable names and expandable detail nodes. Preserve
model/System context and stable identity; cross-model names are not globally unique.
Whether phase headings remain is a design choice: recommended default is model →
viewpoint → detail to avoid introducing another duplicate hierarchy.

Hide non-actionable reference/depth-limit placeholders from the normal tree.
Retain cycle/depth protections internally and keep valid references clickable.
Do not hide genuine invalid-source/tool diagnostics or make unreachable content
look complete; expose real truncation in an appropriate status/diagnostic surface.

## Overview and detail generation

- Clicking a viewpoint title generates a genuine overview of that viewpoint;
  expansion affordance opens its children without generating every detail page.
  This deliberately supersedes the temporary #025 behavior where title activation
  only expands. Keep its runtime ownership and on-demand guarantees.
- Use-case overview: all declared actors, use cases and relevant functionalities
  within the selected model/System, with their actual model relations. Use proper
  Mermaid use-case notation. Keep functionality as supporting realization context,
  not relabelled as actors/use cases. Define a clear notation/companion section for it.
- A use-case child generates a focused diagram plus a sequence diagram where SDL
  provides ordered participants/interactions/scenario facts. A static dependency
  graph alone does not prove message order. List pre/postconditions when explicitly
  modeled or derivable by a documented rule with source references. Distinguish
  derived statements from authored ones; say not specified when evidence is absent.
- VP07 overview: the complete selected architecture with features overlaid on the
  containers/units/functionality they affect. Feature selection shows focused
  realization detail. Do not substitute a bag of unrelated detail graphs for an
  architecture overview. Use labels/legend and source IDs, not color alone.
- Review every remaining viewpoint's question, overview semantics, detail target,
  SDL facts, missing data and renderer requirements. A generic graph is not proof
  that the intended viewpoint is useful. Keep explicit per-viewpoint dispositions.
- Every source ID links to exact source span/revision using #021. No arbitrary
  selection cap or silent omissions; coordinate producer work and #025.

## Evidence and ownership

SDPTool/tree.go currently creates phase → abstraction → viewpoint-reference rows;
that explains duplicate viewpoints. XFMD SdpPanel::add follows references and
adds the non-actionable reference/depth-limit suffix when ancestry reaches its
bound or cycles. Native activation currently expands parent viewpoints. This is
source inspection, not fresh native acceptance.

Upstream KB-SDP-032 already owns viewpoint usefulness feedback; extend that study
rather than create another producer viewpoint card. SDL owns projection semantics;
SDPTool owns navigation/actions; XFMD owns tree/activation/rendering/source jumps.
KB-XFMD-020 owns usecase-beta compatibility and the supplied VP07 Libavoid failure.
Mermaid's current official docs describe usecase-beta (12.0.0+); this is not proof
that XFMD's pinned Rust parser/renderer supports it. Test the exact native pipeline.

## Bounded sequence and acceptance

1. Capture producer/consumer navigation fixture; select unique viewpoint structure,
   retained valid references and separate click/expand behavior before code.
2. Deliver the tree cleanup with stable selection/history/refresh and no loss of
   reachable details. Do not change producer URI semantics by guesswork.
3. Coordinate use-case overview/detail and VP07 projection, exact source links and
   required #020 renderer work. Native/PDF fixtures must show intended semantics.
4. Record all other viewpoint dispositions and concrete successors for missing
   model facts; do not invent sequences or conditions to fill a page.

Include multiple Systems, cross references/cycles, deep hierarchy, empty/invalid
models, 25+ use cases, cancellation/stale revisions and large overview behavior.
Model-source hold remains in force until explicitly reconciled; this card records
requirements to design, not permission to rewrite authored .design/.sdui files.

[Session 0003](../../Sessions/session-%230003--SDL-navigation-and-SDUI.md). Planning/capture only; no implementation claimed.

## T002 design and queue — 2026-10-06

Prepared [PLAN-XFMD-0009](../../05--Implementation/SDL-Navigation/Plan.md) and
[selected design](../../04--Design/SDL-Navigation/Design.md). Queue N2 first:
consumer tree normalization with the current producer, preserving original
reference targets (canonical resolution currently risks switching context to VP11).
N3/N4 coordinate overview semantics, proper notation and source spans; no product
implementation or producer contract agreement is claimed by this planning step.
Predecessor: Session 0003 S2/N1 design preparation; ready for bounded N2 execution.

## T003 delivery checkpoint — 2026-10-07

PLAN-XFMD-0009 N2 implemented, built and installed as owner-selected XFMD 0.6.
[N2-001](../../05--Implementation/SDL-Navigation/evidence/N2-001/README.md) records
88/88 tests, actual native tree/source/navigation screenshots, 168 installed file
hashes and prior-version backup. Card remains in-progress: producer aggregate
viewpoints, proper use-case/VP07 rendering and source-span work in N3/N4 are not
completed by the tree cleanup. No false whole-card or full-Session acceptance.

## Session 0004 route — 2026-10-07

Owner selects SDUI next, then a separate diagram Session. N2 remains delivered;
N3/N4 diagram work is deferred, not completed. Active plan/card lifecycle is
retained; no current execution of this card during Session 0004.

SDL diagram successor: [Session0005](../../Sessions/session-%230005--SDL-viewpoint-diagrams.md), created after SDUI0.7 delivery. Recover live producer contracts before N3/N4 execution; no new completion claim.

## Session0005 T003 result — 2026-10-08

[N3c-001](../../05--Implementation/SDL-Navigation/evidence/N3c-001/README.md)
delivers native use-case notation in the working Release candidate: actors,
ellipses/rectangles, system boundaries, associations and explicit UML relations.
Fourteen fork tests and 13 affected CTests pass with native/PDF inspection. Initial
detached actor edges were rejected and corrected before this result. Installed0.7
is unchanged. The card remains in-progress; aggregate VP01/VP07 and exact SDL
provenance remain producer work. See [handoff](../../04--Design/SDL-Navigation/Producer-Handoff.md).

## Session0005 T004 result

N3d delivers the first VP01 overview through SDL projection, SDPTool overviewTarget
and native XFMD activation. Complete actor/use-case inventory includes isolated
items; functionality realization stays separate. Real XFMD source retains one
actor, 11 use cases and 11 pursues relations. Native inspection selected LR after
rejecting the unreadably wide TB fan-out. Query metadata stays in selection.json.
The tall real diagram also exposed a one-ULP paged-height overshoot; DiagramPlacement
now bounds the final height and has a synthetic plus real PDF regression.
Go viewpoint/documents and SDPTool facade tests pass. Native and affected consumer
checks pass, including Enter/click, expansion-only, older/empty producer fallback,
stale/foreign targets and PDF export. The first new GUI attempt lacked explicit
X11 focus; its failing log is preserved alongside corrected passing evidence.

[Evidence](../../05--Implementation/SDL-Navigation/evidence/N3d-001/README.md). Installed0.7 unchanged. Remaining VP07/detail/provenance work stays in Session0005.

## Session0005 T005 — N3e

N3e implements lazy VP07 logical overview and named-mode targets. All declared
System/container/unit structure is retained. Feature keys annotate real owners and
selected-mode containers through proven Functionality paths; the legend and table
retain all Features, contributions and allocation gaps. No cross-mode union or
invented direct relation. Native Enter/selection exercises both mode targets.
Ten affected CTests pass (five GUI); SDL viewpoint/documents pass and the SDPTool
facade suite passes with one documented pre-existing SDUI test excluded. That test
also fails in an isolated copy with N3e edits removed: UIPreview uses Combined,
while the all-family test compares against Markdown. Do not claim full producer
suite success or change another workstream's acceptance in this increment.

The literal graph timed out; the adopted annotation projection renders the real
XFMD model (23 architecture nodes, 19 contains edges, 140 proof annotations and
137 facts) and exports an eight-page vector PDF. Small-fixture rendering is readable.
Real-model full-view labels remain small at fit-to-width / A4. Grouping disconnected
nodes was explored but not adopted: it improved native width while worsening PDF
pagination. Large-overview reading is still an acceptance gap owned by KB020/028;
no overall visual acceptance or Session completion is claimed.

[Evidence](../../05--Implementation/SDL-Navigation/evidence/N3e-001/README.md). Remain active for the named successors.

## Owner feedback — Session0005 T007, 2026-10-09

Owner reports VP07 DocumentReferences detail failing with Libavoid NoSpace:
label attachment intrusion on edge 11. Reproduced through the same candidate
SDPTool target, normal native renderer and PDF probe: zero native diagram scenes;
PDF contains the exact error. This is a 12-node/15-edge detail graph, separate
from the newly added VP07 aggregate and its fit-to-width reading gap.

The strict renderer check in layout/routed/labels.rs rejects another edge crossing
the attachment zone between an edge and its label; the error is not an SDL mode
validation failure or invalid Mermaid declaration. One bounded expanded-spacing
retry already exists and is insufficient for this fixture. Root cause within label
placement/obstacle routing is not yet established. Preserve strict geometry,
source semantics, fixed candidate pin and existing original routing regression.
Next implementation boundary: reproduce with exact text metrics in the Mermaid
fork, correct attachment routing/placement, then test original and new fixtures
through native preview/PDF before dependency adoption. No guard removal or source
model rewrite is selected as a workaround.

NormalDesktop is the model's sole declared mode (XfmdDesktop.design line 180),
used by allocations, channel participation and scenarios. It denotes ordinary
standalone desktop execution; the name has no built-in runtime switch semantics.
Selecting it in VP07 filters modeled allocation facts, not the running app's mode.

[Reproduction](../../05--Implementation/SDL-Navigation/evidence/N3f-001/README.md). Cards remain active; no repair or approval claimed.

## N3f delivery — Session0005 T009

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

[Evidence](../../05--Implementation/SDL-Navigation/evidence/N3f-002/README.md). Owner confirms preserving NormalDesktop; no model edit.

## N3g delivery — Session0005 T010

ChapterNavigation caption ambiguity is repaired in the pinned renderer fork.
The ten-detail VP07 sweep now passes10/10; native/PDF inspection and all targeted
checks pass. [N3g-001](../../05--Implementation/SDL-Navigation/evidence/N3g-001/README.md)
contains exact evidence. Remain active for large-overview reading and the existing
viewpoint/detail/source successors. NormalDesktop and authored-model hold persist;
installed 0.7 is unchanged.

## Local 0.8 installation — Session0005 T011, 2026-10-10

The verified working candidate through N3g is installed as 0.8; full 91/91 CTest and
installed smoke checks pass. [R08-001](../../05--Implementation/SDL-Navigation/evidence/R08-001/README.md)
records the exact package and backup. Existing residual scope remains active;
installation does not close the card or claim independent/public acceptance.

## Owner pause — Session0005 T012, 2026-10-10

Session0005 is paused in favor of [Session0006](../../Sessions/session-%230006--Discoverable-SDPTool-actions.md) / KB030.
CardState in-progress → ready records the paused handoff; active placement is
retained. Resume only when the owner resumes the diagram Session. The 0.8 delivery
and remaining obligations are unchanged; no completion is implied.

## Session0005 T013 — resumed

Owner resumes diagram work while KB030 waits for its API release. N3h selects
opt-in full-size diagrams with existing horizontal scrolling/zoom in Wrap.
A4/PDF readability and remaining projection/source work stay open.

N3h outcome: full-size continuous diagram reading is built and verified (19/19
affected CTests, 11 GUI, plus actual VP07 native workflow). See
[N3h-001](../../05--Implementation/SDL-Navigation/evidence/N3h-001/README.md).
Default fit and A4/PDF remain; large-overview print readability and remaining
projection/source scope are not complete. Installed0.8 unchanged.

## Session0005 T016 — producer integration candidate

Local SDP commit cc7b52f isolates and verifies the earlier VP01/VP07 producer
work, including composed-source freshness/provenance and the native XFMD workflow.
[N3i-001](../../05--Implementation/SDL-Navigation/evidence/N3i-001/README.md) records
identity and handoff. Integration/release is outstanding; no overall card closure.

## N3j current-main compatibility — Session0005 T019

Producer successor ecd9dcd on sdp/xfmd-viewpoint-overviews-main is based directly
on fetched main e8760d3. Both scoped conflicts are resolved; full module tests,
Actions overview compatibility and native XFMD/PDF checks pass. Final merge
forecast is clean. Use this successor for integration, preserving cc7b52f as
history. Evidence: ../../05--Implementation/SDL-Navigation/evidence/N3j-001/README.md.
No consumer code change, card transition, main merge, release or installation.

## Session0005 T021 — N4 implementation and residual handoff

[N4-001](../../05--Implementation/SDL-Navigation/evidence/N4-001/README.md)
delivers focused use cases/explicit scenarios and original composed-source spans.
Fixed 24/48 diagram-count caps are removed, with 50-diagram coverage. Producer
commit fde2239 follows ecd9dcd on the main-based successor; full producer/native
checks pass. Main/PG merge forecasts are clean; no merge/release/install.

[Current disposition](../../04--Design/SDL-Navigation/Viewpoint-Disposition.md)
records exactly what this card still owns. Session implementation closure does
not complete this broader card or waive large-print, compatibility, remaining
resource limits, integration or review obligations. CardState is unchanged.

## Session0007 T001 — integration and VP02

Owner requests merging current work and continuing with the next viewpoint.
N3j/N4 is merged/pushed to SDP main (5ca0e90). VP02 is implemented and verified
separately (cdf4437, sdp/xfmd-vp02-overview): complete structural inventory,
contains edges, isolated roots and exact-source register; old details preserved.
[Session0007](../../Sessions/session-%230007--Architecture-viewpoint.md) and
[N5-001](../../05--Implementation/SDL-Navigation/evidence/N5-001/README.md) record
full producer/affected native checks and actual-model PDF/reading evidence.
The card stays in-progress for remaining scope. VP03 is proposed next; no N5
main merge, release or install is claimed. Shared XFMD product work is preserved.

## Session0007 T002 — VP03 candidate

N6 is committed/pushed as 63da7ae on sdp/xfmd-vp03-overview, stacked on VP02
cdf4437. It retains complete architecture/responsibility/capability inventories
with distinct ownership, contribution and provision annotations and exact source
proofs. Existing detail targets remain. Full SDL/SDPTool suites, vet and 12 native
consumer tests pass; actual XFMD 23 nodes/19 edges/28 responsibilities render and
export. No capability is invented for the current model. Long PDF identifiers
wrap; large-print readability remains open. VP04 interfaces is proposed next.

No N5/N6 merge/release/install, production model edit or shared producer product
change. Broader cards/plan stay active and independent review remains outstanding.

file:///home/warloc/git/xfmd-sdl-navigation/SDP/05--Implementation/SDL-Navigation/evidence/N6-001/README.md

## Session0007 T003 — VP04 candidate

N7 85d62f2 on sdp/xfmd-vp04-overview stacks on N6 63da7ae. Complete interface
inventory, explicit dashed consumption graph and exact-source register preserve
the ordinary Interface use table. No provider/transport/call/necessity inference.
Full SDL/SDPTool suites, vet and all 12 affected consumer tests pass. Actual XFMD:
14 interfaces, 12 consumers, 24 edges; all names retained in PDF, full-size reading
verified. Native Right-on-leaf activation is preserved and tested. Large-fit
packing/print readability remains open. VP05 mode-specific dependencies is next
proposed. The broader plan/cards remain active; no merge/release/install or
production model edit. Shared producer product files/history remain untouched.

file:///home/warloc/git/xfmd-sdl-navigation/SDP/Sessions/session-%230007--Architecture-viewpoint.md

file:///home/warloc/git/xfmd-sdl-navigation/SDP/05--Implementation/SDL-Navigation/evidence/N7-001/README.md

## Session0007 T004 — VP05 candidate

N8 3b3d935 on sdp/xfmd-vp05-overview stacks on N7 85d62f2. VP05 now provides a
mode index plus isolated per-mode dependency diagrams/registers; all declared
capabilities and empty cases remain visible. No mode inheritance or combined
graph, provider or sufficiency inference. Original details and VP07 overlays
remain. Full producer suites/vet and 12 native consumer tests pass; native/PDF
mode isolation and actual XFMD's empty NormalDesktop output are verified.
VP06 activities/delivery planning is proposed next. Broader cards/plan stay active.
No merge/release/install, authored model/native product change or PG mutation.

file:///home/warloc/git/xfmd-sdl-navigation/SDP/Sessions/session-%230007--Architecture-viewpoint.md

file:///home/warloc/git/xfmd-sdl-navigation/SDP/05--Implementation/SDL-Navigation/evidence/N8-001/README.md

## Session0007 T005 — VP06 candidate

N9 76fc87f on sdp/xfmd-vp06-overview stacks on N8 3b3d935. Complete
activity refinement and separate prerequisites retain isolated activities, with
source-linked status, responsibility, Feature and Scenario registers. Status is
source-claimed; missing addresses does not imply missing native implementation.
Full SDL/SDPTool suites, vet and 12 affected consumer tests pass. Native/PDF checks
verify both graphs and all statuses. Actual XFMD has no Activities: its explicit
empty plan and all 28 uncovered Functionality links are verified. VP08 channels/
scenario overview is proposed next; VP07 is already delivered. Broader plan/cards
remain active; N5–N9 await integration. No merge/release/install, authored model or
native product change, PG mutation, independent approval or large-print acceptance.

file:///home/warloc/git/xfmd-sdl-navigation/SDP/Sessions/session-%230007--Architecture-viewpoint.md

file:///home/warloc/git/xfmd-sdl-navigation/SDP/05--Implementation/SDL-Navigation/evidence/N9-001/README.md

## Session0007 T006 — VP08 candidate

N10 d677d70 on sdp/xfmd-vp08-overview stacks on N9 76fc87f. VP08 has an
index and explicit mode targets, complete per-channel participation diagrams and
source-linked channel/binding/scenario registers. Original scenario sequences
remain on demand. The initial union failed Libavoid's routing budget; final
channel partitions retain all 60 unique nodes, 123 grouped edges and 202 uses
facts across 40 diagrams. All 22 scenarios and node names survive PDF; 850 source
spans are in bounds. Full producer suites/vet and 12 native consumer tests pass.

Actual model tree/select succeeds. Whole-project discovery hits the existing
20000-node/32 MiB guard on N9 and N10; KB025 retains that obligation. KB020 retains
the failed union renderer fixture. No generic large-print acceptance, independent
approval, native product/authored model change, merge/release/install or PG change.
Extended Session round complete; broader plan/cards remain active. N5–N10 await
integration. VP09 contract/storage overview is next proposed; VP07 already exists.

file:///home/warloc/git/xfmd-sdl-navigation/SDP/Sessions/session-%230007--Architecture-viewpoint.md

file:///home/warloc/git/xfmd-sdl-navigation/SDP/05--Implementation/SDL-Navigation/evidence/N10-001/README.md

## Session0007 T007 — VP09 candidate

N11 6b80b40 on sdp/xfmd-vp09-overview stacks on d677d70. Complete data/storage/
contract inventories and Dataset neighborhoods retain source proofs and original
detail selections. Registers distinguish explicit persistence, source-claimed
completeness, optional fields and ternary projections without SQL/runtime inference.
Full producer suites/vet and 12 consumer tests pass. Actual XFMD's three Unit-held
Datasets render as 3 diagrams/9 nodes/6 edges; all 44 Contracts and 14 Fields survive
PDF, with 358 checked source spans. Persistent stores, Datagram/projection behavior
and unused shapes are verified by the native fixture. Contract count columns were
combined after PDF inspection; final artifacts were reverified.

Extended Session round complete; broader plan/cards stay active. N5–N11 await
integration. VP10 encodings/packets is next proposed. Existing discovery/routing
limits remain KB025/KB020. No merge/release/install, native product/authored model
change, PG mutation, independent approval or general large-print acceptance.

file:///home/warloc/git/xfmd-sdl-navigation/SDP/Sessions/session-%230007--Architecture-viewpoint.md

file:///home/warloc/git/xfmd-sdl-navigation/SDP/05--Implementation/SDL-Navigation/evidence/N11-001/README.md

## Session0007 T008 — VP10 candidate

N12 f1c3342 on sdp/xfmd-vp10-overview stacks on 6b80b40. Complete Encoding/
Variant coverage and separate validated packet diagrams retain exact source links,
byte/bit order and ordered field positions/types/owners. No serialization or layout
is inferred. Original detail selection is unchanged. Full producer suites/vet and
12 consumer tests pass. Native fixture verifies two 29-bit packets and both orders;
34 source spans and all six placement statements check exactly. Actual XFMD has
no Encodings/Variants; its empty page is verified natively and in PDF.

Visual inspection reproduces clipped narrow-field labels and crowded adjacent bit
numbers in overview and original detail. KB020 retains this renderer follow-up;
new source registers remain complete. PDF text retention is not visual acceptance.
Initial native-test synchronization failure was corrected by waiting for the new
document/render token; final reruns pass. Evidence: XFMD N12-001.

Bounded projection round complete; broader plan/cards and renderer/discovery
residuals remain active. N5–N12 await integration. VP11 properties/traceability/gaps
is next proposed. No merge/release/install, native product/authored model change,
PG mutation, independent approval or general visual/large-print acceptance.

file:///home/warloc/git/xfmd-sdl-navigation/SDP/Sessions/session-%230007--Architecture-viewpoint.md

file:///home/warloc/git/xfmd-sdl-navigation/SDP/05--Implementation/SDL-Navigation/evidence/N12-001/README.md

## Session0007 T009 — VP11 candidate and catalog handoff

N13 486b29d on sdp/xfmd-vp11-overview stacks on f1c3342. Complete properties,
snapshot counts, existing cross-viewpoint gaps and declaration/fact registries
retain exact source links and file/line context. Counts/properties are source
assertions, not implementation evidence; no new gap rules or inferred defaults.
Original VP11 selection remains unchanged. Full producer suites/vet and 12 consumer
tests pass, including composed-source freshness and native exact property selection.

Actual XFMD: 312 declarations, 1034 facts, 195 explicit properties, one reported
NO_ENCODINGS gap and 1346 exact source spans. Every fact sentence and declaration
name survives the 88-page PDF. Native fixture has 47 declarations/87 facts with
cross-viewpoint gap context. Inspected native Preview/source selection and sampled
PDF pages; no independent or general large-print acceptance. N13-001 retains inputs,
checks and artifacts. Source-link test restores Preview explicitly after Editor.

All catalog overviews now have selected development candidates. N5–N13 await
integration; N3j/N4 remains on main. Next: reconcile the stacked integration
candidate and prioritize retained KB020 acceptance/renderer work. KB025 discovery
guard and KB032 legacy eager-export alignment also remain open. Broader plan/cards
remain active. No merge/release/install, native product/authored model change or
PG mutation. Shared producer receives only this existing-card/Integration handoff.

file:///home/warloc/git/xfmd-sdl-navigation/SDP/Sessions/session-%230007--Architecture-viewpoint.md

file:///home/warloc/git/xfmd-sdl-navigation/SDP/05--Implementation/SDL-Navigation/evidence/N13-001/README.md
