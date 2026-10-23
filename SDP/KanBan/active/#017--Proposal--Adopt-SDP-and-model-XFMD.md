# KB-XFMD-017 — Adopt SDP and model XFMD functionality in SDL

| Field | Value |
| --- | --- |
| id | KB-XFMD-017 |
| project | XFMD |
| type | Proposal |
| CardState | in-progress |
| created | 2026-09-25T11:02:19Z |
| source | owner-conversation-2026-09-25 |
| next_review | At navigation behavior/evidence review and next coverage expansion |
| PlanId | MAINT-XFMD-0004 |

## Current direction — 2026-09-29, latest owner refinement

The owner selects a complete SDL model of existing XFMD, plus SDUI wherever
supported composition is useful. Editor and Markdown viewer may be labeled
regions; actual loading/editing/saving/rendering is not required. A basic multiline
input is a useful producer proposal, not a prerequisite or selected producer edit.

[MAINT-XFMD-0004](../../Maintenance/MAINT-XFMD-0004.md#owner-scope-clarification--2026-09-29)
coordinates all five active child plans. Model functional ownership, contracts,
dependencies and workflow/failure behavior to keep #014/#015 extension maintainable.
The latest instruction refines the preceding removal of all required SDUI work;
advanced native parity remains excluded. CardState remains in-progress.

Current execution: extend the structural model to all 37 existing blueprints,
encode navigation example paths with supported channels/scenarios, and add honest
SDUI editor/preview composition. Do not implement #014/#015 or a new widget here.

## Historical direction — 2026-09-28, superseded in SDUI scope

The owner requests a complete SDL **and SDUI** description of XFMD before
proceeding with KB-XFMD-014. The overarching
[MaintenancePlan](../../Maintenance/MAINT-XFMD-0004.md) is a draft for joint
review. It defines proposed RequirementPlan, ArchitecturePlan, DesignPlan,
ImplementationPlan and VerificationPlan responsibilities; these subordinate
plans have not been created and execution has not started. CardState remains
backlog.

The installed System-owned source convention supersedes the original proposed
placement in numbered phases below: authored models belong under
`SDP/SDL/XFMD/`, including its owning UI container's SDUI sources. Process plans,
rationale and derived views remain in the numbered phases. See the
[current source guide](../../SDL/README.md). Current language/input capabilities
must be probed rather than inferred from either this dated proposal or the
producer's experimental whole-system examples.

## Owner assignment and starting point

The owner requests that the next XFMD agent take the new [SDP area](../../README.md)
into real use and create XFMD's .design sources. SDP1-M1 establishes directories
and board compatibility only. Read AGENTS.md, SDP/README.md, the phase entries,
requirements, architecture, working method, blueprint registry and current Git
status before selection. Preserve unrelated work and current product behavior.

## Original scope and proposed milestones — 2026-09-25

1. Inventory requirements, use cases, features/functionality, blueprints, contracts,
   code owners, sprint plans and evidence. Produce an authority/migration map with
   stable existing IDs; separate current facts, dated proposals and unknowns.
2. Establish the mandate/stakeholder/actor context and requirement/feature links.
   Model XFMD as its own system. Identify real containers and external tool
   contracts; application/interpreter/renderer are source layers unless a runtime
   boundary is actually present. SDL, SDUI and SDPTool are separate cooperating
   systems, not owned XFMD implementations.
3. Author readable .design sources in the numbered phases: requirements/intent,
   architecture topology, per-container detail and implementation relationships.
   Keep shared contracts once and preserve UR/SR/AT/FEAT/FUNC references. If current
   SDL cannot encode a fact, retain a clearly marked Markdown design requirement;
   do not invent accepted System/import/Stakeholder syntax. Design-core 0.5 does
   not implement System; multi-file input is currently an unfinished SDP workstream.
4. Run the Go SDL toolkit to validate/format the model and generate relevant
   viewpoints. Start with context/actors-usecases, feature contributions,
   containers/channels, detailed collaboration and delivery/evidence views where
   supported by explicit facts. Generated Markdown/Mermaid/SVG must come from tools,
   not hand-written substitute viewpoints. Record commands and exact versions.
5. Reconcile maintained docs and process instructions, source destinations and
   traceability. Make one source authoritative per fact; preserve dated evidence.
   Capture outstanding language/tool gaps in the responsible board. Commit bounded
   milestones on phase branches following the existing XFMD working method.

## Acceptance

- All five phase entries point to substantive, current design or explicitly
  documented gaps; no empty claims that migration is complete.
- Existing identities, requirements, blueprints, evidence and ownership remain
  traceable; implementation status is supported by evidence, not inferred from
  parser success or diagrams.
- Design inputs parse with the identified supported Go SDL profile. Generated
  viewpoints have source provenance; current links and KanBan replay pass.
- Design detail follows actual container ownership without duplicated shared
  library/contract definitions. A0–A5 is not inferred from folder numbers.
- Remaining decisions and unsupported SDL constructs have an explicit home.

## Dependencies and exclusions

[KB-XFMD-014](../active/%23014--Proposal--SDP-sidebar-and-generated-navigation.md) owns the
native SDP sidebar. [KB-XFMD-015](../backlog/%23015--Proposal--SDL-design-file-preview.md)
owns direct .design preview. Neither feature is delivered by this adoption card.
[KB-XFMD-012](../backlog/%23012--Ref--SDP--014--KanBan-format-sync.md) retains future KanBan
release alignment. FOX remains XFMD's GUI toolkit; do not switch it to Fyne.
Do not implement SDL/SDUI parsers in XFMD or edit the producer repositories from
this task without separate selection. Existing pipeline guidance:

file:///home/warloc/git/SDP-vNow/Toolkit/SDPTool/Navigation-and-Design-Preview.md

## Worklog

- Registered on 2026-09-25 for another XFMD agent. CardState remains backlog.
  No XFMD .design files or generated model viewpoints have been delivered yet.

## Released process installation — 2026-09-27

SDP 0.2.0 is installed through released gh-sdp. MAINT-XFMD-0001 records the signed
transaction; MAINT-XFMD-0002 reconciles the local board helper and instructions.
The remaining work on this card is the actual authority/modeling adoption above;
this installation does not claim generated SDL models or native UI integration.

2026-09-27T08:17:07.642945Z: EVT-KB-XFMD-000077. Installation recorded; CardState remains backlog for SDL modeling.

## Planning review — 2026-09-28

MAINT-XFMD-0003 subsequently records Toolkit 1.0.0 installation. The draft
MAINT-XFMD-0004 now coordinates the remaining modeling work and the owner's SDUI
scope addition. Historical installation and milestone notes above are preserved;
the new plan records current source placement and proposes explicit coverage and
evidence checkpoints. Draft registration is not execution or model completion.

2026-09-28T21:59:39.147846Z: EVT-KB-XFMD-000079. Linked planned MAINT-XFMD-0004; CardState remains backlog.

## Execution selected — 2026-09-29

The owner authorized MAINT-XFMD-0004 and requested an explicit SDL/SDUI gap
register with extension proposals. Current assignment: inventory and capability
baseline (XM-M1/XM-M2), coordinated by Codex. Completion criteria are in the
MaintenancePlan; the full card remains open until modeling and verification
closeout. Earlier draft-only notes retain their dated scope.

## Current findings and handoff — 2026-09-29

[Baseline](../../02--Requirements/XFMD-Modeling/Baseline.md),
[inventory](../../02--Requirements/XFMD-Modeling/Inventory.md),
[gap register](../../02--Requirements/XFMD-Modeling/Gaps.md) and
[probe evidence](../../05--Implementation/XFMD-Modeling/evidence/XM-M2/README.md)
are now available. Five child plans are linked from the MaintenancePlan.
The 18 gap entries distinguish observed profile limitations, documentation work,
unverified behavior and proposed SDUI extensions. Producer changes remain outside
this assignment; no producer card or implementation was changed.

Next: semantic source/requirement reconciliation and the architecture/profile
handoff, then the representative model. CardState stays in-progress; inventory
and successful boundary probes do not complete the full-model assignment.

## Navigation pilot — 2026-09-29 continuation

[Coverage reconciliation](../../02--Requirements/XFMD-Modeling/Coverage.md),
[model sources](../../SDL/XFMD/README.md) and
[XM-M3 evidence](../../05--Implementation/XFMD-Modeling/evidence/XM-M3/README.md)
now supersede the preceding no-model checkpoint. The pilot maps 17 blueprint
identities and three static shell states. All 52 generated artifact hashes
reproduce. Twenty gaps remain registered, including new typography and transaction
scenario evidence findings. Whole-model coverage, bindings/native behavior and
independent review remain open; CardState stays in-progress.

## Scope review recorded — 2026-09-29

EVT-KB-XFMD-000083 records the owner clarification above and the revised
MaintenancePlan/child-plan acceptance. CardState remains in-progress. Existing
SDUI artifacts are retained; no new SDUI or native feature work is selected.

## Whole-inventory model checkpoint — 2026-09-30

[Current model](../../SDL/XFMD/README.md) and
[XM-M4 evidence](../../05--Implementation/XFMD-Modeling/evidence/XM-M4/README.md)
now provide 37 blueprint mappings, 10 current use cases, 22 declared examples,
five feasible SDUI concepts and 243 reproducible exports. This follows the latest
owner refinement: complete SDL is the goal, and editor/viewer placeholders are
sufficient for SDUI. Full semantic/native evidence and independent review remain
open; CardState stays in-progress. EVT-KB-XFMD-000085 records this review.

## Continuing navigation Session — 2026-10-01

[SESSION-XFMD-0001](../../Sessions/session-%230001--SDP-project-navigation.md)
links this work to native SDP navigation. See
[PLAN-XFMD-0006](../../05--Implementation/SDP-Navigation/Plan.md) for the selected
increments. Existing unfinished scope and evidence remain open.

The T001 checkpoint also reconciles FUNC-032/UC-012: 38 blueprint identities,
11 current use cases, 73 requirement IDs, six SDUI concepts and 248 generated
outputs. Existing 22 declared scenarios remain; they do not model every new SDP
request race. See SN-001 under PLAN-XFMD-0006 for current evidence and limits.
