# Sprint 004 — SDL document navigation

Requirements UR-044/SR-027/SR-028; owner FUNC-031, use case UC-011. Based on
main `c245fd9`; the separate BoxUI worktree was not integrated. This is a FOX
Markdown document host for SDL output, not an interactive SDUI/BoxUI host.
Autonomous phase builds; no merge or installation is implied by implementation.

| Phase | Branch | Milestone commits | Evidence |
| --- | --- | --- | --- |
| 049 | sprint/004/phase/049-sdl-navigation | M1 820d86f; M2 d6f75b8 | [Navigation and endpoint](Phase-049--SDL-Navigation.md) |
| 050 | sprint/004/phase/050-document-leases | M1 5fa35e3; M2 47a245a | [Broker leases](Phase-050--Document-Leases.md) |

The combined delivery is identified as [PR38](https://github.com/Hans-Einar/xfmd/pull/38)
in SDL's dated checkpoint. Local main remains `c245fd9`; current remote PR status
has not been queried. Integration should use the final phase branch and preserve
history. No per-phase PR is intended.

Documentation was reconciled on 2026-09-24 against `47a245a`. SR-028/AT-072 name
existing P050 lease behavior; they do not add code or imply full verification.
Historical phase reports are separated below from recovered artifacts and coverage
gaps. Do not infer the tested source tree from a milestone title alone.
