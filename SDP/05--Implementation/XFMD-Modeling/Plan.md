# PLAN-XFMD-0004 — Author and integrate the XFMD model baseline

| Field | Value |
| --- | --- |
| id | PLAN-XFMD-0004 |
| project | XFMD |
| state | active |
| PlanType | ImplementationPlan |
| BranchPolicy | current |
| CommitPolicy | milestone |
| ParentPlan | MAINT-XFMD-0004 |
| source | Owner execution authorization 2026-09-29; KB-XFMD-017 |

## Outcome and authority

Implement the approved modeling design and reproducible generation/registration workflow in bounded deliveries while preserving product behavior.

Parent: [MAINT-XFMD-0004](../../Maintenance/MAINT-XFMD-0004.md).
Work card: [KB-XFMD-017](../../KanBan/active/%23017--Proposal--Adopt-SDP-and-model-XFMD.md).
The owner authorized execution and systematic gap capture. The later
[2026-09-29 scope clarification](../../Maintenance/MAINT-XFMD-0004.md#owner-scope-clarification--2026-09-29)
selects complete SDL functionality, ownership, contract and workflow modeling,
with SDUI composition wherever feasible. Labeled editor/preview areas suffice;
working file editing, rich rendering and native bindings are not required.
Preserve prior evidence; producer widget proposals are not acceptance gates.

## Scope and invariants

Changes are model sources, generation/check integration and reconciled maintained documentation. No native SDP tab, SDL parser extension, SDUI host rewrite, producer edit or installation is included.

Baseline is XFMD HEAD `41fa494` plus the identified working-tree artifacts;
producer probes use tracked commit `3b4cff695ecab36a0e897052e3f096bbeb9f3ea0`.
Preserve concurrent installation/backlog edits, existing IDs and historical evidence.
Use the parent's gap register; do not duplicate or silently close its findings.

## Milestones

| Milestone | Assignment | Acceptance | Current result |
| --- | --- | --- | --- |
| IMP-M1 | Implement the vertical model | Author supported SDL for the designed workflow, map unsupported behavior explicitly, generate views using tools and retain source/tool provenance. | Partial structural pilot delivered; ordered behavior open; SDUI concepts without live editor/preview requirement |
| IMP-M2 | Extend coverage by responsibility | Cover documents/navigation, renderer/media/diagrams/output, workspace/preferences and cross-cutting lifecycles; update inventory mappings and gaps at each milestone. | Planned |
| IMP-M3 | Integrate process and navigation documentation | Reconcile stale board/version/source pointers and maintained English design content; register supported model entries and reproduce navigation outputs. | Planned |

## Evidence and completion

[RequirementPlan](../../02--Requirements/XFMD-Modeling/Plan.md), [ArchitecturePlan](../../03--Architecture/XFMD-Modeling/Plan.md), [DesignPlan](../../04--Design/XFMD-Modeling/Plan.md), [VerificationPlan](VerificationPlan.md). Tool probes are preparatory evidence, not IMP-M1 completion.

Do not expand the SDL pattern until its input and generation approach works. SDUI execution is not a prerequisite. Every generated output claimed as current evidence must be reproducible; unsupported facts retain explicit gaps. Close only on integrated evidence, not an inventory count or successful parser exit.

## Git policy and handoff

Use the current `sprint/009/phase/057-favorites` checkout for this documentation/
modeling assignment, with milestone commits. This does not reopen the delivered
Favorites product sprint. Preserve unrelated working-tree changes; commit only
reviewed assignment changes and intentionally reconciled dependencies. Push,
merge, release and installation remain outside this assignment.

Record governing decisions, candidate identity, changed files, checks, gaps and
next permitted milestone before handoff. An active plan is not a completed model.

## Current checkpoint — 2026-09-29 continuation

IMP-M1: [Model sources](../../SDL/XFMD/README.md), generation script and 52 reproducible artifacts deliver a partial structural/static pilot. Expansion remains planned; this is not whole-workflow runtime implementation.

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

## 2026-10-01 — Session-led native integration delta

[SN-001](../SDP-Navigation/evidence/SN-001/README.md) records the FUNC-032/UC-012 structural delta and sixth SDUI
concept alongside native implementation under PLAN-XFMD-0006. Current generated
views/inventory are regenerated; historical XM-M2/XM-M4 evidence remains intact.
This does not close this plan's earlier semantic, native-evidence or independent
review obligations. Continue with SESSION-XFMD-0001 for the new feature.
