# KB-XFMD-016 — Initialize SDP and align KanBan

| Field | Value |
| --- | --- |
| id | KB-XFMD-016 |
| project | XFMD |
| type | Change |
| CardState | completed |
| created | 2026-09-25T11:02:19Z |
| source | owner-conversation-2026-09-25 |
| next_review | None; bounded bootstrap complete |

## Selected scope

Initialize XFMD's own five-phase SDP area, relocate the existing board, adopt a
pinned SDP KanBan baseline and preserve all card identities and historical event
bytes. Update current instructions and links. Owner: Project Owner; executor: Codex.

Plan and acceptance: [SDP1-M1](../../../Maintenance/SDP1/Plan-and-Evidence.md).
Product code, existing requirements/design, GUI behavior and model generation
are outside the bootstrap. A separate agent owns
[KB-XFMD-017](../backlog/%23017--Proposal--Adopt-SDP-and-model-XFMD.md).

## Outcome and verification

2026-09-25T11:05:31Z, Codex, EVT-KB-XFMD-000069: SDP1-M1 completed. Five phase entries and
traceability/verification entry points exist; the board uses the pinned SDP KanBan
rules. All 29 original board files are accounted for, the 51-event ledger prefix
and non-Markdown bytes are preserved, and current CardState is explicit.
Schema/replay/state/index, lineage positive/negative examples, local link and
blueprint/symbol checks passed. No application code or GUI behavior changed.
[Evidence](../../../Maintenance/SDP1/Plan-and-Evidence.md) records the limits.
KB-XFMD-017 remains backlog for another agent; its implementation is not part of
this closure. KB-XFMD-012 retains future published-version alignment.
