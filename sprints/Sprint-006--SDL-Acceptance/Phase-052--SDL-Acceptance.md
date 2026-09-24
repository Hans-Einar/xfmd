# Phase 052 — SDL acceptance evidence

Branch: `sprint/006/phase/052-sdl-acceptance`. Mode: collaborative, focused native
verification. Status: locally completed, uncommitted. Owner: Codex.
No product behavior change selected; no PR, merge or installation performed.

## Scope and acceptance

UR-044 / AT-070: rerun real registered SDL click and target retention; add a dirty
main-buffer rejection check that detects unexpected modal-dialog invocation.
SR-027 / AT-071: rerun independent processes and extend protocol evidence for
lease metadata, sequence/client limits, idle peers and disconnect timing.
SR-028 / AT-072: test failed-open lease retention, retry, queue admission and
best-effort release without acknowledgment; rerun the real broker restart,
replacement and normal-close probe.

Existing owner: [FUNC-031](../../src/blueprint/functionality/Functionality-031--Generated-Document-Navigation.md).
No new runtime contract or planned production symbol. Tests use public entrypoints,
isolated temporary runtime directories and Xvfb. Any injected failure must be labeled
separately from real-kernel execution. A new pass cannot identify an old test run.

## Milestone

| ID | Delivery | Commit | Check |
| --- | --- | --- | --- |
| M01 | Failure-path tests and identified acceptance matrix | Uncommitted working-tree delivery | 5/5 focused CTests and pinned external native-broker probe pass. |

## Execution plan

Use a new `build-acceptance` directory; preserve the historical `build` artifacts.
Build SDL CLI, reader and daemon from an isolated Git archive of SDP `b784552`,
including that revision's SDUI module. Do not build from another agent's changing
checkout or reuse unidentified historical `/tmp` binaries. Record tool versions,
source-tree hashes, generated XFMD identity, executable hashes, commands and raw logs.
Use the pinned XFMD Mermaid renderer source for the external renderer binary.

The final matrix must identify passed, injected, inspected-only and remaining
cases. FUNC-031 stays Implemented unless all acceptance obligations have evidence.

## Identified local execution — 2026-09-24

[Verification manifest](evidence/verification.json) records HEAD `47a245a`, the
dirty source state, source-file hashes, tool versions, source revisions and binary
hashes. Runtime source under src (excluding blueprints) matches HEAD. Test/CMake
changes, earlier documentation/tooling changes and concurrent UI backlog additions
make the working tree dirty. This is a local verification build, not a published
clean milestone. [Generated build identity](evidence/xfmd-build.json):
`0.1 sprint/006/phase/052-sdl-acceptance:191+dirty`.

SDL CLI/daemon/reader were built from the archived SDP revision identified in the
manifest with Go 1.27.1. Renderer source is XFMD's existing pin `589517a`; all 1,283
tracked files were compared to its Git blobs without mismatches. No dependency pin
was upgraded. Existing cmark static libraries and MicroTeX source are reused as
identified configure inputs; the new XFMD/test executables were built separately.

The initial configure could not locate cmark's pkg-config entry. The successful
configure supplies its existing local prefix explicitly and the existing MicroTeX
source directory. Commands and inputs are recorded rather than installing or
modifying system dependencies. Logs: [configure](evidence/configure.txt),
[build](evidence/build.txt), [test build](evidence/boundary-build.txt).

The [registered test list](evidence/test-list.json) confirms that the conditional
DocumentViewsGuiTest uses the newly built SDL executable. The [verbose CTest log](evidence/ctest.txt)
records **5/5 passes in 21.82 seconds**, using isolated Xvfb displays and a private
runtime directory. Only selected targets/tests were built/run; no full-suite or
sanitizer result is claimed.

## Acceptance-to-evidence matrix

| Acceptance / case | Result | Evidence and precise boundary |
| --- | --- | --- |
| AT-070: real link click, captured target, retained navigator, invalid project/viewpoint | Pass | DocumentViewsGuiTest, real pinned SDL CLI and native X11 events; immediate focus change after click. NavigationGuiTest also passes. |
| AT-070/071: independent window processes, explicit main/navigation target, missing/closed target | Pass | DocumentEndpointTest; checks addressed process paths and preserves the other process. |
| AT-070/072: dirty main and failed-open lease retention | Pass | DocumentLeaseGuiTest uses real endpoint packets and the production edit controller; checks unchanged dirty token/text/path, zero modal callbacks, no release on rejection and only the original lease released on later success. |
| AT-071: malformed/oversized/wrong-target/stale packets | Pass | DocumentEndpointTest against native processes. |
| AT-071/072: invalid lease metadata; unsuccessful opening consumes sequence without registering lease | Pass | WindowEndpointBoundaryTest exercises handle with callback counters; valid retry must use a newer sequence. |
| AT-071: 256 client/panel sequence keys | Pass | WindowEndpointBoundaryTest rejects a new 257th key without calling open; an existing key still advances. |
| AT-071: 16 pending peers, overflow and two-second idle expiry | Pass | Real local sockets in WindowEndpointBoundaryTest; overflow is closed, 16 idle clients expire and capacity is usable again. |
| AT-071: disconnect before and after packet transmission | Pass | Real sockets; pre-packet disconnect leaves state unchanged. A sent packet can commit after disconnect; its sequence cannot be replayed even when the reply was lost. |
| AT-071: foreign peer UID | Injected pass | GNU linker wrapper changes only the successful SO_PEERCRED result in this test executable. Rejection is exercised, but no actual different-UID client was run. Normal same-UID traffic uses real credentials. |
| AT-072: failed connect/send retry | Pass / injected pass | Broker socket disappearance gives real connect failures. A test-only send wrapper forces EAGAIN for release packets; removing the fault allows release on a subsequent poll. |
| AT-072: 256 pending releases and recovery | Pass | Real native endpoint/lease queue. Overflow retains the document; restored broker receives all 256 distinct pending releases, then addressed opens resume. |
| AT-072: normal destruction, both panels | Pass | DocumentLeaseGuiTest observes exactly one release per active panel on owner destruction. Native WM_DELETE_WINDOW with the real broker is a separate probe below. |
| AT-072: send succeeds but no acknowledgment arrives | Pass, limitation confirmed | Stub broker never acknowledges or performs durable work; XFMD does not retry successfully sent releases. This is not durable delivery evidence. |
| AT-072: shutdown while broker unavailable | Pass, limitation confirmed | Destroy owner with pending release, recreate owner and restore broker: no replay occurs. No release journal is persisted. |
| AT-072: real broker restart, leased SVG, invalid generation, replacement and native close | Pass | Unmodified verify_native_broker.py from the pinned SDP revision; new daemon/reader/renderer/XFMD binaries, private runtime and Xvfb. See execution below. |

Test ownership: application/navigation verification. New tests live in
tests/application/WindowEndpointBoundaryTest.cpp and tests/gui/DocumentLeaseGuiTest.cpp;
cmake/Tests.cmake registers them. Linker wrappers affect only those test binaries,
not XFMD. Tests use public contracts and observable callbacks/socket traffic;
they do not expose private production state.

## Real broker probe

The unmodified external probe completed with exit code 0; [raw output](evidence/broker.txt),
[renderer build log](evidence/renderer-build.txt) and [native capture](evidence/broker.png)
are preserved. The manifest identifies its source commit, exact script hash,
invocation and all nine tested executable hashes. All 373 recorded source hashes
and evidence-file hashes were checked after execution.

The probe opens a generated SVG document, kills/restarts its own daemon, checks
that the leased bundle survives a sweep, rejects a stale request and invalid SDL,
then replaces the document and checks reclamation of the old bundle. Finally it
sends native WM_DELETE_WINDOW to its isolated FOX window and checks reclamation of
the active bundle. The capture was inspected: the separate navigator remains
visible beside VP02 with the replacement `Reloaded (unit)` SVG and the expected
dirty-build identity. This is acceptance evidence for document hosting, not a UI
redesign approval or evidence for interactive SDUI/BoxUI hosting.

Final non-runtime checks pass: blueprint structure (37 objects / 70 requirements),
250 implemented plumbing callees, pure-layer boundaries, clang-format for both new
tests and git diff --check. Board replay confirms 11 cards / 25 events, including
the other agent's six UI cards, with consistent predecessor chains, paths and links.
KB-XFMD-005 is completed; UI cards remain in backlog. No unrelated runtime tests
were repeated after the focused pass because subsequent edits only record evidence.

## Remaining limits and disposition

- Historical P049/P050 binary/run provenance remains unresolved. These new logs
  establish a new baseline and do not retroactively prove the historical runs.
- Real different-UID access rejection remains unexecuted; injection tests the
  application decision, not kernel credential isolation or filesystem permissions
  between actual users.
- No arbitrary crash-timing campaign across XFMD/broker send, receipt and durable
  commit. A successful send may have an unknown broker outcome; the tests explicitly
  preserve that contract instead of claiming exactly-once or durable release.
- No change to release semantics is needed to meet the documented best-effort
  contract. Stronger crash recovery would require separately selected broker/XFMD
  protocol work. Revisit if reliable release acknowledgment/persistence becomes a
  product requirement. FUNC-031 remains Implemented, not fully Verified.
