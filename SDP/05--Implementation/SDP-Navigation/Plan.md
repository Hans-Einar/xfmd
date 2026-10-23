# PLAN-XFMD-0006 — Native SDP project navigation

| Field | Value |
| --- | --- |
| id | PLAN-XFMD-0006 |
| project | XFMD |
| state | active |
| PlanType | ImplementationPlan |
| BranchPolicy | current |
| CommitPolicy | milestone |
| SprintId | SPR-XFMD-0010 |
| Source cards | KB-XFMD-014, KB-XFMD-021, KB-XFMD-015 |
| Session | [SESSION-XFMD-0001](../../Sessions/session-%230001--SDP-project-navigation.md) |

## Outcome and authority

Owner 2026-10-01 authorizes progressing from the installed SDL/SDP upgrade to
working native project browsing and generated views, with as much implementation
as possible in this turn. This plan owns delivery; Session owns the continuing
route. #014 is executing; #021 implements the T003 sidebar refinement under T004 authority.
#015 remains selected for later M4, not yet executing.

## Baseline and design

[FUNC-032](../../../src/blueprint/functionality/Functionality-032--SDP-Project-Navigation.md)
and UR-046/SR-029 define the new boundary. Reuse WorkspacePanel, NavigationTree,
DocumentCoordinator, NavigationCoordinator and the Markdown/Mermaid pipeline.
Keep existing DocumentViews navigator/IPC compatibility independent.

Consume SDPTool 2.0.0 sdptool/0.2: discover --json already contains navigation;
select with returned model/URI/revision generates one caller-owned bundle.
SDUI frame targets use sdui-preview with explicit entry. Native code neither parses
SDL/SDUI nor invents model identities. Unknown/invalid sources remain visible.
No project-supplied executable is run. Default host command is gh sdp; an explicit
XFMD_SDP_TOOL executable selects a compatible standalone sdptool. No shell parsing.

Use asynchronous bounded child execution and main-thread result admission. Root,
selection and document changes invalidate pending delivery. Dirty-buffer delivery
is rejected without saving/discarding or a late modal. Keep generated bundles for
the window lifetime so Back/Forward and asynchronous rendering retain resources.
Cap aggregate retained bundles and report exhaustion; never silently evict history.
Automatic source/card watching now queues coalesced rediscovery behind generation,
restores surviving tree identities and preserves the current document. Watch
limits/errors expose manual Refresh. Generated documents remain snapshots.

## Git policy

Current working branch sprint/009/phase/057-favorites is not a protected target.
This typed plan selects current-branch work for the bounded first increment,
overriding the general phase-branch default for this new scope only. Milestone
commits remain intended at completed delivery boundaries. T001 Git metadata was
read-only; T002 has unrestricted access. This continuation preserves the existing
shared, uncommitted modeling/toolkit baseline and staged concurrent #020 card.
T002 authorizes a local development installation before the remaining milestone
checks, recorded by exact artifact hashes; it does not authorize publication.
No clean-commit or milestone-commit claim is made for this checkpoint.

## Milestones

| Milestone | Outcome and acceptance | State |
| --- | --- | --- |
| M1 | Requirements/blueprint, Session, producer boundary and bounded execution; protocol failures preserve callers | completed; SN-001 protocol/real-producer evidence |
| M2 | SDP visibility for immediate SDP area, inner KanBan/SDL/SDUI, lazy bounded tree, open cards and on-demand generated view; real producer test | completed; SN-002 real-producer native GUI acceptance |
| M3 | Dirty/root/document/selection races, invalid schema/tool/output diagnostics, refresh and leases; GUI/regression evidence | partial; native GUI, automatic refresh, graph bounds and Back/Forward pass; remaining admission/link/window-isolation evidence open |
| M3a | #021 dynamic tab roots/order/identity, canonical viewpoint names, producer-owned abstraction titles/fallback and View source context action; real protocol and native acceptance | implemented; SN-003 native/contract evidence; updated real Sessions producer test pending |
| M4 | #015 standalone saved .design preview with clear source/preview identity and no implicit save | planned |
| M5 | Reconcile SDL/feasible SDUI model and docs; independent review, actual final card dispositions | prior structural delta retained; further model edits held by T005/#022; semantic evidence and independent review pending |

## Verification and remaining work

AT-074 covers detection, native tabs/tree activation, card opening and real
producer-generated Markdown delivery. AT-075 covers malformed/oversized JSON,
unknown schema, process failure/cancellation/timeout, bounded graph expansion,
stale results and dirty-document retention. #015 acceptance is refined before M4.
Use temporary private projects and output bundles. Record command, source/binary
identity and environment; self-checks are not independent review. Installation,
release and closure are not implied by a local build.

Evidence is appended here when executed. Until then no test is claimed passed.

## T001 checkpoint — 2026-10-01

[SN-001 evidence](evidence/SN-001/README.md) records the actual candidate and
results. The first native path is implemented and built, but the host denies
Xvfb listener creation and ptrace injection. Do not close M2/M3 based on producer
or compile-only GUI results. No product installation or Git commit was attempted.

The canonical SDL model now has 38 blueprint mappings, 11 current use cases and
73 requirement identities. Six conceptual SDUI compositions and 248 generated
outputs reproduce with the recorded standalone tools. This reconciles the new
structural delta; the new async workflow still needs scenario-level modeling and
independent review. Historical XM-M2/XM-M4 evidence bytes remain unchanged.

## T002 checkpoint — 2026-10-01

[SN-002](evidence/SN-002/README.md) records native acceptance, automatic refresh,
regressions, installation identity and remaining limits. The owner explicitly
authorized installing the latest development candidate and continuing the Session.
The previous host restrictions no longer apply. #014 remains active; #015 stays
queued until M4 begins. No release, publication or independent approval is claimed.

## T003 contract and scope refinement — 2026-10-01

Owner asks to record dynamic SDP sub-tabs, descriptive A0–A5/VP01–VP11 names and
right-click View source in a card and this Session plan. The detailed contract,
acceptance and producer follow-up are in
[KB-XFMD-021](../../KanBan/active/%23021--Change--Dynamic-SDP-tabs-labels-and-source-opening.md).
This is planning, not an implemented extension or a rewrite of completed M2.

Observed installed discovery already supplies tab roots/kinds, full canonical
viewpoint labels and sources[].id/source. XFMD currently discards roots/sources
and hardcodes three tabs. Viewpoint references carry short aliases; resolve the
canonical label for display. A0–A5 descriptive titles are absent from this JSON:
request producer-owned titles, retain code fallback and do not duplicate the
language catalog in XFMD. The inspected producer returns no Sessions tab yet.
A future published producer/client update is distinct from upgrading project SDP;
no upgrade is executed or claimed here.

### Next bounded work

1. Test #021/M3a with the owner-provided updated gh-sdp release: verify Sessions
   appears from returned roots and opens its returned file targets. The generic
   tab/ref-label/source implementation and controlled extra-tab fixture are in
   SN-003. Do not claim the new producer was tested before it is installed.
2. Finish M3 native boundary evidence alongside the refined sidebar: generated-link
   navigation, two-window isolation, hostile output-path/revision admission and
   window-shutdown bundle cleanup. Existing SN-002 covers cyclic/deep/wide graphs,
   dirty/root/document/selection races, auto-refresh and generated Back/Forward.
3. Begin #015 M4 after S2: saved .design source plus preview with separate source
   ownership, stale labels and explicit refresh/save semantics. #021's ordinary
   View source action does not deliver this source/preview pairing.
4. Per T005 owner steering, preserve .design/.sdui and generated model views until
   the Git/release baseline and diff-blueprint approach is selected. Retain M5
   semantic reconciliation as pending. Obtain independent review, then reconcile actual
   completion/card dispositions and form coherent milestone commits without
   absorbing the staged #020 card or unrelated shared baseline changes.

## T004/T005 checkpoint — 2026-10-01

[SN-003](evidence/SN-003/README.md) records #021 implementation and native/contract
checks. Dynamic tabs retain producer order/labels and selected root identity;
source opening uses catalog identities and the existing document transaction.
The installed producer still supplies code-only A0–A5 labels; descriptive titles
and actual Sessions discovery remain producer follow-ups, not hardcoded UI data.

Owner T005 preserves model sources/generated views pending
[KB-XFMD-022](../../KanBan/backlog/%23022--Idea--Git-based-design-baselines-and-change-blueprints.md).
Requirements and textual blueprint preceded native code. M5 remains outstanding;
no SDL/SDUI conformance or independent approval is inferred from passing CTest.

## T006 checkpoint — 2026-10-01

Owner selects product version 0.4 and authorizes local installation. [SN-004](evidence/SN-004/README.md)
records the version-only product delta, seven passing targeted checks, installed
candidate hash and previous 0.3 backup. Milestone/card states and T005 model-source
hold are unchanged. This is a development installation, not public publication.
