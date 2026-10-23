---
id: FUNC-032
kind: Functionality
audience: User
role: Workflow
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-046, SR-029
uses: FUNC-001, FUNC-002, FUNC-005, FUNC-006, FUNC-007, FUNC-008, FUNC-010
---

# Functionality-032: SDP project navigation

## 1. Purpose and scope

UC-012 adds project-aware KanBan/SDL/SDUI browsing and selected generated views.
The owning work is [PLAN-XFMD-0006](../../../SDP/05--Implementation/SDP-Navigation/Plan.md).
Existing FUNC-031 explicit navigator/endpoint behavior remains a separate consumer.

## 2. Requirements and acceptance

UR-046/SR-029; AT-074, AT-075 cover the native workflow and asynchronous failure
boundaries. Paired source/derived preview remains a later #015 increment; #021 opens source
through the existing native editor.

## 3. Contracts and ownership

Application owns SdpNavigation, SdpToolJob, SdpProtocol and SdpPanel. Tool jobs
run outside FOX; presentation and document admission occur on the owning window's
main thread. SdpProtocol decodes sdptool/0.2 through an application-local Rust
serde_json bridge (already locked/offline dependency), not an SDL parser. The
bridge has a separate crate; it does not add SDP semantics to Mermaid layers.

Discovery returns a bounded graph with typed targets. Use returned model IDs,
URI, revision, frame entry and absolute open paths. Host selection prefers the private accompanying SDPTool, then gh sdp;
XFMD_SDP_TOOL overrides one executable (standalone sdptool), never a shell string
or a project manifest command. Inherit the host's SDP_OFFLINE/cache policy.

## 4. Behavior, state and failures

Recognize only the current directory named SDP or its immediate real SDP child;
no ancestor search. Discovery establishes actual validity. Candidate errors have
visible SDP diagnostics; a non-SDP root hides the tab. Refresh invalidates old
requests and tree activations. Keep Files and Index independent.

NavigationTree expands graph children lazily with an ancestor/reference guard,
depth and total-row bounds. Unknown operations have no executable effect.
SDUI source rows expand frames; explicit frame selection generates its preview.
Open targets use existing native document opening, not desktop launch. All
producer paths must remain inside the selected SDP area; generation results must
remain inside the private request bundle. Child processes use argv, a process
group, cancellation, a deadline and bounded output. No shell is involved.

Each selection captures the document token. Dirty buffers reject generated
selection/delivery without saving or a late modal. Root changes, refresh and newer
selections cancel the job; a changed document token rejects delivery even if the
source revision matches. Generated sdl-view links retain their originating model
and revision. Bundles survive normal Back/Forward while current/history entries refer to them.
T007 removes the cumulative admission stop; reclaim unreachable bundles only when
preview/export workers are idle. Destroy services/render workers before final cleanup.

## 5. Plumbing

| Step | Event / caller | Called symbol | Source or contract file | Data / result | Failure / side effect | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | Application workspace root/refresh | `SdpNavigation::refresh` | `src/application/sdp/SdpNavigation.cpp` | root → discovery request | cancel prior request; clear old graph | Implemented |
| 2 | SdpNavigation | `SdpToolJob::start` | `src/application/sdp/SdpToolJob.cpp` | argv → bounded background result | process group canceled/reaped | Implemented |
| 3 | SdpToolJob | `decodeSdpReply` | `src/application/sdp/SdpProtocol.cpp` | JSON → application DTO | reject unknown schema/invalid graph | Implemented |
| 4 | SdpNavigation result | `SdpPanel::present` | `src/application/ui/SdpPanel.cpp` | graph → lazy native rows | diagnostics remain visible | Implemented |
| 5 | SdpPanel activation | `SdpNavigation::activate` | `src/application/sdp/SdpNavigation.cpp` | typed target → open/select/frame | containment, revision and dirty checks | Implemented |
| 7 | Background source/card changes | `SdpWatch::take` | `src/application/sdp/SdpWatch.cpp` | coalesced change → rediscovery | bounded scan errors require manual Refresh | Implemented |
| 6 | SdpNavigation poll | `Application::open` | `src/application/Application.cpp` | admitted bundle entry → main document | changed token/failed read preserves document | Implemented |
| 8 | Source row / descendant projection | `sdpSourceForNode` | `src/application/sdp/SdpSources.cpp` | explicit model / inherited context → source ID | conflicts disable source action; no label parsing | Implemented |
| 9 | Native right-button dispatch | `SdpTree::handle` | `src/application/ui/SdpTree.cpp` | row → source context callback | cancel delayed normal activation | Implemented |
| 10 | Source context callback | `SdpPanel::context` | `src/application/ui/SdpPanelContext.cpp` | source ID → copied target and native popup | defer graph destruction; invalidate stale action | Implemented |
| 11 | View source selection | `SdpNavigation::openSource` | `src/application/sdp/SdpNavigation.cpp` | current catalog target → original source in Editor | path/revision checks; normal dirty transaction | Implemented |
| 12 | SdpNavigation generated-detail allocation | `sdpRuntimeWorkspace` | `src/application/sdp/SdpRuntime.cpp` | runtime root → private window workspace | reject missing/unsafe runtime; no /tmp fallback | Implemented |
| 13 | SdpNavigation idle watch poll | `SdpNavigation::prune` | `src/application/sdp/SdpNavigation.cpp` | current/history paths → retained contexts | defer while preview/export consumes resources | Implemented |
| 14 | SdpPanel present/expand/restore | `sdpDisplayChildren` | `src/application/sdp/SdpDisplay.cpp` | immutable SdpReply + source parent → ordered display child IDs, transparent healthy wrappers and deduplicated viewpoints | diagnostics retained, no generation or SDL parsing | Implemented |
| 15 | SdpPanel row creation/activation | `sdpDisplayRow` | `src/application/sdp/SdpDisplay.cpp` | original node + ancestry/source → canonical node and effective contextual target | terminal valid references retained; inert rows omitted, source conflicts disable source action | Implemented |

| 16 | Source association, target construction and opening | `sdpTextSource` | `src/application/sdp/SdpSources.cpp` | catalog kind + suffix → supported original-text source | reject kind/suffix mismatches and unknown kinds | Implemented |

| 17 | SDUI selected generation | `sduiExecutable` | `src/application/sdp/SduiTools.cpp` | private installed/build helper or explicit override | missing helpers preserve legacy producer preview; launch disabled | Implemented |
| 18 | Generated-page launch context | `SdpNavigation::currentSdui` | `src/application/sdp/SduiSource.cpp` | current document/project/catalog → entry target | stale or unrelated documents have no action | Implemented |
| 19 | SDUI source link | `SdpNavigation::followSduiSource` | `src/application/sdp/SduiSource.cpp` | snapshot + byte span → original Editor selection | reject changed source, invalid range and dirty cancel | Implemented |
| 20 | Scheduled contextual tools | `SduiWorkflow::poll` | `src/application/sdp/SduiWorkflow.cpp` | document context + producer check → enabled action/reason | cancel obsolete checks; process failure surfaced | Implemented |
| 21 | CommandRouter SDUI Launch | `SduiWorkflow::launch` | `src/application/sdp/SduiWorkflow.cpp` | checked entry/revision → separate Fyne process | argv-only spawn, runtime log, no navigation-owned cancellation | Implemented |
| 22 | Generated SDL source link | `SdpNavigation::followSdlSource` | `src/application/sdp/SdlSource.cpp` | retained multi-file snapshot + UTF-8 byte span → original Editor selection | reject stale graph, unsafe path, invalid range and dirty cancel | Implemented |

| 23 | SdpPanel deferred source | `SdpNavigation::expandModel` | `src/application/sdp/SdpDiscovery.cpp` | target → serialized model request | validate catalog identity; retain retryable rows | Implemented |
| 24 | Model response | `SdpPanel::mergeModel` | `src/application/ui/SdpPanel.cpp` | graph + request → refreshed native snapshot | preserve current document and selection | Implemented |
| 25 | SdpPanel merge | `expandSdpModel` | `src/application/sdp/SdpExpansion.cpp` | current/model snapshots → candidate snapshot | atomic project/revision/namespace validation | Implemented |

## 6. Reuse and dependencies

Keep all SDP code in application. Reuse NavigationTree and document services;
no parser/runtime duplication, renderer subprocess registration or project-side
metadata mutation. Use private temporary output, never the source directory.
Automatic refresh uses an application-owned SdpWatch background metadata scanner.
It inspects only the selected SDP area (no symlink traversal), at most 20,000
entries/depth 32, with interruptible waits. A stable pair of samples coalesces
writes; continuous changes publish at least every two seconds. Errors disable
watching with a visible manual-Refresh diagnostic. Native event dispatch never
scans directories. Discovery is queued behind an in-flight generation; it never
replaces the document. Surviving canonical row identities restore expansion and
selection. Root changes discard the old graph; Refresh within a root preserves
its presentation state. Generated documents are snapshots and are not regenerated
automatically.

## 7. Verification

Initial evidence is recorded in [SN-001](../../../SDP/05--Implementation/SDP-Navigation/evidence/SN-001/README.md). Tests must exercise a real producer and
native tree/document delivery, with fake tools only for deterministic failures.
A compiled binary alone does not establish AT-074, AT-075 or independent review.

## 8. Status, risks and change impact

Implemented first increment, 2026-10-01. Protocol/producer and focused sanitizer
checks pass. SN-002 adds native GUI, graph bounds, automatic refresh and history
acceptance; independent review and the remaining M3 boundary checks are pending. Broad Mermaid grammar/render compatibility
remains #020; preserve source and report failures. Generated-document bundles use
current/history ownership with idle reclamation (T007), instead of a broker lease. Abrupt termination can leave private
temporary files; no recovery or global cleanup service is claimed.

SN-002 continues native acceptance and records the owner-authorized development
installation: [evidence](../../../SDP/05--Implementation/SDP-Navigation/evidence/SN-002/README.md).

### T004 / M3a refinement — selected design

Discovery owns the ordered root IDs, source catalog and expansion-depth limit.
Decode at most 32 roots and 256 source entries; reject duplicate/missing roots,
duplicate sources and malformed relative source paths. Keep the existing graph
limits and use the stricter of producer depth and the native 32-level cap.
SdpPanel creates tabs only for root nodes of kind tab; selected tab and row state
use IDs, not indices. Retain unavailable tabs/diagnostics; removal selects the
first available tab, then the first diagnostic tab. Zero tabs leaves an explicit
status with Files/Index intact. Viewpoint references use canonical labels;
other aliases and card Ref identity remain unchanged. FOX row tooltips expose
full labels. Producer-supplied A-level labels are displayed without a dictionary.

Implemented plumbing for this increment: SdpSources resolves explicit target.model
and the current producer's exact source-node identity sdl/<sources.id> or sdui/<sources.id>, including
invalid source rows, then passes that source context down the rendered branch.
Conflicting associations yield no source action. Labels never determine paths.
Sources remain regular .design (catalog kind sdl) or .sdui (kind sdui) files inside the selected SDP area. Composed
models open the catalog entry source, not an inferred declaration file.
SdpTree owns right-click dispatch; SdpPanelContext owns the native View source
popup. Copy source identity before modal dispatch, defer tree destruction while
a popup is open and invalidate its action on refresh/root changes. Apply pending
graph changes after dispatch. SdpNavigation::openSource rechecks project/model/
revision/path against the current accepted catalog, cancels obsolete generation
and uses Application::open plus Editor mode with existing Save/Discard/Cancel.
Source access is separate from #015 source/derived-preview pairing.

T004/T005 verification is recorded in
[SN-003](../../../SDP/05--Implementation/SDP-Navigation/evidence/SN-003/README.md).
Owner T005 explicitly holds authored SDL/SDUI and generated model views pending
Git/release baseline and diff-blueprint discussion (#022). This textual design
was updated before native implementation; no model-conformance update is claimed.

### T007 runtime and demand-driven selection — design selected before code

PLAN-XFMD-0008 / KB-XFMD-025: SdpRuntime selects XDG_RUNTIME_DIR or
/run/user/<uid>, validate absolute/user-owned/private directory identity and
allocate a unique private workspace beneath xfmd. No automatic /tmp fallback.
Allocation happens on generated-detail selection, never discovery. SdpPanel
activation of a viewpoint with children expands that row without producer select;
concrete diagram/frame requests retain existing typed actions. SdpNavigation::prune removes only contexts outside current/history resource
ownership when parser/preview/export work is idle. The 64-context/256-MiB total
stop is removed; per-result validation remains, with outstanding #025 limits
explicitly unfulfilled. No SDL/SDUI source or upstream producer changes.

### Session 0003 refinement — N2 implemented; N3/N4 pending

[PLAN-XFMD-0009](../../../SDP/05--Implementation/SDL-Navigation/Plan.md) and
[SDL navigation design](../../../SDP/04--Design/SDL-Navigation/Design.md) select a
lazy application display projection over immutable discovery. Healthy phase/A-level
wrappers become transparent only in SDL source context. Deduplicate canonical
viewpoints per source, preserve occurrence identities for contextual references,
and prefer an explicit origin reference target over its canonical object's target.
Keep actionable cyclic references as terminal links; omit inert placeholders.
Real model errors and genuine truncation remain visible through diagnostics/status.

Projected child rows, stable occurrence keys and effective targets are distinct
from canonical graph nodes. Refresh and native expansion use the same projection;
SdpPanel must not restore hidden A-level routes or dispatch through the resolved
node when that loses the reference's viewpoint-specific target.


SdpPanel additionally calls sdpDisplayRow (SdpDisplay.cpp) to resolve original versus
canonical identities, retain effective targets and source/diagnostic context, and
terminate expansion. SdpItem owns the display-parent occurrence key, canonical node
ID and source association; refresh uses that key, with origin-ID fallback for moved
entries. The projected children and row responsibilities are separate helper calls.

N2 recorded a proposal for optional producer overviewTarget using the existing
typed target shape. N3d below adopts the producer contract and decoder/dispatch. A supplied overview opens on label/Enter; expansion
box/Right only expands. N2 retains explicitly labelled older-producer expansion
behavior. Exact source ranges (#021), overview projection semantics and Mermaid
use-case compatibility (#020) remain separate dependencies, not new XFMD parsers.

### SDUI source access — Session 0004 U1

Extend the existing SdpSources association and SdpNavigation::openSource boundary
to SDUI files and frames, including invalid source rows without preview targets.
Kind and extension must agree; conflicting source identities disable the action.
SDL display-wrapper flattening remains SDL-only. Open source uses the existing
Editor transaction and preserves dirty cancel, containment and revision guards.
No SDUI parsing, runtime launch, exact-line highlighting or diagram change is
selected in U1. See PLAN-XFMD-0010 for subsequent launch contract work.

### SDUI combined preview and host integration — Session 0004 T002

SDUI producer owns per-container composition mindmaps/provenance and static text; XFMD does not parse
SDUI. Bundled sdui-preview accepts a catalog-bound source, entry, revision and
caller-owned output directory. It returns the existing generated-view envelope,
plus source.sdui and sdui.json for source provenance. Source links are contextual
sdui-source://START/END UTF-8 byte ranges, not arbitrary filesystem navigation.

SduiWorkflow owns asynchronous check and independently lived native processes.
The sdptool/0.2 sdui-check operation carries source/entry/revision, prototype or
prototype-unbound status and a diagnostic. No SDL runtime is inferred. The
helper is separate from the owner's gh sdp installation. In 0.9, the private
SDPTool owns discovery/SDL generation, with gh sdp as fallback. Optional development builds without helpers retain the
older text preview; 0.7 release acceptance requires both bundled binaries.

Toolbar follows the existing sidebar tab context. All four groups remain visible
when that tab is active. KanBan Add/Move are disabled because the mutation
service is outside this SDUI increment. Fyne is a separate window: no speculative
FOX/Fyne embedding. Process logs use the existing private runtime area. Closed
successful children release logs; failed/detached processes retain diagnostic
logs until runtime cleanup. SdpNavigation retains request orchestration; source
span handling and host lifecycle are separate files to limit responsibility.

### VP01 overview target — Session0005 N3d

Producer Contract.md now adopts optional overviewTarget using the existing typed
select target and SDL-owned projection=overview URI. SdpProtocol decodes it only
on viewpoint nodes and validates project, discovered SDL model, matching source
revision and select fields. SdpPanel label/Enter activation uses that canonical
viewpoint target; expansion stays generation-free. SdpDisplay continues preserving
original detail targets. Missing overview shows the explicit older-producer status,
including empty viewpoints; it never calls the old broad-selection target. The
producer generates complete VP01 actor/use-case notation plus a realization table;
VP07 is not advertised yet. Existing SdpNavigation request, freshness, dirty-buffer,
runtime-bundle and history boundaries apply unchanged. No SDL parsing in XFMD.

### N3e producer compatibility — Session0005 T005

The existing generic overviewTarget and ordinary diagram-row target paths consume
VP07 logical and mode-specific projections without new native product code.
SdpOverviewGuiTest proves native activation, mode isolation and native/PDF scenes.
SDL owns the architecture/annotation projection, SDPTool owns typed mode targets;
XFMD does not reconstruct modes or Feature semantics. Large-overview reading remains
an explicit acceptance gap in N3e-001, not an asserted visual completion.

### N4 implemented source/detail contract

[SDL source/detail design](../../../SDP/04--Design/SDL-Navigation/N4-Source-and-Details.md)
adds optional generated result sourceLinks and context-bound sdl-source links.
SDL/SDPTool owns checked fact/declaration spans and source snapshots; application
owns containment, freshness, dirty-buffer opening and exact editor selection.
Implemented plumbing: decodeSdpReply validates sourceLinks; generated
Context retains them; SdpNavigation::followSdlSource compares every source snapshot
before/after normal opening and selects a valid UTF-8 byte range. No PG mutation.

N4 verification: SdpProtocolTest and SdlSourceGuiTest cover composed sources,
non-ASCII offsets, original-file selection, changed dependencies, unsafe paths,
malformed spans, history and dirty cancellation. See N4-001.

### VP02 compatibility — Session0007 N5

The generic overviewTarget path also consumes the producer's complete logical
architecture projection. No native product change or viewpoint dictionary is
needed. SdpOverviewGuiTest verifies VP02 Enter versus expansion, isolated
structure, source references and native/PDF output while preserving VP01/VP07.
See N5-001; the producer capability remains dependent on the actual installed tool.

### VP03 compatibility — Session0007 N6

The unchanged generic overviewTarget path consumes complete VP03 responsibility
and capability projections. Producer-owned annotations distinguish owns, realizes
and provides; XFMD neither infers these semantics nor parses the SDL. Extended
SdpOverviewGuiTest proves discovery/Enter versus expansion, exact source links,
complete native scenes and PDF output while retaining previous viewpoints.
N6-001 records full producer and 12 affected consumer tests. Installed availability
still depends on the released tool; N5/N6 are not merged or installed.

### VP04 compatibility — Session0007 N7

The unchanged generic overviewTarget path consumes a producer-owned interface
consumption overview. SDL selects all declared interfaces and explicit consumers,
with source-bound proof and a register; no provider/transport is reconstructed in
XFMD. The existing typed target/freshness/source handling is sufficient; native
product code is unchanged. SdpOverviewGuiTest is extended for advertised VP04
leaf activation through Right/Enter, isolated interfaces, native scenes and PDF.
Existing branch-only expansion checks remain. Evidence is
recorded under N7-001; installed availability depends on the released producer.

### VP05 compatibility — Session0007 N8

The unchanged native overviewTarget/select path consumes a mode index and
viewpoint-qualified overview-mode rows. Producer requires facts stay isolated by
mode, including empty-mode targets; no mode default or inheritance in XFMD.
SdpOverviewGuiTest protects index expansion/selection, Offline/Online/Quiet scope,
source links and native/PDF scenes. Actual NormalDesktop's absent capabilities
produce explicit empty output. N8-001 records full producer and 12 consumer tests;
installed availability depends on later producer integration/release.

### VP06 compatibility — Session0007 N9

Existing overviewTarget/select/source plumbing consumes two producer-owned graphs:
activity refinement and separate prerequisites. XFMD does not infer schedules,
status or coverage. The native integration test now verifies exact VP06 selection,
both complete scenes including isolated activity, dashed prerequisite edges,
source-claimed status register/source links and PDF. Actual XFMD empty activity
scope remains explicit with 28 uncovered Functionality links. N9-001 records
producer and consumer evidence; installed availability awaits producer release.

### VP08 compatibility — Session0007 N10

Existing overviewTarget/select/source plumbing consumes VP08 index and named-mode
pages without native product changes. Producer pages partition complete topology
by Channel; grouped sender/receiver edges retain every message binding/proof.
The native test covers index selection versus branch expansion, mode isolation,
idle channels and original Scenario sequence selection. N10-001 verifies all 40
actual channel diagrams and 123 edges through native/PDF, with source registers.
The initial 60-node union exhausted routing budget (KB020); whole-root discover
hits the pre-existing inventory guard (KB025). Explicit model tree/select succeeds.
No general resource, renderer or released-availability claim is made.

### VP09 compatibility — Session0007 N11

Existing overviewTarget/select/source handling consumes producer-owned Dataset
neighborhoods and complete source-linked storage/contract/shape/projection registers.
No native product or source-model change. SdpDataOverviewChecks exercises overview
selection versus expansion, persistent versus Unit-held data, projection provenance,
unused definitions and original Contract detail selection. N11-001 verifies actual
3 diagrams/9 nodes/6 edges plus all 44 Contracts/14 Fields through native/PDF.
Exact model tree/select works; existing whole-root discovery guard remains KB025.
Installed availability still depends on selected integration/release.

### VP10 compatibility — Session0007 N12

Existing overviewTarget/select/source handling consumes producer-owned separate
packet diagrams and complete source-linked Encoding/Variant/field registers.
No native product or authored-model change. SdpEncodingOverviewChecks verifies
overview versus expansion, two opposite-order packet layouts, source references,
original single-packet detail and PDF. N12-001 records exact fixture positions,
34 source spans and actual empty-state evidence. Packet narrow-label clipping and
crowded boundary numbers reproduce in the existing renderer and remain KB020;
register completeness does not imply diagram visual acceptance. Whole-root
discovery remains KB025; exact model tree/select works. Integration/release pending.

### VP11 compatibility — Session0007 N13

Existing overviewTarget/select/source handling consumes producer-owned snapshot,
property, gap and complete source registries. SdpTraceabilityOverviewChecks verifies
branch expansion versus selection and a property source link selecting exact editor
bytes; source mode is intentionally Editor, with Preview explicitly restored for
subsequent tests. No native product or authored model changes. N13-001 verifies
actual 312 declarations/1034 facts/195 properties, 1346 exact source spans and full
PDF retention. Composed-source freshness and file/line provenance remain producer-
owned. All catalog overviews are now selected candidates; integration/release and
existing KB020/KB025/KB032 residuals are not closed by this increment.

### Deferred SDL graph loading — N15b

Discovery retains every source and diagnostic while deferring SDL model graphs
when advertised by the producer action catalogue. SdpNavigation owns serialized
requests/cancellation and revision identity. SdpProtocol decodes action capability
and model-only replies; SdpExpansion validates namespace, identity and every target
before merging a candidate snapshot. SdpPanel requests loading on source expansion
and preserves native selection/tab state during the replacement. Failures preserve
the prior snapshot and document and allow retry. Refresh discards loaded graphs.
The installed private SDPTool provides the matching additive contract; explicit
XFMD_SDP_TOOL overrides it and gh sdp remains the fallback. Existing byte/per-model
and presentation guards are separate KB025 follow-up obligations.
