# XFMD sprint register

## Delivery overview — local history reviewed 2026-09-24

| Delivery | Phases / final branch | State and evidence |
| --- | --- | --- |
| Historical baseline | P0–P41 | Integrated in main; [phase history](../implementationPlan.md), [P41 integration](../docs/evidence/P41-integration.md). |
| [Sprint 001 — build identity](Sprint-001--Versioning/README.md) | P042 / sprint/001/phase/042-build-identity | Integrated by PR35 merge 4e21e99; phase build/evidence retained. |
| [Sprint 002 — status version](Sprint-002--Status-Version/README.md) | P043 / sprint/002/phase/043-status-version | Integrated by PR36 merge c245fd9; phase build/evidence retained. |
| Sprint 003 — historical BoxUI prototype | P044–P048 / sprint/003/phase/048-boxui-verification at a7495b8 | Parallel branch from c245fd9, not integrated into local main or Sprint 004. [Historical delivery](https://github.com/Hans-Einar/xfmd/blob/a7495b8/sprints/Sprint-003--Interactive-BoxUI/README.md). |
| [Sprint 004 — SDL document navigation](Sprint-004--SDL-Navigation/README.md) | P049–P050 / sprint/004/phase/050-document-leases at 47a245a | Implemented on separate branch from c245fd9; PR38 identified in dated SDL documentation. Evidence is partial and provenance limits are explicit. |
| [Sprint 005 — tooling boundaries](Sprint-005--Tooling-Boundaries/README.md) | P051 / sprint/005/phase/051-tooling-boundaries | Two isolated tooling fixes implemented and locally checked; working-tree delivery, no commits/PR yet. |
| [Sprint 006 — SDL acceptance](Sprint-006--SDL-Acceptance/README.md) | P052 / sprint/006/phase/052-sdl-acceptance | 5/5 focused CTests and pinned native-broker probe pass; source/binary hashes and residual limits recorded. Uncommitted local delivery. |

Main was c245fd9 when reviewed. These are local ancestry and dated source findings,
not a live query of GitHub PR/CI state. BoxUI was an interactive FOX-host prototype;
the later SDUI direction uses standalone Go/Fyne. Branch disposition and isolated
reuse remain separate work, not an instruction to merge Sprint 003. Its reserved
IDs are preserved, and its documentation is not copied here as active product scope.

## Selected UI delivery

[Sprint 007](Sprint-007--Workspace-UI/README.md) implements selected KB-XFMD-006–011
on phase branches P053–P055. P053 opening/routing and P054 workspace composition
are locally complete with linked acceptance evidence. P055 shared zoom remains
selected and queued. SDUI ownership is excluded.

## Working convention

A sprint is a bounded integration delivery, not mandatory agile ceremonies or SDP.

- Sprint-NNN--Topic/README.md: goals, requirements, phases, final branch and one PR.
- Phase-NNN--Topic.md: branch, milestones, acceptance and actual build evidence.
- One branch per phase; one commit per milestone; next phase starts from the previous.
- One PR from the final phase branch to main; preserve commits with a merge commit.
- Autonomous work builds once per planned phase boundary; corrective checks remain
  allowed. Collaborative work may build each fix. Documentation-only evidence does
  not require rebuilding unchanged application code.

Numbers are stable. Sprint numbering begins at 001 and phase numbering continues
from P41 with P042. Do not recast old phase history as invented sprint deliveries.
[Working method](../docs/working-method.md), [versioning](../docs/versioning.md),
[phase template](templates/Phase.md). Discussion and reconciliation notes live in
[KanBan](../Agents/KanBan/README.md); they are not automatically new product sprints.
