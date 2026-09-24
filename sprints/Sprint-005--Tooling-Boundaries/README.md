# Sprint 005 — Tooling boundaries

Selected from [KB-XFMD-002](../../Agents/KanBan/completed/%23002--Study--BoxUI-branch-disposition.md)
on 2026-09-24: preserve two independent tooling fixes from the historical BoxUI
branch. XFMD owns its Rust formatting and project documentation checks; dependency
source and documentation have their own maintainers.

| Phase | Branch | Delivery |
| --- | --- | --- |
| [051](Phase-051--Tooling-Boundaries.md) | sprint/005/phase/051-tooling-boundaries | Package-scoped Rust formatting and dependency-document exclusion. |

Base: `47a245a`, with the uncommitted KB-XFMD-001 documentation reconciliation
carried forward from `docs/kanban-documentation-reconciliation`. Existing work is
preserved. This is developer-tool maintenance, with no new product requirement,
runtime contract or blueprint object. The working-method validation contract is
the affected documentation. No BoxUI implementation or dependency-pin update is
included. A combined sprint PR is not created; commits and remote integration
remain pending. See the phase report for actual checks and limitations.
