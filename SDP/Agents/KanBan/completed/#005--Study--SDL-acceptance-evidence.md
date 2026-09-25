# Complete the remaining SDL navigation acceptance evidence

| Field | Value |
| --- | --- |
| id | KB-XFMD-005 |
| project | XFMD |
| CardState | completed |
| type | Study |
| created | 2026-09-24T14:15:23Z |
| source | Follow-up from KB-XFMD-001 documentation reconciliation, owner conversation 2026-09-24 |
| owner | Codex |
| next_review | Before full Verified status or any proposal for stronger release semantics; see P052 residual limits |
| sources | KB-XFMD-001 |

## Need and retained context

[KB-XFMD-001](%23001--Proposal--Documentation-reconciliation.md)
reconciles documentation, not missing historical execution. Preserve the remaining
verification work here rather than marking FUNC-031 Verified or losing the gaps.
This is a follow-up proposal, not authorization to change application behavior.

[Phase 049](../../../../sprints/Sprint-004--SDL-Navigation/Phase-049--SDL-Navigation.md)
explains why the reported 63-test run cannot be tied to clean 820d86f: the endpoint
test first appeared in d6f75b8. The original tested tree/dirty state/full log were
not recovered. A new run establishes a new baseline; it cannot prove an old run.

[Phase 050](../../../../sprints/Sprint-004--SDL-Navigation/Phase-050--Document-Leases.md)
now preserves recovered metadata/logs, pins available external probe sources and
provides a reproduction recipe. The historical daemon/reader/mmdr binaries and raw
broker-run log remain unidentified. Recovered CTest output has no embedded source SHA.

## Candidate verification scope

- Select exact XFMD/SDL/reader/daemon/renderer revisions; preserve executable hashes,
  clean/dirty state, configure command, generated build identity and raw logs.
- Enable SDL_TOOL_EXECUTABLE explicitly and confirm DocumentViewsGuiTest is listed.
- Re-run real panel click, focus/target retention, two-process endpoint and broker
  restart/replacement/normal-close scenarios against that identified baseline.
- Add or execute explicit cases for dirty-main rejection, failed-open lease retention,
  invalid lease metadata, peer-UID rejection, pending-client/sequence-key bounds,
  disconnect timing, failed-send retry and 256-entry release admission.
- Examine broker/XFMD release failure boundaries: current XFMD drops a queue entry
  after successful send without reading acknowledgment, and does not persist retry
  on shutdown. Decide whether this best-effort contract is sufficient before proposing
  stronger delivery semantics. Do not claim durable or exactly-once release from
  the existing implementation.

Use isolated GUI/process fixtures, never the user's running windows. Only run a
full/sanitizer suite if selected coverage requires it. A focused pass is not full
acceptance. Check whether external repository paths moved; the phase document uses
commit-pinned historical sources, not an assumed current checkout layout.

## Completion criteria

A reviewed acceptance-to-test/evidence matrix tied to actual identified execution,
with explicit remaining limits or separately selected fixes. New evidence is linked
from the phase/blueprint without rewriting historical reports. If further work is
not selected, record that disposition; do not silently promote status to Verified.

## Selection — 2026-09-24

The owner selected #5 and asked that the existing non-UI work be completed before
new UI backlog items being recorded by another agent. Owner: Codex. Scope: build
an identified local verification baseline; execute existing native/broker probes,
add focused failure-path checks where practical, and publish the acceptance/evidence
matrix with explicit residual limits. This selection does not change best-effort
release semantics. Preserve concurrent backlog edits and historical reports.

## Outcome — 2026-09-24

Completed the selected study and verification delivery. [Sprint 006 / P052](../../../../sprints/Sprint-006--SDL-Acceptance/Phase-052--SDL-Acceptance.md)
is the acceptance-to-test/evidence matrix and reproduction record. It preserves
actual source/binary hashes, tool versions, generated build identity, raw logs and
an inspected native screenshot. This note records disposition without duplicating
the product verification report.

- A fresh identified local XFMD build passed all five selected CTests, including
  new protocol and native lease boundary tests. The explicitly enabled real-tool
  GUI test used the newly built SDL CLI.
- SDL CLI, daemon and reader came from an isolated pinned Git archive. The
  unmodified real-broker probe passed with a newly built pinned renderer: leased
  SVG, daemon kill/restart, invalid source, replacement and native window close.
- Dirty-buffer/failed-open retention, sequence/client/release bounds, disconnect,
  retries and shutdown/no-acknowledgment boundaries now have executed evidence.
  Foreign UID and failed send use explicitly labeled test-only fault injection.
- Historical run identities cannot be reconstructed retroactively. Real different-UID
  execution and arbitrary crash/commit timing remain unexecuted. No full-suite,
  sanitizer, durable/exactly-once release or complete Verified status is claimed.

No production behavior change was needed. Retain the documented best-effort
release contract; any stronger acknowledgment/persistence requirement requires
separate selection. The residual limits stay visible in P052 before an integration
or full-verification claim; they are not silently converted into delivered guarantees.

Working branch: sprint/006/phase/052-sdl-acceptance. Delivery remains uncommitted;
no PR, merge or installation performed. Earlier work and the other agent's new
UI notes KB-XFMD-006–011 were preserved. KB-XFMD-003 remains the separate SDUI
widget-ownership question. Completing this item does not select UI implementation.

## SDP KanBan migration — 2026-09-25

EVT-KB-XFMD-000059: relocated the board and added visible CardState (completed).
Existing lifecycle, IDs, decisions and evidence remain unchanged. The owner now
authorizes XFMD's SDP process area; earlier instructions to remain note-only
are historical. [Migration evidence](../../../Maintenance/SDP1/Plan-and-Evidence.md).
