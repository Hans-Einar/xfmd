# PLAN-XFMD-0010 — SDUI browsing and explicit prototype launch

| Field | Value |
| --- | --- |
| id | PLAN-XFMD-0010 |
| project | XFMD |
| state | active |
| PlanType | ImplementationPlan |
| BranchPolicy | current |
| CommitPolicy | milestone |
| Source cards | KB-XFMD-027; KB-XFMD-029 (toolbar dependency) |
| Session | SESSION-XFMD-0004 |

## Outcome, authority and boundaries

Owner 2026-10-07 requests a new SDUI Session, followed by a separate diagram
Session. Start SDUI browsing and prototype launch now. Owner clarification includes
SDUI composition diagrams alongside the text preview in this Session; only SDL
viewpoint diagrams move to the later Session. Do not close the combined card on
text-only delivery. Authored SDL/SDUI model sources remain held under #022.

XFMD remains FOX. SDUI owns parsing, normalization and runtime; SDPTool owns
catalog identities and generated preview bundles. Reuse the existing on-demand
sdui-preview exporter. A static text preview is not a running UI or SDL execution.

## Selected design

U1 extends the existing source context menu to catalog entries of kind sdui with
.sdui paths, alongside kind sdl with .design paths. Match complete source IDs,
including target-free invalid source rows, and inherit only unambiguous context.
Revalidate project, revision, expected path, containment and regular-file status
at opening. Open through the ordinary dirty-buffer transaction in Editor mode.
Do not apply SDL phase/abstraction flattening to SDUI source trees. U1 has no line-span claim; U1b adds exact SDUI source links. Broader SDL source references remain #021 work.

U1b will extend the producer exporter rather than construct SDUI structure in
XFMD. Preserve the existing text renderer and add a clearly labelled source
composition section, using parser declaration/instance provenance before hidden
content filtering. Select a supported grouped frame/region/row representation
with real fixtures and inspected XFMD output; do not infer layout from text dump
whitespace. KB-SDUI-005 owns the producer increment under this plan’s bounded external work unit.

U2 first selects a producer readiness/launch contract: installed host, explicit
entry, prototype versus SDL-connected bindings, validation errors and revision.
The inspected standalone Fyne CLI accepts -entry FRAME -watch=true SOURCE. It
has no JSON readiness endpoint; symbolic SDL callbacks remain unbound. Do not
infer runnable completeness from discovery's validated syntax status. Host is
not currently on PATH. Separate Fyne window is the initial proposal; embedding
requires a separate tested design. No implicit execution on preview selection.

## Milestones and acceptance

| Milestone | Acceptance | State |
| --- | --- | --- |
| U1 | SDUI source/frame View source, valid and invalid entries, dirty cancel/discard, stale/escaping targets rejected; existing text preview and SDL projection preserved | completed; U1-001 |
| U1b | Producer-owned composition projection joined with existing text preview; hidden/reused/binding fixtures and native inspection | completed; U4-001 |
| U2 | Select/test producer readiness and host discovery; explicit launch with errors and lifecycle independent of document navigation | completed; U4-001 |
| U3 | Integrate contextual SDUI Launch into #029's persistent SDP/KanBan/SDUI/Sessions groups; disabled reasons and native acceptance | completed; U4-001 |
| U4 | Verify/build/install 0.7, reconcile review disposition and SDL diagram successor | completed; U4-001 |

KanBan mutations stay #029's unselected service-contract work. SDL viewpoint work stays in PLAN-XFMD-0009 N3/N4 and #020/#021 for the later Session.
SDUI composition stays here as U1b, coordinated with producer KB-SDUI-005.

## Git and verification

Use current sprint/009/phase/057-favorites branch and milestone commits when
scoped integration can preserve the shared dirty baseline and staged #020.
Record exact changed-source hashes/test logs until then. No release, installation,
publication or version change is selected. Installed XFMD 0.6 remains the baseline.
Run source-association and display unit tests plus real-producer native navigation
for U1, then document/board/layer checks. Same-context checks are not independent
review. Record actual evidence below; proposed tests are not passes.

## U1 delivery — 2026-10-07

[U1-001](evidence/U1-001/README.md): targeted build and 3/3 tests PASS, including
real-producer native source/text workflows and inspected screenshots. Board,
blueprint, implemented-symbol and layer checks pass. No independent review or
installation. U1b combined composition/text is next; host readiness and launch
follow. Exact changed source and test-binary hashes identify the dirty candidate.

## T002 execution and release selection — 2026-10-07

Owner explicitly authorizes completing all remaining SDUI work, building and
installing XFMD 0.7 in this turn. This supersedes the earlier no-install boundary.
Select bundled SDUI-owned preview/check and Fyne binaries in libexec/xfmd;
retain gh sdp discovery and SDL operations. This avoids replacing the owner's
separately managed gh extension or coupling the release to concurrent SDPTool
changes. The facade will also reuse the combined exporter when rebuilt upstream.

Composition uses nested frame/region/row clusters with widget labels, backed by
a provenance table containing instance identity, declaration/use spans, layout
properties and bindings. Source links carry UTF-8 byte offsets into the exact
preview source snapshot; XFMD refuses highlighting after source changes.
Hidden nodes remain in composition, while the existing visible text renderer
retains its semantics. No hand-authored model changes are selected.

Preflight uses the bundled producer check operation and reports local prototype
versus unbound SDL callbacks. Unbound prototypes may launch with explicit labels;
no SDL execution is claimed. Launch rechecks revision, uses argv without a shell,
opens a separate Fyne window, reports failures and survives navigation/closing
XFMD. No embedded bridge is claimed. Toolbar groups remain visible in SDP mode;
KanBan Add/Move stay disabled with a missing-service reason, outside SDUI scope.

### Native composition discovery

U1b visual inspection exposed an existing native group-order assumption: the
layout engine returns independent nested clusters in a different order. Select
a bounded ID-based mapping fix in MermaidDiagramLayout with regression coverage;
this is required for SDUI composition and does not start SDL viewpoint redesign.
Reject unknown/duplicate groups, retain model order in inspection metadata.

### Final composition notation decision

Native visual inspection found overlapping labels in deeply nested flowchart
clusters even after valid group IDs were accepted. Select Mermaid mindmaps for
source composition: one overview per frame/group, explicit region and ordered
row branches, then detail maps for nested containers. Keep all instances in the
provenance table and visible text below. This describes source composition, not
measured UI placement. Shallow maps avoid nesting-depth loss and make reuse and
hidden branches legible. No upstream graph-layout redesign is selected. The
independent ID-order defect fix remains a covered native compatibility correction.

## T002 delivery and review disposition

All implementation milestones and the owner-requested 0.7 installation are
complete; see [U4-001](evidence/U4-001/README.md) for 89/89 tests, real Fyne/PDF
inspection, exact source/package hashes and installation receipt/backup.
Current selected notation is per-container mindmaps (superseding the initial
nested-cluster proposal above). Authored models and gh sdp are unchanged.

Canonical state remains active solely for owner/independent review disposition;
this does not imply unfinished SDUI implementation. KB027 is gate-review. KB029's
SDUI portion is delivered; KanBan mutations remain separate scope. Session0004
closes implementation/install and Session0005 selects the later SDL diagram route.
No independent approval/publication is inferred from passing self-checks. Milestone
commit deferred to avoid collecting the shared dirty/staged baseline; exact source
archives preserve reviewability until scoped integration is selected.
