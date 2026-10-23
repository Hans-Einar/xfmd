# PLAN-XFMD-0002 — Define SDL responsibility and dependency boundaries

| Field | Value |
| --- | --- |
| id | PLAN-XFMD-0002 |
| project | XFMD |
| state | active |
| PlanType | ArchitecturePlan |
| BranchPolicy | current |
| CommitPolicy | milestone |
| ParentPlan | MAINT-XFMD-0004 |
| source | Owner execution authorization 2026-09-29; KB-XFMD-017 |

## Outcome and authority

Choose the smallest supported SDL modeling strategy that exposes XFMD responsibility, contract and dependency boundaries before feature expansion. Make unsupported System/source composition explicit; SDUI uses supported conceptual compositions without native parity.

Parent: [MAINT-XFMD-0004](../../Maintenance/MAINT-XFMD-0004.md).
Work card: [KB-XFMD-017](../../KanBan/active/%23017--Proposal--Adopt-SDP-and-model-XFMD.md).
The owner authorized execution and systematic gap capture. The later
[2026-09-29 scope clarification](../../Maintenance/MAINT-XFMD-0004.md#owner-scope-clarification--2026-09-29)
selects complete SDL functionality, ownership, contract and workflow modeling,
with SDUI composition wherever feasible. Labeled editor/preview areas suffice;
working file editing, rich rendering and native bindings are not required.
Preserve prior evidence; producer widget proposals are not acceptance gates.

## Scope and invariants

FOX owns the XFMD application window. Application/interpreter/renderer/contracts are source responsibilities. SDL, SDUI and SDPTool are external collaborators; Fyne is SDUI's separate host.

Baseline is XFMD HEAD `41fa494` plus the identified working-tree artifacts;
producer probes use tracked commit `3b4cff695ecab36a0e897052e3f096bbeb9f3ea0`.
Preserve concurrent installation/backlog edits, existing IDs and historical evidence.
Use the parent's gap register; do not duplicate or silently close its findings.

## Milestones

| Milestone | Assignment | Acceptance | Current result |
| --- | --- | --- | --- |
| ARC-M1 | Measure producer capabilities | Pin producer/tool identity and execute positive and negative SDL/SDUI probes; separate parser, layout, runtime and host claims. | 21 probes delivered |
| ARC-M2 | Resolve model organization | Document actual runtime/source boundaries, profile/input selection, cross-file identity strategy and alternatives; link all remaining gaps. | Pilot decisions delivered; full composition open |
| ARC-M3 | Hand off the model architecture | DesignPlan can trace the representative workflow to explicit responsibility owners and dependency boundaries; future #014/#015 designs can identify architectural changes without invented grammar, duplicate ownership or a toolkit migration. | Pilot handoff delivered; whole-model handoff pending |

## Evidence and completion

[Capability evidence](../../05--Implementation/XFMD-Modeling/evidence/XM-M2/README.md) and [gap register](../../02--Requirements/XFMD-Modeling/Gaps.md). The producer archive excludes untracked sourceinput work; the profile is not upgraded.

ARC-M2 compares supported bounded models plus explicit gaps with waiting for producer extensions and with an experimental model. No full-model exception is owner-approved. Prefer supported inputs for the first probe/model; do not promote experimental declarations into accepted syntax.

## Git policy and handoff

Use the current `sprint/009/phase/057-favorites` checkout for this documentation/
modeling assignment, with milestone commits. This does not reopen the delivered
Favorites product sprint. Preserve unrelated working-tree changes; commit only
reviewed assignment changes and intentionally reconciled dependencies. Push,
merge, release and installation remain outside this assignment.

Record governing decisions, candidate identity, changed files, checks, gaps and
next permitted milestone before handoff. An active plan is not a completed model.

## Current checkpoint — 2026-09-29 continuation

ARC-M2: [Decisions](Decisions.md) select the supported bounded pilot and document runtime/source boundaries. The pilot handoff is delivered; full System/source-set and behavioral profile decisions remain open.

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
