# Extend the current SDUI tab with combined previews and explicit launch

| Field | Value |
| --- | --- |
| id | KB-XFMD-027 |
| project | XFMD |
| type | Proposal |
| CardState | gate-review |
| created | 2026-10-06T21:49:41.481757+00:00 |
| source | Owner Session 0003 T001, 2026-10-06 |
| next_review | Review installed 0.7 SDUI workflow and U4-001 evidence |

## Consolidation and authority

Owner requests one current SDUI work card. This supersedes the two local sources;
it does not close or absorb the upstream language/runtime backlog.

| Source | Scope carried forward | Disposition |
| --- | --- | --- |
| [#003](../superseded/%23003--Question--SDUI-widget-ownership.md) | Fyne/SDUI widget ownership and XFMD host boundary | Preserve evidence: Fyne owns its standalone window and can supply controls; fully custom controls remain an upstream question, not an XFMD prerequisite |
| [#026](../superseded/%23026--Proposal--SDUI-navigation-and-source-structure-views.md) | Existing tab, runnable readiness, structural projection, provenance and embedding experiment | Retain scope; latest owner selects a combined diagram + text page, superseding diagram-only/fallback framing |

Historical BoxUI/FOX-widget implementation is not revived. XFMD remains FOX;
SDUI frontend, widget semantics, layout and runtime remain upstream-owned.
The old custom-widget question is carried here as a deferred upstream decision;
neither this merge nor using Fyne-backed controls answers it by fiat.

## Selected desired workflow

- Extend the existing producer-declared SDUI tab. Use discovered source/entry IDs,
  not a second registry. Describe System membership only when supplied by producer.
- Selecting an entry generates one Markdown page containing both a recognizable
  composition diagram and the existing text-based SDUI render. Label the diagram
  as source structure and the text as a static prototype; neither is live execution.
- The installed `gh sdp . sdui-preview --model ID --entry FRAME --output DIR --json`
  already delegates to presentation.Markdown; Session 0003 evidence proves a real
  EditorWorkspace export. Reuse it. Missing addition: structural projection, joined
  page contract and provenance; existing text fidelity gaps have upstream owners.
- Prefer grouped frame/region composition with ordered row/widget labels and a
  separate binding/reuse view when useful. A flat block graph is not the selected
  default. Choose supported notation using fixtures and native/PDF inspection.
- Preserve declaration and instance IDs, reuse, hidden elements in the source view,
  layout overrides, bindings and source spans. Text visibility can legitimately
  differ: label that difference rather than silently losing hidden source nodes.
- Explicit SDUI toolbar action launches the Fyne interpreter for the current
  entry. Discover/preflight host, profile, entry and binding readiness. Distinguish
  runnable prototype, SDL-connected runtime, invalid source and unavailable host.
  A preview never launches code. Disabled action shows a reason.
- Investigate embedded Fyne preview as an optional feasibility step. No supported
  in-process FOX/Fyne bridge has been established. Separate Fyne window is the
  proposed first implementation; embedding needs focus, resize/DPI, clipboard/IME,
  event-loop, close/crash and platform evidence before adoption.
- Launch lifecycle and errors must be explicit; changing the viewed document must
  not unexpectedly terminate a prototype. Reuse #029 toolbar and #021 source links.

## Ownership, dependencies and acceptance

SDUI owns parsing/normalization/structure + text projections; SDPTool exposes the
bundle, provenance and action/readiness contract; XFMD displays it and dispatches
explicit actions. Do not add an SDUI parser to XFMD. Generate on demand and retain
source revisions under #025's lifecycle requirements.

External producer card: KB-SDUI-005 in SDP-vNow, with KB-SDUI-003 (capabilities)
and KB-SDUI-004 (text fidelity) retained independently. Local #017 remains XFMD's
modeling work, #015 direct .design editing/preview, and #019 shared rendering.

Acceptance: combined page from real SDUI, multiple/reused frames and hidden nodes,
source spans surviving normalization, text comparison with existing exporter,
missing runtime/bindings, explicit launch and process failure/lifecycle. Inspect
actual XFMD preview/PDF and selected Fyne host; no parity or embedding claim from
static exports. Source IDs must use #021 exact-source navigation.

[Session 0003](../../Sessions/session-%230003--SDL-navigation-and-SDUI.md). Planning/capture only; no implementation claimed.

## Session 0004 selection — 2026-10-07

Owner selects SDUI now. Clarification T001 includes SDUI composition diagrams
now; only SDL viewpoint diagrams move to a later Session. Execute
[PLAN-XFMD-0010](../../05--Implementation/SDUI/Plan.md), starting with SDUI source
access and preserved text preview. Combined composition/text remains required in this Session as U1b. Launch readiness and
#029 toolbar integration follow under the selected plan. Owner: Codex for this
bounded implementation; completion requires the plan's native evidence and an
explicit disposition of deferred diagram scope.

[Session 0004](../../Sessions/session-%230004--SDUI-workflow.md) owns the new route.

U1 outcome: SDUI source/frame View source implemented and verified by 3/3 tests,
including the real-producer native workflow. [Evidence](../../05--Implementation/SDUI/evidence/U1-001/README.md).
U1b combined composition/text remains next; launch and toolbar are not delivered.

## Session 0004 T002 delivered — XFMD 0.7

Combined SDUI composition mindmaps/text, exact revision-checked source spans,
readiness and explicit separate-window Fyne launch are implemented and installed.
Four toolbar groups remain visible in SDP mode; unavailable actions have reasons.
89/89 tests and native/PDF/Fyne inspection passed; see
[U4-001](../../05--Implementation/SDUI/evidence/U4-001/README.md).
No embedded host, SDL execution or independent approval claimed.
Card is gate-review for owner/independent disposition of the concrete installed result. No further SDUI implementation is presently selected.

## Session0004 T003 — historical widget fixtures in the SDUI tab

**Current disposition:** T005 below withdraws the proposed discovery exclusion;
the owner explicitly permits XM-M2 in the normal model overview.

Owner asks whether XM-M2 indicates missing XFMD widgets or an obsolete BoxUI
concept, with a screenshot showing invalid SDUI sources from its fixtures.
Read-only code/history analysis plus live `gh sdp . discover --json` confirms:

- XM-M2 is the 2026-09-29 modeling architecture/capability baseline. Its 21 positive
  and negative probes intentionally include unsupported widgets/properties. It is
  historical evidence, not an unfinished XFMD widget implementation. Preserve it;
  `render_pilot.py` and later XM-M4 evidence reference its identities/hashes.
- Current SDUI parser schema still has button/input/svg only, plus composition
  and Markdown. checked/icon/onContext and the ten probed widget kinds are outside
  that profile. Native Fyne host maps button/input to its controls. Adding useful
  controls belongs to SDUI parser/profile, layout, runtime, presentation and host
  together, not merely allowing a spelling in the parser or changing XFMD.
- The producer gap study completed research on 2026-09-29. Widget/capability
  implementation was handed to upstream KB-SDUI-003, which remains backlog.
  Its selected-next-work recommendation is capability/activation design followed
  by a bounded collection/scrolling pilot; full XFMD UI parity was explicitly
  removed as a requirement. XFMD0.7 implements preview/readiness/launch, not this
  separate language/widget backlog or general SDL callback execution.
- Current discovery walks the SDP area and promotes .design/.sdui files, including
  evidence fixtures, into the source catalog. Live discovery reproduces the
  screenshot's invalid diagnostic rows. The immediate navigation issue is source
  classification: historical negative fixtures appear alongside project models.
  Hiding every invalid source would also hide real authoring errors and is not
  the proposed remedy.

Follow-up proposal, not implemented: let producer discovery distinguish authored
model sources from evidence/test/generated artifacts and supply navigable model
membership or exclusions. XFMD should display that declared scope, preserving
explicit access to evidence files and diagnostics for invalid authored models.
Coordinate this with SDPTool discovery; avoid deleting XM-M2 or hard-coding its
specific folder name in XFMD. This refinement stays open independently of the
installed 0.7 implementation review.

[Live discovery excerpt](../evidence/2026-10-07-widget-discovery/discovery-summary.json).
Producer successor: `/home/warloc/git/SDP-vNow/SDP/KanBan/backlog/#003--SDUI--Proposal--Capabilities-and-navigation-pilot.md`.
No model/source/runtime changes, evidence deletion, installation or new widget
implementation in this turn. The owner's conditional deletion is not applied:
XM-M2 is still referenced modeling evidence, not the obsolete BoxUI runtime.

## Session0004 T005 — widget inventory and retained XM-M2 discovery

Owner asks to expand upstream KB-SDUI-003 with all useful widgets and explicitly
permits XM-M2 in the ordinary model overview. Withdraw T003's proposed exclusion;
no hiding, removal, renaming or source-discovery change is selected. Preserve the
historical probes/results and show genuine diagnostics with the current parser.
The widget work belongs to SDUI and does not reopen XFMD's completed 0.7 scope.

Prepared a complete card update covering tree/list, tabs/splits, menus/dialogs,
multiline input, checkbox/slider/select/numeric input, existing button/input/svg
extensions, icons/checked/context behavior, scrolling and typed activation.
Each family requires applicable parser/runtime/layout/presentation/Fyne evidence.
The exact grammar remains a design decision; full XFMD UI parity is not required.

[Prepared handoff](../evidence/2026-10-07-sdui-widget-scope/README.md) includes a
full updated card, baseline hashes and a patch verified in an isolated directory.
It is **not applied** to SDP-vNow: current host policy permits writes only to the
XFMD repository and /tmp. Apply from an authorized SDP workspace and append the
real review event there using its live ledger predecessor. No upstream event or
implementation is claimed. Card lifecycle remains gate-review for delivered0.7.

## Session0004 T006 — producer handoff applied

Write access restored; applied the unchanged-baseline widget-card patch to
upstream KB-SDUI-003 and recorded EVT-KB-SDUI-000018. It remains backlog; no implementation
selected. The T005 pending-write limitation is resolved. XM-M2 remains visible
as the owner requested. [Handoff status](../evidence/2026-10-07-sdui-widget-scope/handoff.json).
