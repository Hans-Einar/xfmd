# PLAN-XFMD-0001 — Establish XFMD modeling coverage and authority

| Field | Value |
| --- | --- |
| id | PLAN-XFMD-0001 |
| project | XFMD |
| state | active |
| PlanType | RequirementPlan |
| BranchPolicy | current |
| CommitPolicy | milestone |
| ParentPlan | MAINT-XFMD-0004 |
| source | Owner execution authorization 2026-09-29; KB-XFMD-017 |

## Outcome and authority

Account for every current requirement, blueprint, accepted amendment and source responsibility before claiming complete SDL coverage. UI behavior remains relevant; feasible SDUI compositions may use labeled document surfaces.

Parent: [MAINT-XFMD-0004](../../Maintenance/MAINT-XFMD-0004.md).
Work card: [KB-XFMD-017](../../KanBan/active/%23017--Proposal--Adopt-SDP-and-model-XFMD.md).
The owner authorized execution and systematic gap capture. The later
[2026-09-29 scope clarification](../../Maintenance/MAINT-XFMD-0004.md#owner-scope-clarification--2026-09-29)
selects complete SDL functionality, ownership, contract and workflow modeling,
with SDUI composition wherever feasible. Labeled editor/preview areas suffice;
working file editing, rich rendering and native bindings are not required.
Preserve prior evidence; producer widget proposals are not acceptance gates.

## Scope and invariants

UR/SR/UC/AT identities retain their meaning. Existing code is evidence of implementation, not authority to weaken a requirement. BoxUI reservations and future xfw integration remain distinct from current features.

Baseline is XFMD HEAD `41fa494` plus the identified working-tree artifacts;
producer probes use tracked commit `3b4cff695ecab36a0e897052e3f096bbeb9f3ea0`.
Preserve concurrent installation/backlog edits, existing IDs and historical evidence.
Use the parent's gap register; do not duplicate or silently close its findings.

## Milestones

| Milestone | Assignment | Acceptance | Current result |
| --- | --- | --- | --- |
| REQ-M1 | Inventory identities and source files | Reproducible inventory captures all current blueprints and source files with hashes; amendments and reservations remain visible. | Inventory delivered; semantic review active |
| REQ-M2 | Reconcile meaning and coverage | Review every requirement/amendment and the source ownership queue; map governing requirement → blueprint/owner → model target → evidence or specific gap. | Family/amendment map delivered; detailed review active |
| REQ-M3 | Accept the modeling baseline | No current feature or required behavior is silently omitted; unresolved language gaps and their effect on #014 are explicit. | Planned |

## Evidence and completion

[Baseline](Baseline.md), [Inventory](Inventory.md), [Gaps](Gaps.md) and [machine-readable inventory](inventory.json). Current generation finds 37 blueprints, 156 referenced identities and 281 source/manifest files. Counts are not semantic completion.

Close only after the semantic/amendment review is complete. XM-M1 remains open while ownership and evidence mappings are pending.

## Git policy and handoff

Use the current `sprint/009/phase/057-favorites` checkout for this documentation/
modeling assignment, with milestone commits. This does not reopen the delivered
Favorites product sprint. Preserve unrelated working-tree changes; commit only
reviewed assignment changes and intentionally reconciled dependencies. Push,
merge, release and installation remain outside this assignment.

Record governing decisions, candidate identity, changed files, checks, gaps and
next permitted milestone before handoff. An active plan is not a completed model.

## Current checkpoint — 2026-09-29 continuation

REQ-M2: [Coverage](Coverage.md) assigns all 71 UR/SR definitions, reconciles amendments and source families. Seventeen blueprint identities have partial model links. Whole-model and acceptance reconciliation remains active.

## Current checkpoint — 2026-09-30, full structural inventory

The latest owner refinement requests complete SDL modeling and SDUI where feasible;
labeled editor/preview regions suffice. The current
[model](../../SDL/XFMD/README.md) includes all 37 existing blueprints, 10 current
use cases and 22 declared scenarios. Five SDUI concepts illustrate native workspace,
preferences and publication without live document actions. Source grouping and
abstraction limits are in [Model-Semantics](../../04--Design/XFMD-Modeling/Model-Semantics.md).

[XM-M4 evidence](../../05--Implementation/XFMD-Modeling/evidence/XM-M4/README.md)
records parser/profile checks, authority-based mapping checks, deterministic
exports and static visual inspection. Full normative/behavioral coverage and
independent review remain open; no overall plan completion or native test claim.
The next bounded work is reviewing remaining source/test semantics against this
baseline, including NAV-05/NAV-10 regression evidence and unresolved SDL bindings.
