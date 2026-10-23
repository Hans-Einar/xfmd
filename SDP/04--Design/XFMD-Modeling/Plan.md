# PLAN-XFMD-0003 — Specify the first complete XFMD modeling workflow

| Field | Value |
| --- | --- |
| id | PLAN-XFMD-0003 |
| project | XFMD |
| state | active |
| PlanType | DesignPlan |
| BranchPolicy | current |
| CommitPolicy | milestone |
| ParentPlan | MAINT-XFMD-0004 |
| source | Owner execution authorization 2026-09-29; KB-XFMD-017 |

## Outcome and authority

Design requirement-to-SDL traceability, collaboration and state/failure contracts using the architecture baseline, then generalize the proven pattern across the inventory. Add feasible SDUI composition with explicitly illustrative editor/preview regions.

Parent: [MAINT-XFMD-0004](../../Maintenance/MAINT-XFMD-0004.md).
Work card: [KB-XFMD-017](../../KanBan/active/%23017--Proposal--Adopt-SDP-and-model-XFMD.md).
The owner authorized execution and systematic gap capture. The later
[2026-09-29 scope clarification](../../Maintenance/MAINT-XFMD-0004.md#owner-scope-clarification--2026-09-29)
selects complete SDL functionality, ownership, contract and workflow modeling,
with SDUI composition wherever feasible. Labeled editor/preview areas suffice;
working file editing, rich rendering and native bindings are not required.
Preserve prior evidence; producer widget proposals are not acceptance gates.

## Scope and invariants

The pilot follows Files/favorites opening, editing and generated navigation. Include dirty admission, cancellation/failure and pane replacement; static layout alone is insufficient.

Baseline is XFMD HEAD `41fa494` plus the identified working-tree artifacts;
producer probes use tracked commit `3b4cff695ecab36a0e897052e3f096bbeb9f3ea0`.
Preserve concurrent installation/backlog edits, existing IDs and historical evidence.
Use the parent's gap register; do not duplicate or silently close its findings.

## Milestones

| Milestone | Assignment | Acceptance | Current result |
| --- | --- | --- | --- |
| DES-M1 | Specify the representative workflow | Describe state owners, calls, contracts, inputs/outputs and failure/recovery against FUNC-001/002/005/007/008/010/012/013/031 and current amendments. | Workflow contract source-traced; evidence review open |
| DES-M2 | Define SDL artifacts and contract mappings | Name actual source owners, entry points, symbol-to-existing-ID mapping, collaboration and state/failure contracts; mark language limitations using gap IDs. | Partial structural sources delivered; SDUI concept states included without native interaction |
| DES-M3 | Review reusable design pattern | Validate the pilot with selected tools and compare it with current XFMD behavior before expanding to other flows. | Static export checked; behavioral fidelity review pending |

## Evidence and completion

[ArchitecturePlan](../../03--Architecture/XFMD-Modeling/Plan.md), [baseline](../../02--Requirements/XFMD-Modeling/Baseline.md), [gaps and SDUI proposals](../../02--Requirements/XFMD-Modeling/Gaps.md). No production UI change is selected.

Exit requires a source-traced SDL workflow with explicit limitations and reviewable ownership/dependency boundaries. Source placement is SDP/SDL/XFMD; plans/evidence remain in numbered phases. No generated viewpoint is hand-maintained. Feasible SDUI exports must distinguish illustrative regions from working controls; earlier exports retain historical scope.

## Git policy and handoff

Use the current `sprint/009/phase/057-favorites` checkout for this documentation/
modeling assignment, with milestone commits. This does not reopen the delivered
Favorites product sprint. Preserve unrelated working-tree changes; commit only
reviewed assignment changes and intentionally reconciled dependencies. Push,
merge, release and installation remain outside this assignment.

Record governing decisions, candidate identity, changed files, checks, gaps and
next permitted milestone before handoff. An active plan is not a completed model.

## Current checkpoint — 2026-09-29 continuation

DES-M1/M2: [Navigation workflow](Navigation-Workflow.md) traces NAV-01–12 and links the bounded SDL/SDUI sources. Three static states are generated; executable behavior and complete fidelity remain open.

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
