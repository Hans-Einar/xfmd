# PLAN-XFMD-0005 — Verify XFMD modeling claims at the right level

| Field | Value |
| --- | --- |
| id | PLAN-XFMD-0005 |
| project | XFMD |
| state | active |
| PlanType | VerificationPlan |
| BranchPolicy | current |
| CommitPolicy | milestone |
| ParentPlan | MAINT-XFMD-0004 |
| source | Owner execution authorization 2026-09-29; KB-XFMD-017 |

## Outcome and authority

Define and execute evidence checks for SDL coverage, model semantics, language/tool compatibility, generation and correspondence to actual source/test behavior on the identified candidate. Review responsibility and dependency consistency; SDUI acceptance covers honest composition, not editor/preview feature parity.

Parent: [MAINT-XFMD-0004](../../Maintenance/MAINT-XFMD-0004.md).
Work card: [KB-XFMD-017](../../KanBan/active/%23017--Proposal--Adopt-SDP-and-model-XFMD.md).
The owner authorized execution and systematic gap capture. The later
[2026-09-29 scope clarification](../../Maintenance/MAINT-XFMD-0004.md#owner-scope-clarification--2026-09-29)
selects complete SDL functionality, ownership, contract and workflow modeling,
with SDUI composition wherever feasible. Labeled editor/preview areas suffice;
working file editing, rich rendering and native bindings are not required.
Preserve prior evidence; producer widget proposals are not acceptance gates.

## Scope and invariants

Product acceptance, parser validity, static rendering, binding/runtime behavior, independent review and owner acceptance are different claims. Mark unrun checks not verified.

Baseline is XFMD HEAD `41fa494` plus the identified working-tree artifacts;
producer probes use tracked commit `3b4cff695ecab36a0e897052e3f096bbeb9f3ea0`.
Preserve concurrent installation/backlog edits, existing IDs and historical evidence.
Use the parent's gap register; do not duplicate or silently close its findings.

## Milestones

| Milestone | Assignment | Acceptance | Current result |
| --- | --- | --- | --- |
| VER-M1 | Establish reproducible baseline evidence | Inventory/tool checks carry exact sources, commands, hashes and limitations; negative probes include expected diagnostics. | Initial checks delivered |
| VER-M2 | Verify models and workflow fidelity | Check stable identity mapping, source ownership, dependency direction, scenario/error coverage and regenerated SDL output against actual XFMD contracts, source and tests. Distinguish implementation mismatches from unsupported model facts. | Initial model checks delivered; actual behavior evidence review pending; conceptual SDUI inspection remains separate |
| VER-M3 | Review integrated candidate and handoff | Record independent review, remaining gap dispositions and readiness for separate #014 work; stale evidence is rerun or qualified. | Planned |

## Evidence and completion

[XM-M2 evidence](evidence/XM-M2/README.md), [inventory](../../02--Requirements/XFMD-Modeling/Inventory.md), [gap register](../../02--Requirements/XFMD-Modeling/Gaps.md). 21 capability expectations pass; this validates the boundary, not missing features.

Independent review is outstanding. Host/session rules currently do not authorize proactive sub-agent delegation; self-checks are labeled same-context. Keep this gate open rather than claiming approval. No native GUI test or full-model verification has run.

## Git policy and handoff

Use the current `sprint/009/phase/057-favorites` checkout for this documentation/
modeling assignment, with milestone commits. This does not reopen the delivered
Favorites product sprint. Preserve unrelated working-tree changes; commit only
reviewed assignment changes and intentionally reconciled dependencies. Push,
merge, release and installation remain outside this assignment.

Record governing decisions, candidate identity, changed files, checks, gaps and
next permitted milestone before handoff. An active plan is not a completed model.

## Executed baseline checks — 2026-09-29

- Inventory regeneration is byte-for-byte deterministic on unchanged inputs:
  37 blueprints, 156 referenced identities, 281 source/manifest files.
- Capability suite: 21 final expectations pass; initial exit-code harness error
  retained and corrected as documented in the evidence.
- `python3 SDP/KanBan/verify_board.py`: PASS, 19 cards, 81 card events and
  nine management records at this checkpoint.
- `python3 tools/validate_blueprints.py`: PASS, 37 blueprints and 71 table-defined
  UR/SR requirements; metadata, matrix, plumbing and local links checked.
  The first run exposed four links broken by the #017 move; all were corrected.
- `python3 tools/check_blueprint_symbols.py`: PASS, 285 implemented callees found;
  this does not establish actual caller semantics.
- `git diff --check`: PASS. Existing ledger bytes are preserved. Producer Git
  status matches the captured pre-probe baseline; its source was not edited.

Current gaps: 18, including SDUI visual-provider fidelity identified after the
first management checkpoint. Model mappings, complete semantic audit, native UI
fidelity and independent review remain unverified. The assignment is uncommitted;
no milestone-completion, publication or product-release claim is made.

[Candidate artifact hashes](evidence/XM-M2/candidate-files.json) identify the child
plans, inventory, gaps and probe artifacts on top of XFMD HEAD. The hash manifest
excludes itself; management status and the parent retain their own append-only
history and live handoff. No product source has changed.

## Current checkpoint — 2026-09-29 continuation

VER-M2: [XM-M3 evidence](evidence/XM-M3/README.md) records source/identity checks, deterministic generation and static visual inspection. Prior candidate hashes are historical. Native behavior and independent review remain unverified.

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
