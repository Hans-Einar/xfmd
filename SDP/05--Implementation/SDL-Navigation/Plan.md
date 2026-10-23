# PLAN-XFMD-0009 — SDL navigation and viewpoint refinement

| Field | Value |
| --- | --- |
| id | PLAN-XFMD-0009 |
| project | XFMD |
| state | active |
| PlanType | ImplementationPlan |
| BranchPolicy | current |
| CommitPolicy | milestone |
| Source cards | KB-XFMD-028; KB-XFMD-020; KB-XFMD-021; KB-XFMD-025 |
| Session | SESSION-XFMD-0007 (continues SESSION-XFMD-0005 and SESSION-XFMD-0003) |

## Outcome and authority

Owner Session 0003 T001 chooses SDL cleanup before SDUI; T002 continues the
announced bounded design/planning step. The first implementation milestone is
usable without new producer projections. Owner T003 (2026-10-07) authorizes implementation, build and installation as 0.6.
N2 execution is active; N3/N4 retain their declared cross-system prerequisites.

The [selected design](../../04--Design/SDL-Navigation/Design.md) defines tree
projection, reference targets, interaction, producer proposal, every viewpoint's
disposition and verification. UR-046/SR-029 and FUNC-032 hold the native boundary.
The owner's model-source hold remains in force; no .design/.sdui edits are selected.

## Git and delivery policy

Use current working branch sprint/009/phase/057-favorites for this bounded increment,
with milestone commits when a coherent scoped candidate can be committed. The
shared dirty tree contains earlier runtime/model/process work and staged #020;
do not commit unrelated content or claim the entire tree belongs to this plan.
Capture exact source/diff/binary evidence until integration can preserve that work.
No push, merge, tag, version bump or installation follows from selecting this plan.
The previous authorized 0.5 installation remains tracked in Session 0002.

## Milestones

| Milestone | Deliverable and acceptance | State |
| --- | --- | --- |
| N1 | Design and responsibility split; actor/viewpoint source evidence; meaningful dispositions for VP01–VP11; explicit compatibility path | prepared T002; document checks recorded below |
| N2 | Unique SDL viewpoint tree, no inert cycle/depth leaves, retained context-specific targets/diagnostics and stable refresh; unit + real-producer native tests | completed; N2-001 native/full-suite/package/install evidence |
| N3 | Coordinate producer overview contract and VP01/VP07 projections; explicit overview activation separate from expansion; native proper-use-case and routing support | selected implementation delivered through N4; large-overview publication acceptance remains active under KB020/028 |
| N4 | Exact source-span navigation and source-supported sequence/condition details; cross-viewpoint disposition and integrated acceptance | implemented/verified N4-001; per-viewpoint residual disposition recorded |
| N5 | Complete VP02 structure/contains overview, source-linked register, original details and native/PDF acceptance | completed candidate cdf4437; N5-001, separate branch |
| N6–N13 | Complete VP03–VP06 and VP08–VP11 selected overviews with preserved source/detail behavior | completed stacked candidates through 486b29d; per-increment evidence below, integration and retained acceptance work remain |
| N14 | Correct narrow packet labels and adjacent endpoint numbers; review integration readiness | completed local candidate 49bb0ee; N14-001 native/PDF, 7 fork tests and 24 CTests; integration remains separate |
| N15a | Additive deferred discovery and revision-bound model-tree service | completed producer e194721; N15-001 service and compatibility evidence; native N15b delivered below |
| N15b | Native deferred model expansion and authorized local application/producer installation | completed N15b-001: 94 tests, actual project, package and installed checks; retained KB025 guards remain |

N2 remains compatible with installed producer discovery. It must explicitly label
unavailable aggregate views rather than triggering bulk generation. It does not
claim #028 complete. N3/N4 are cross-system milestones: first select corresponding
producer implementation under its own plans. Backlog handoff is not producer delivery.
SDUI #027 and toolbar #029 follow this Session route under their own bounded plans.

## Acceptance and evidence

Run projection tests and affected SdpPanel/SdpDynamic/SdpNavigation native checks.
Exercise the actual gh sdp fixture, multiple independent models, valid references
with per-viewpoint targets, cycle/depth guards, diagnostics, keyboard/pointer
expansion, dirty/history and refresh behavior. Record the exact candidate and
native evidence. For N3/N4 use source-derived completeness checks plus inspected
native/PDF output; no fabricated scenario order or conditions. Required fixtures
and error behavior are in the design, including the original VP07 routing report.

Current evidence: source/JSON inspection only. No product test pass or independent
review is claimed. N1 validation results are appended after document checks.

## N1 design checkpoint — 2026-10-06

Prepared design and queued N2. Canonical requirements and FUNC-032 updated with
planned behavior. Local board validation PASS (29 cards, 114 card events, 15
management records); blueprint validation PASS (38 blueprints, 75 requirements);
symbol check PASS (297 implemented callees, new display symbol correctly Planned);
upstream ledger schema/replay/lineage PASS; git diff --check PASS in both repos.

[Discovery inspection](../../04--Design/SDL-Navigation/evidence/discovery-inspection.json)
records snapshot hash and sample original/canonical target differences. The graph
contains 1,765 reference-bearing nodes with differing select targets; this is not
a count of proven user-visible failures. Native reproduction remains N2 work.
No product code, native acceptance, integration commit or independent review.

## T003 execution selection — 2026-10-07

Owner requests implementation, Release build and installation as XFMD 0.6. Execute
N2's selected consumer boundary now and verify the previously blocked runtime
checks in the unrestricted environment. Producer overview and source-span contracts
remain proposals, not existing services; do not claim N3/N4 delivered by changing
the tree. Record the exact shipped boundary and remaining dependencies in release
notes. No model-source edits or public publication are selected.

## N2 delivery — Session 0003 T003, 2026-10-07

[N2-001](evidence/N2-001/README.md) identifies the exact 0.6 candidate, 88/88 CTest
results (42 GUI), inspected native screenshots, package and installed receipt.
N2 is delivered. N3/N4 remain pending producer contracts/projections, #020 renderer
work and #021 exact-source navigation. No broad #028 completion, independent
approval, public publication or authored-model update is claimed. Milestone commit
is deferred to avoid incorporating unrelated shared/staged baseline; exact source
archive and hashes retain the tested identity.

## T004 route disposition — 2026-10-07

Owner selects Session 0004 SDUI before further diagrams. N3/N4 execution is
deferred until the later diagram Session; this plan stays active and incomplete.
N2's installed evidence is unchanged. See Session 0004 / PLAN-XFMD-0010.

## Successor Session selected — 2026-10-07

SDUI implementation/install is delivered as 0.7. Resume remaining N3/N4 through
[Session0005](../../Sessions/session-%230005--SDL-viewpoint-diagrams.md), beginning
with live producer/renderer contract recovery. This handoff selects no new release
version, authored-model changes or unverified producer capabilities.

## Session0005 T001 — N3a execution selection

Owner says to proceed with the recommended #020 → #028/#021 round. Reuse this
ImplementationPlan and current branch/milestone policy. N3a first recovers live
producer capabilities, reproduces VP07/nested-layout failures using the exact
XFMD pipeline, and selects the smallest evidence-backed compatibility correction.
Routing belongs to the pinned Mermaid fork, never hidden .deps edits or automatic
Legacy fallback. Native/PDF geometry and retained facts must support any fix.
No new release/install, authored model edits or upstream publication selected.
Record remaining aggregate-view/source-span contracts before dependent work.

N3a bounded consumer correction: the supported mindmap shape parser eagerly
validates an unused plain-label fallback, rejecting quoted shaped labels. Select
a lazy branch, with retained-label/shape/parent tests and native/PDF verification.
This is independent of the reproduced VP07 fork defect and does not claim its fix.

## N3a first increment — Session0005 T001, 2026-10-08

[N3a-001](evidence/N3a-001/README.md): contract recovery/reproductions complete;
quoted mindmap labels corrected in normal consumer build;22 parser and12 affected
CTest passes plus inspected native/PDF. VP07 routing fix is independently tested
through the actual C++ consumer against local Mermaid fork commit e79d8f9caf649995a5db5e54b0ea5d6b7fab8221,
with13 fork tests and retained12-node/18-edge native/PDF output. Normal pin and
installed0.7 unchanged; dependency adoption remains next, no public release.
N3/N4 remain incomplete: aggregate targets, native use-case notation, feature
overlays, exact SDL provenance and nested hierarchy still need their own work.
Current milestones/branch policy remain; no source-model hold lifted.

## Session0005 T002 — N3b execution selection

Owner continues S2. Adopt fork e79d8f9caf649995a5db5e54b0ea5d6b7fab8221 using
the exact checksummed source archive described in the current design. No public
push/release or install selected. Add a normal C++ VP07 regression; verify bootstrap
integrity/refusal paths, normal Release build, affected tests and native/PDF output.
Keep Cargo.lock unchanged if the fork's dependency graph is unchanged. Preserve
previous .deps source in a separate ignored directory rather than patching it.

## N3b delivery — Session0005 T002

[N3b-001](evidence/N3b-001/README.md): renderer adoption delivered in normal Release
build; original VP07 measured regression and native/PDF retain 12 nodes/18 edges.
13 affected CTests pass. Cargo.lock and installed0.7 unchanged. Exact source archive
and manifest replace reliance on an unpublished checkout; no .deps patch/publication.
Root milestone commit remains deferred for the shared baseline; candidate archive
and hashes preserve evidence. N3/N4 stay incomplete pending producer overview and
source contracts, native use-case notation and related acceptance.

## Session0005 T003 — N3c execution selection

Implement the [native use-case profile](../../04--Design/SDL-Navigation/UseCase-Profile.md)
under existing KB020/028 intent. Update FUNC023/024, domain contracts, parser and
fork layout/SVG, then verify native/PDF and relevant regressions. Reuse current
XFMD branch and an isolated local fork branch; no public publication or install.
Producer overview and exact-source contract adoption remain separate work.

## N3c delivery — Session0005 T003

[N3c-001](evidence/N3c-001/README.md) records the native use-case profile in the
normal Release candidate: contract/parser/fork conversion, visible actor/ellipse
notation, boundaries and semantic relations. Fourteen fork tests and 13 affected
CTest passes; native/PDF inspection includes attachment correction. Candidate
source archive and binary hashes identify the shared dirty tree; installed0.7
and lockfile remain unchanged. See the explicit layout limitations in evidence.
N3/N4 stay incomplete: producer aggregate targets/projections, SDL provenance,
related scenario/condition views and nested flowchart containment remain open.
Next execution follows the selected producer handoff; no upstream state changed.

## Session0005 T004 — N3d execution selection

Implement the VP01 producer projection, typed overviewTarget advertisement and
native consumer activation as one vertical increment. Reuse this plan and current
branch/milestone policy; producer work unit is in SDP-vNow's SDL/ViewpointNavigation.
Preserve concurrent shared edits and exact tested source evidence. VP07 projection,
focused scenario/conditions and exact-source N4 are explicitly remaining work.
No authored XFMD model change, release, extension upgrade or install selected.

## N3d delivery — T004

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

[Evidence](evidence/N3d-001/README.md). Installed0.7 unchanged. Remaining VP07/detail/provenance work stays in Session0005.

## N3e selection — Session0005 T005, 2026-10-08

Deliver VP07 as a complete logical architecture base (all declared System,
container and unit nodes, including isolated nodes), overlaid with every Feature
and its contributing Functionality. Preserve actual contains, owns and
contributes-to edges, with distinct feature/functionality shapes and relation
styles. Do not fabricate placement from ownership. The default overview is
explicitly logical and omits allocation edges; discovery adds one lazy overview
selection per declared mode. A selected mode includes only its allocated-to
facts and explicitly reports unallocated contributions. Never union modes.
Existing feature detail targets remain usable. This uses the N3d typed target;
no XFMD parser or hardcoded viewpoint list is introduced.

A single union of all allocations is rejected because modes may be incompatible.
Generating all mode diagrams on activation is rejected in favor of on-demand
selection. Full structural base means decomposition, not every unrelated
interface/data/scenario relation. All declaration/relation provenance remains.
Verify isolation, multiple modes, unallocated/isolated features, empty models,
old VP01 behavior, actual discovery-to-native selection and real XFMD PDF.
The current branch/shared-baseline policy from N3d remains; no release/install,
model edit or broad commit. Exact source links remain N4.

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

[Evidence](evidence/N3e-001/README.md). Remain active for the named successors.

## N3f execution — Session0005 T009, 2026-10-09

Owner confirms keeping NormalDesktop and asks to continue. Prioritize the exact
DocumentReferences detail regression from N3f-001. Keep renderer responsibility
in the fork, source pin immutable and application geometry untouched. The selected
measurement-derived spacing refinement is specified in
[repair design](../../04--Design/SDL-Navigation/N3f-Routing.md). Preserve current
branch/shared-tree policy; local scoped fork commit is allowed, no broad root
commit, publication, release or installed-version change. Normal candidate native
checks and PDF inspection are required before calling this increment delivered.

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

[Evidence](evidence/N3f-002/README.md). Owner confirms preserving NormalDesktop; no model edit.

## N3g delivery — Session0005 T010, 2026-10-09

The remaining known VP07 detail-routing failure is repaired; the ten actual
feature details render successfully. [N3g-001](evidence/N3g-001/README.md) records
fork/native/PDF checks and the exact pinned candidate. N3 remains ongoing for
large-overview reading and focused projection acceptance; N4 remains pending.
No root-wide milestone commit, installation or publication is claimed.

## R08 installation checkpoint — Session0005 T011

Owner-selected 0.8 snapshot through N3g is locally installed. Full 91/91 CTest and
installed startup checks pass; [R08-001](evidence/R08-001/README.md) retains exact
identity and prior 0.7 backup. N3/N4 remain incomplete for their declared successors.
No public publication, producer upgrade or broad integration commit selected.

## Execution pause — Session0005 T012, 2026-10-10

Owner pauses the diagram Session and selects [Session0006](../../Sessions/session-%230006--Discoverable-SDPTool-actions.md) / KB030.
The plan stays active in the installed lifecycle, with execution explicitly paused.
Resume S2/N3 on owner request; preserve the verified 0.8 delivery and all N3/N4
residual scope. KB020/028 are ready under the board's paused-handoff rule.

## N3h selection — Session0005 T013, 2026-10-10

Owner resumes diagrams while KB030 waits. Implement the opt-in reading behavior
in [N3h design](../../04--Design/SDL-Navigation/N3h-Reading.md), through existing
FUNC010/007/024 boundaries. Test native menu activation, pan/zoom, resize, dirty
state, selection, scene reuse, profile rejection and unchanged paged/PDF geometry.
No renderer dependency or producer changes are required. Preserve the current
branch and deferred broad-commit policy; capture the exact scoped candidate.
This closes only the continuous-view readability increment; N3/N4 remain open.

## N3h delivery — Session0005 T013

N3h provides opt-in full-size diagrams in Wrap using existing pan/zoom, with
default fit and A4/PDF preserved. Normal Release build and 19/19 affected CTests
(11 GUI) pass. A separate actual VP07 snapshot workflow passes; inspected panned
detail is readable. Exact source/binary identity, tests and screenshots are in
[N3h-001](evidence/N3h-001/README.md). Installed 0.8 is unchanged. No producer edit,
release, authored-model change, broad commit or independent approval.

Remaining N3: readable large-overview publication and focused use-case/scenario
projections. N4: agreed revision-aware per-file source links and remaining viewpoint
dispositions. Installed SDPTool 2.2.0 advertises no overviewTarget, so local N3d/N3e
producer work still needs integration/release. Recover current source-composition
provenance before N4. KB030's JSON action API remains a separate held workstream.

## N3i delivery preparation — Session0005 T016, 2026-10-18

Owner continues preparation of our producer delivery. Use isolated branch
sdp/xfmd-viewpoint-overviews rather than staging the shared producer tree.
Commit cc7b52f captures N3d/N3e code, consolidated contract/requirements and
composed-source regression. [N3i-001](evidence/N3i-001/README.md) records current
SDL/root SDPTool/native/actual-model verification and integration handoff. No
consumer product changes, merge, release or install; N3/N4 remain incomplete.

## N3j main compatibility — Session0005 T019, 2026-10-18

Owner selects reconciliation before further feature work. Producer BranchPolicy:
new isolated `sdp/xfmd-viewpoint-overviews-main` from main e8760d3; CommitPolicy:
scoped milestone. This replaces N3i's integration branch recommendation while
preserving its original commit. XFMD current branch policy is unchanged.

Commit `ecd9dcd5cd95fb2b1f323cea96da1776659b2513` resolves both scoped conflicts additively, retaining main's
Actions and blueprint behavior. Complete SDL/SDPTool suites, vet, new Actions
integration regression and native XFMD/PDF pass; final merge forecast is clean.
[N3j-001](evidence/N3j-001/README.md) holds exact evidence. No consumer product
changes or main merge/release/install. N3/N4 and broader KB032 remain incomplete.

## N4 execution — Session0005 T021

Owner selects completion. Implement the bounded [source/detail design](../../04--Design/SDL-Navigation/N4-Source-and-Details.md), preserving PG/blueprint ownership.
Producer continues main-based successor branch with milestone commits; XFMD
current branch policy remains. Verify real source/detail native workflows,
compatibility/error paths and complete affected suites; record all-viewpoint
and residual renderer/publication dispositions before Session closeout.

## N4 delivery and Session handoff — T021

[N4-001](evidence/N4-001/README.md) records focused use cases/explicit scenarios,
original composed-source selection, removed count-only limits, full producer and
native verification. Producer commit fde2239 follows ecd9dcd; XFMD remains an
exact shared-tree candidate without broad staging. Main and PG merge forecasts
pass; no integration/release/install is selected.

Session0005 closes its selected implementation round with
[explicit residual dispositions](../../04--Design/SDL-Navigation/Viewpoint-Disposition.md).
N4 is implemented; N3's large-overview publication acceptance remains open under
KB020/028. The plan therefore remains active, not completed. Other viewpoints
remain scoped producer proposals under KB032, not advertised capabilities.

## N5 execution — Session0007 T001, 2026-10-19

Owner authorizes main integration of the existing producer and continuation with
the next viewpoint. N3j/N4 merge 5ca0e90 is pushed; selected main tree equals the
tested fde2239. Select [VP02 design](../../04--Design/SDL-Navigation/N5-Architecture-Overview.md).
Producer BranchPolicy: new sdp/xfmd-vp02-overview from 5ca0e90; CommitPolicy:
scoped milestone. XFMD shared branch/evidence policy remains. Full producer plus
native overview/PDF checks verify the additive capability. N5 merge/release/install
is not implied by merging the previous delivery. Existing plan remains active.

### N5 delivery — Session0007 T001

[N5-001](evidence/N5-001/README.md) verifies VP02 through full producer suites,
12 affected native/consumer tests and actual-model rendering/PDF/reading. N3j/N4
merge 5ca0e90 is on origin/main; N5 commit cdf4437 is pushed on its own branch.
No native product code or installed version changes. Remaining plan obligations
retain their explicit disposition; VP03 is proposed next, not yet implemented.

## N6 selection — Session0007 T002

Owner continuation selects [VP03 responsibilities/capabilities](../../04--Design/SDL-Navigation/N6-Responsibilities-Overview.md).
Stack producer branch sdp/xfmd-vp03-overview on cdf4437 with milestone commits;
retain XFMD current shared-tree/evidence policy. Complete architecture plus
source-proven annotations/register, no feature/allocation inference. Verify full
producer and affected native/PDF workflows before delivery. No new merge/release.

### N6 delivery — Session0007 T002

63da7ae on sdp/xfmd-vp03-overview implements VP03 with complete architecture,
responsibility/capability registers and exact proof annotations. [N6-001](evidence/N6-001/README.md)
records full producer/vet, 12 consumer tests and actual-model native/PDF/reading.
Session0007 closes its extended round; the broader plan remains active. VP04
is proposed next. N5/N6 are stacked candidates, without merge/release/install.

## N7 selection — Session0007 T003, 2026-10-19

Owner continuation selects VP04 interface consumption. BranchPolicy:
sdp/xfmd-vp04-overview stacked on N6 63da7ae; CommitPolicy: scoped milestone.
Complete interface inventory with explicit consumes dependency graph/register,
no inferred provider/transport/necessity; preserve ordinary VP04 table selection.
N7 design selects the projection before code, with full producer/native evidence.
No authored model changes or merge/release/install; broader plan remains active.

### N7 delivery — Session0007 T003

85d62f2 on sdp/xfmd-vp04-overview delivers the complete interface consumption
projection/register with original table compatibility. [N7-001](evidence/N7-001/README.md)
records full producer suites/vet, 12 native consumer tests, actual model/PDF and
full-size reading. Native product is unchanged; Right-on-leaf behavior is retained
and verified. Session0007 closes the extended round; broader plan remains active.
VP05 is proposed next. N5/N6/N7 remain stacked candidates without merge/release/install.

## N8 selection — Session0007 T004

Owner continuation selects VP05 mode-index and isolated per-mode dependencies.
BranchPolicy: sdp/xfmd-vp05-overview from N7 85d62f2; milestone commit. N8 design
precedes code, preserving original details, VP07 overlays and authored models.
Verify producer/native/PDF workflows and actual XFMD's empty NormalDesktop scope.
No merge/release/install; broader plan and cards remain active.

### N8 delivery — Session0007 T004

3b3d935 on sdp/xfmd-vp05-overview delivers the mode-index and isolated per-mode
dependency diagrams/registers. [N8-001](evidence/N8-001/README.md) records full
producer suites/vet and 12 affected consumer tests, native mode switching/PDF
and actual XFMD's truthful empty NormalDesktop result. Original details/VP07
remain; native product code unchanged. Extended Session round complete, broader
plan active. VP06 is proposed next; no N5–N8 merge/release/install.

## N9 selection — Session0007 T005

Owner continuation selects VP06 complete activity decomposition, separate
dependencies and source-claimed status/commitment registers. BranchPolicy:
sdp/xfmd-vp06-overview from 3b3d935, milestone commit. N9 design precedes code.
Verify producer/native/PDF and actual empty XFMD plan; preserve detail identities
and authored models. No merge/release/install; broader plan remains active.

### N9 delivery — Session0007 T005

76fc87f delivers complete VP06 Activities/refinement, separate dependencies
and source-claimed status/commitment registers. [N9-001](evidence/N9-001/README.md)
records full producer suites/vet and 12 consumer tests with native/PDF checks.
Actual XFMD has no Activities; empty-plan and 28 uncovered Functionality source
links are verified. Native product remains unchanged. Extended Session complete;
broader plan remains active. N5–N9 await integration; VP08 is proposed next.

## N10 selection — Session0007 T006, 2026-10-20

Owner continuation selects VP08 mode-index, isolated communication topology and
scenario registers. N10 design precedes code. BranchPolicy: sdp/xfmd-vp08-overview
from 76fc87f; scoped milestone commit. Verify producer/native/source/PDF and actual
XFMD topology while preserving sequence details and authored models. No merge,
release or install; broader plan stays active. Concurrent preflight finds no current
PG/coordination overlap; shared history/product scope remains untouched.

### N10 delivery — Session0007 T006

d677d70 delivers VP08 index, mode-isolated channel neighborhoods and source
registers. [N10-001](evidence/N10-001/README.md) retains design refinement from the
failed large union to complete per-channel partitions, producer/native evidence,
all 40 actual diagrams/123 edges/202 uses and 22 scenarios in PDF. Full producer
suites/vet and 12 affected consumer tests pass. Root discovery remains limited
(KB025); explicit model tree/select works. Broader plan active; Session round
complete. N5–N10 await integration; VP09 is next. No merge/release/install.

## N11 selection — Session0007 T007, 2026-10-21

Select VP09 Dataset neighborhoods and complete storage/contract/shape/projection
registers per N11 design. BranchPolicy: sdp/xfmd-vp09-overview from d677d70;
scoped milestone commit. Verify producer/native/source/PDF, preserving original
details and authored models. No scoped PG/coordination overlap. No merge/release/
install; broader plan remains active.

### N11 delivery — Session0007 T007

6b80b40 delivers VP09 Dataset neighborhoods and complete data/storage/contract/
shape/projection registers. [N11-001](evidence/N11-001/README.md) verifies full
producer suites/vet, 12 native consumer tests, actual 3 diagrams/9 nodes/6 edges,
all 44 Contracts/14 Fields in PDF and 358 source spans. Native fixture covers
explicit persistent versus Unit-held data and projection/unused definitions.
Original detail behavior preserved; native product unchanged. Extended Session
complete, broader plan active. N5–N11 await integration; VP10 next. No release/install.

## N12 selection — Session0007 T008, 2026-10-21

VP10 Encoding inventory and separate validated packet layouts follow N12 design.
Branch sdp/xfmd-vp10-overview stacks on 6b80b40; scoped milestone commit.
No authored model changes or merge/release/install. Broader plan remains active.

### N12 delivery — Session0007 T008

f1c3342 delivers complete VP10 Encoding/Variant coverage with separate packets
and exact source-linked field layouts. [N12-001](evidence/N12-001/README.md) records
full producer suites/vet, 12 native consumer tests, two 29-bit fixture packets,
34 checked source spans and actual empty page/PDF. Original detail preserved;
native product unchanged. Narrow packet labels/crowded boundary numbers remain
an explicitly reproduced KB020 renderer defect. This projection round is complete;
broader plan active. N5–N12 await integration; VP11 next. No release/install or
independent/general visual acceptance.

## N13 selection — Session0007 T009, 2026-10-21

VP11 complete properties, traceability and reported model gaps follow N13 design.
Branch sdp/xfmd-vp11-overview stacks on f1c3342; scoped milestone commit.
No authored model changes or merge/release/install. Broader plan remains active.

### N13 delivery — Session0007 T009

486b29d delivers VP11 snapshot/property/gap/source registries. [N13-001](evidence/N13-001/README.md)
records full producer suites/vet, 12 consumer checks, native exact property-source
selection and actual 312 declarations/1034 facts/195 properties/1346 spans retained
through native and 88-page PDF. Original selection preserved; native product unchanged.
All catalog overviews have selected candidates. N5–N13 await selected integration;
broader plan remains active for retained KB020/KB025/KB032, review and publication.
No release/install or independent/general visual acceptance. This Session round is complete.

## N14 selection — Session0007 T010, 2026-10-21

Review the complete N5–N13 integration candidate, then correct the N12 packet
readability defect in KB020 using [N14 design](../../04--Design/SDL-Navigation/N14-Packet-Readability.md).
N14a: fork layout/tests and exact dependency adoption. N14b: native/PDF verification,
regressions and durable handoff. BranchPolicy: isolated renderer phase branch
`feature/xfmd-packet-readability` from 55eff5d; scoped milestone commit. XFMD retains
its shared current branch and exact changed-source evidence, deferring a commit
that would absorb unrelated staged/dirty work. No producer merge, publication,
release/install or authored model updates. This continues existing owner authority
to resolve the selected diagram work; independent approval remains separate.

### N14 outcome

N14 corrects the N12/KB020 packet renderer defect. The common measured bit
pitch preserves proportions, places endpoint numbers at their bit centers using
the actual SVG font, and separates adjacent multi-digit numbers. Overflow names
use numbered in-cell markers and full per-row legends with exact bit ranges.
Row-spanning fields retain the same ordinal. No producer source rewrite or ABI
change. Renderer commit 49bb0ee is locally committed and adopted via a checksum-
pinned archive; fork publication is separate.

Seven focused fork tests and 24 affected CTests pass (nine GUI tests), including
all existing diagram family/PDF and SDP overview/source/runtime regressions.
Native measured SVG, standalone native Preview and two one-page vector PDFs were
inspected; all field names are visible and the 12/13 PDF glyph boxes have a 5.55pt
gap. Original generated VP10 overview/detail workflow passes with full labels.
The development executable is built. Installed XFMD 0.8 and gh sdp are unchanged.

N5–N13 producer stack 486b29d has clean committed-tree merge forecasts against
main 5ca0e90 and PG 6dfde97. This does not establish independent semantic review
or authorize integration. Neither producer nor PG was changed. Broad KB020
large-print/VP08 routing acceptance, KB025 whole-project discovery/resource guards,
and KB032 eager-export alignment/integration remain open. Proposed next bounded
work: KB025 discovery of the growing project, starting with source/inventory policy.
No authored SDL/SDUI edits, merge, release, install or independent approval.

[Evidence](evidence/N14-001/README.md). N14a/N14b complete; broader plan remains active.

## N15 selection — Session0007 T011

N15a delivers the additive deferred SDL discovery and revision-bound single-model
tree contract described in N15. N15b integrates native expansion afterward.
Producer BranchPolicy: `sdp/xfmd-lazy-discovery` stacked on 486b29d; CommitPolicy:
scoped N15a milestone. Preserve shared dirty trees/history and authored-model hold.
No merge/release/install. Source/graph/GUI guard removal beyond this model-tree
boundary remains explicit KB025 work; do not mark the card complete.

## Session0007 T011 — deferred discovery producer

N15a candidate e194721 implements opt-in compact discovery with all sources
retained and revision-bound per-model expansion, shared by CLI and JSON actions.
Full SDPTool tests/vet, producer document validation and twelve affected native
compatibility checks pass. The actual checkout retains 63 sources in compact
discovery; XFMD expansion returns all eleven overview targets. The aggregate
fixture expands 22,236 nodes across twelve models without building one combined
graph. Default eager behavior remains compatible. See N15-001 evidence.

Next: N15b native capability negotiation, expansion/atomic tree merge and stale
response handling. Installed XFMD still uses eager discovery. Source parsing,
remaining resource guards, integration and independent review are not completed
by N15a. KB025 and the broader plan remain active; no authored models changed.

## T012 delivery — N15b / XFMD 0.9

Owner-authorized local installation is complete. N15b negotiates producer support,
loads source graphs on expansion, validates/merges atomically and preserves dirty
buffers, source actions and refresh state. Full final CTest passes 94/94 (47 GUI).
The actual project loads all eleven XFMD viewpoints and generates native VP02.
Installed discovery retains 63 sources; the installed GUI is observed invoking
its private SDPTool with lazy-sdl, without an environment override.

XFMD 0.9 and standalone/private SDPTool 2.2.0-dev at e194721 are installed with
per-file checksum verification and predecessor backups. Mermaid and SDUI/Fyne
installed smoke checks pass. gh-sdp's signed 2.2.0 default and project receipt are
unchanged. This is a shared-dirty development delivery, not a public release or
main integration. Source archive, candidate/package hashes, screenshots, logs,
checklist and installation receipt are in N15b-001.

Remaining KB025: source scanning/parsing costs, filesystem/byte/per-model guards,
GUI row/depth guards and demand-driven non-SDL inventories. Broader KB020/028/032
acceptance, integration, legacy export alignment and independent review remain.
No authored SDL/SDUI edits or unrelated shared-tree staging. Cards/plan retain
active scope; local installation does not close those residual obligations.
