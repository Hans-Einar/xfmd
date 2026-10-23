# Working Sessions — manual format 1

A Session follows one goal across turns, cards, plans and replacement agents.
Use session-#NNNN--Topic.md, unique within this project, copied from
[Session-template.md](Session-template.md). Start from a primary card and link
back from every affected card. Small factual answers do not require a Session.
Read this guide when resuming a goal that spans turns or multiple plans.

## Authority and routine

The Session owns the goal, roadmap, route changes and turn summaries. Cards own
CardState; plans own phases, milestones, Git policy and acceptance. Session tables
are dated projections, not a new lifecycle authority. Use existing
[KanBan](../KanBan/README.md) and [project management and planning](../ProjectManagement/README.md).
Do not introduce a Session ledger kind or duplicate lifecycle ledger.

Before each turn, recover the goal/current step, check request scope and applicable
procedures, and explain a needed route change. During work, link new cards/plans
and record discoveries. At return, update the step states and evidence and state
what is next. A completed turn does not complete a step, card or goal.

## Roadmap first

Place the roadmap immediately after the title: a Gantt followed by a step table.
Show planned, next, on-going, waiting, blocked, completed, canceled or superseded
with text labels as well as colors. Normally one next step is selected; explicit
parallel work can have separate lanes. Keep dependencies, authority and evidence.
When timing is unknown, label synthetic Gantt dates **sequence only**. Do not use
axisFormat, todayMarker or init directives in the conservative XFMD example.
The manual Gantt and table must agree. Automatic event/turn timelines are not
implemented by this format.

## Cards, plans and history

Record card links with initial, planned-final, current and actual-final states.
Plans retain their detailed milestones; link instead of copying them. Document
readiness (planned/planning/ready/on-going/completed) is separate from canonical
plan lifecycle. Preserve completed steps and previous direction with dated reasons.
If a table is stale, reconcile against the actual card/plan; never change a card
only to make the projection agree.

## Turn capture

Record local turn identifiers, owner inputs, assistant outcome, decisions, relevant
procedure/skill use and evidence. Distinguish exact observed quotations from manual
summaries. Unknown host IDs, attachments and timing remain unknown. Do not invent
an eventual final response before it has been sent. Record visible messages only,
never private reasoning or credentials. No automatic app-server transcript capture,
MCP observer or enforced routine engine is supplied by this format.

## Closeout and upgrades

Close only when goal evidence exists and every included card/plan/step has an
explicit disposition. Remaining work names a successor; do not claim it completed.
A replacement thread continues the same durable Session. This template and guide
initialize only when missing. Upgrades preserve project-authored Sessions, guides
and templates; no SDP repository conversations or Session 0001 are installed.
The receipt advertises sdp.sessions.manual.v1 when provided by the release.

## Current project focus — 2026-10-19

[Session0006](session-%230006--Discoverable-SDPTool-actions.md) is paused for KB030
until the PLAN-SDP-0024 JSON API is implemented, integrated into main and released.
S1 will verify the actual released contract on resumption. [Session0005](session-%230005--SDL-viewpoint-diagrams.md)
closed its selected implementation round at T021: native viewpoint/source work is
verified and producer commit fde2239 follows ecd9dcd on the main-based successor.
The [residual disposition](../04--Design/SDL-Navigation/Viewpoint-Disposition.md)
keeps large-print acceptance, remaining aggregate proposals and release integration
explicitly open in their existing cards/plan. Installed tools are unchanged.

[Session0007](session-%230007--Architecture-viewpoint.md) records completed bounded
rounds for VP02 (cdf4437) and VP03 (63da7ae), verified and pushed separately.
N3j/N4 is on main 5ca0e90. T003 completes VP04/N7 (85d62f2), with producer and
native verification in N7-001. T004 completes VP05/N8 (3b3d935), verified with
mode isolation and explicit empty states. T005 completes VP06/N9 (76fc87f),
with separate activity graphs/status registers and actual empty-plan verification.
T006 completes VP08/N10 (d677d70): channel partitions, source registers and
40 actual native/PDF diagrams verified. Whole-project discovery guard is handed
to KB025; original union routing failure remains KB020. N5–N10 await selected
integration/release. T007 completes VP09/N11 (6b80b40), with data/storage and
contract inventories verified through producer/native/PDF. N5–N11 await integration;
T008 completes the VP10/N12 projection (f1c3342), with two fixture packets,
source registers and actual empty state verified. Narrow packet labels/boundary
numbers remain a KB020 renderer defect. N5–N12 await integration; VP11 properties/
traceability/gaps follows in T009/N13 (486b29d): all facts/declarations, property
claims and existing model gaps, with complete actual source/PDF and native property
selection evidence. All catalog overview candidates now exist; N5–N13 await selected
integration. Next: review integration readiness and retained KB020/KB025/KB032 work.
VP07 is already delivered.
Session0006 remains held for the released API.

Session0007 T010 completes N14: KB020 packet readability corrected in renderer
49bb0ee, adopted and built locally. Seven focused tests and 24 CTests pass with
inspected native/PDF evidence. Integration forecast for N5–N13 against main/PG
is clean at the recorded heads; no merge or installation. Session remains
on-going; next proposed scope is KB025 whole-project discovery policy. See N14-001
for the existing whole-project SDP validator compatibility limitation.

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
