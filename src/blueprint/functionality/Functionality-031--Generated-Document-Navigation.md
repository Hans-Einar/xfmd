---
id: FUNC-031
kind: Functionality
audience: Integration
role: Workflow
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-044, SR-027, SR-028
uses: FUNC-001, FUNC-002, FUNC-005, FUNC-006, FUNC-007, FUNC-008, FUNC-009, FUNC-016
---

# Functionality-031: Generated document navigation

## 1. Purpose and scope

UC-011: display a navigator and main document using the existing Markdown pipeline.
SDL owns source interpretation, projection and generated resources. XFMD owns its
window, panels, registered-tool invocation and local delivery endpoint. This is a
document host, not an interactive BoxUI/SDUI host.

## 2. Requirements and acceptance

UR-044, SR-027, SR-028; AT-070, AT-071, AT-072 in the
[requirements](../../../xfmd_requirements.md). The gaps before these requirement
IDs are reserved by separate BoxUI work. AT-072 makes existing P050 lease behavior
explicit; it is not a claim that every failure path has been tested.

## 3. Contracts and ownership

Application owns DocumentViews and destroys it before its main window/services.
DocumentViewConfig supplies navigator path, registered tool/source/project,
optional renderer/broker and window identity. The navigator owns a separate
DocumentSession, file adapter, NavigationCoordinator, ScrollCoordinator,
PreviewCoordinator, font metrics, scheduler and FoxRenderHost.

DocumentViews owns one child process group, temporary direct-generation bundles,
its polling scheduler and WindowEndpoint. It holds a lease/broker pair per panel
and a retry queue for release attempts. WindowEndpoint owns the private Linux
SOCK_SEQPACKET socket and per-client/panel sequence records. These are application
contracts; interpreter and renderer receive no SDL/IPC dependency.

Wire profiles (tab-separated, one newline-terminated packet):

```text
XFMD1 INFO window
XFMD1 OPEN window client sequence pane absolute-path end
XFMD1 OPEN window client sequence pane absolute-path lease-id absolute-broker-socket end
SDLVIEW1 RELEASE lease-id
```

The display above uses spaces for readability; actual separators are tabs.
INFO returns main/navigation paths. OPEN returns OK plus sequence or ERROR plus
reason. Pane is main or navigation. Window/client/lease IDs have an ASCII identity
profile of 1–64 characters. A valid newer sequence is consumed before attempting
the open, even when opening fails. Lease metadata is registered only after success.

## 4. Behavior, state and failures

A click uses the originating DocumentViews instance; it never discovers a later
focused window. URI prefix must match the registered project, with maximum 8192
bytes and no tab/newline. Tool arguments use execv, not a shell. A new accepted
selection cancels the previous child process group. Generation has a 65-second
deadline and one active child. Direct mode reads delivery.txt for the target pane
and opens entry.md. Broker mode delegates delivery to the registered reader tool.

The endpoint uses private 0700 same-owner directories, same-UID peer credentials,
32 KiB packets, 16 pending clients, two-second idle deadlines and 256 sequence keys.
The CLI request helper waits up to five seconds for a window response. Wrong targets,
stale/malformed requests and failed reads do not replace the current document.
Addressed opens reject a dirty main buffer without a modal dialog. Navigation-panel
heading links support the current restricted ASCII fragment profile only.

A successful main or navigator document-open callback calls documentChanged(pane).
The old lease is moved into the release queue; successful leased OPEN then installs
the new lease. Failed open leaves the old lease registered. Normal destruction
queues both panel leases, attempts release, cancels the child, destroys the navigator
and removes only window-owned direct-generation files. Broker bundles are not deleted
by XFMD and have no XFMD time-based expiry.

flushReleases retries failed connect/send while the window lives. A full successful
send drops the queue entry **without reading a broker acknowledgment**. This is
best-effort notification, not a durable release guarantee. The queue is not persisted
on shutdown/crash. At 256 pending releases, DocumentViews::open rejects new addressed
opens. Direct UI file operations are not all routed through that admission check.
Broker recovery/retention policies remain SDL-owned.

## 5. Plumbing

| Step | Event / caller | Called symbol | Source or contract file | Data / result | Failure / side effect | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | main, navigator mode | `DocumentViews::DocumentViews` | `src/application/navigation/DocumentViews.cpp` | config → endpoint, navigator, polling | invalid runtime/window/navigator aborts startup | Implemented |
| 2 | main host link callback / navigator host callback | `DocumentViews::follow` | `src/application/navigation/DocumentViews.cpp` | registered URI → captured-window job | reject unknown project; cancel prior child | Implemented |
| 3 | DocumentViews::follow / destruction / timeout | `DocumentViews::cancel` | `src/application/navigation/DocumentViews.cpp` | child process group → stopped/reaped | no background generation after owner destruction | Implemented |
| 4 | scheduler callback | `DocumentViews::poll` | `src/application/navigation/DocumentViews.cpp` | endpoint and child state → delivery/status | generation failure retains current document | Implemented |
| 5 | DocumentViews::poll | `WindowEndpoint::poll` | `src/application/navigation/WindowEndpoint.cpp` | same-user bounded packets → dispatch | close invalid/idle peers | Implemented |
| 6 | WindowEndpoint::poll | `WindowEndpoint::handle` | `src/application/navigation/WindowEndpoint.cpp` | version/identity/sequence/pane/path → callback | invalid/stale requests rejected | Implemented |
| 7 | endpoint.open callback / successful direct generation | `DocumentViews::open` | `src/application/navigation/DocumentViews.cpp` | pane/path → existing document workflow | dirty main or full release queue rejects | Implemented |
| 8 | DocumentViews::open, main pane | `Application::open` | `src/application/Application.cpp` | path → main navigation | read failure keeps document and lease | Implemented |
| 9 | DocumentViews::open, navigation pane | `NavigationPanel::open` | `src/application/navigation/NavigationPanel.cpp` | path → independent navigator | read failure keeps navigator and lease | Implemented |
| 10 | NavigationPanel::open | `NavigationCoordinator::openTarget` | `src/application/navigation/NavigationCoordinator.cpp` | path → transactional document open | failed open does not commit history | Implemented |
| 11 | main documentOpened / navigator changed / destruction | `DocumentViews::documentChanged` | `src/application/navigation/DocumentViews.cpp` | pane → queue preceding lease | no lease mutation for failed opening | Implemented |
| 12 | documentChanged / poll / open / destruction | `DocumentViews::flushReleases` | `src/application/navigation/DocumentViews.cpp` | queued lease/broker → SDLVIEW1 RELEASE | failed send retries; no durable acknowledgment | Implemented |
| 13 | CLI addressed OPEN/INFO | `WindowEndpoint::request` | `src/application/navigation/WindowEndpoint.cpp` | exact window and packet → response | unavailable target or five-second timeout | Implemented |
| 14 | navigator non-SDL link callback | `NavigationPanel::follow` | `src/application/navigation/NavigationPanel.cpp` | local link/ASCII fragment → open/pending anchor | unsupported or missing heading reports error | Implemented |

After row 7 succeeds, WindowEndpoint::handle invokes the injected endpoint.leased
callback to store the new panel lease. That callback is wired in the DocumentViews
constructor; it is not an invented lease-service method. NavigationPanel's modelReady
callback restores a matching heading's source anchor through ScrollCoordinator.

## 6. Reuse and dependencies

Reuse [document state](Functionality-001--Document-Session.md),
[file storage](Functionality-002--Local-File-Storage.md),
[FOX presentation](Functionality-005--FOX-Presentation-Host.md),
[scheduling](Functionality-006--Event-Scheduling.md),
[preview](Functionality-007--Preview-Pipeline.md),
[navigation](Functionality-008--Navigation-History.md),
[anchors](Functionality-009--Source-Anchor-Mapping.md) and
[typography](Functionality-016--Typography-And-Drawing.md).
Consumers are the application main/navigator link callbacks and SDL's registered
reader adapter. No new Markdown/SDL parser, SVG renderer or generic service bus.

## 7. Verification

[P049](../../../sprints/Sprint-004--SDL-Navigation/Phase-049--SDL-Navigation.md)
records real X11 click, navigator retention and two-process endpoint tests (AT-070/071).
Its historical full-suite attribution has an unresolved source-tree identity.
[P050](../../../sprints/Sprint-004--SDL-Navigation/Phase-050--Document-Leases.md)
records broker restart, invalid generation, replacement and normal-close scenarios
(AT-072), external test sources and recovered local evidence.

DocumentViewsGuiTest requires explicit SDL_TOOL_EXECUTABLE configuration.
[P052](../../../sprints/Sprint-006--SDL-Acceptance/Phase-052--SDL-Acceptance.md)
records a new identified local build: NavigationGuiTest, DocumentViewsGuiTest,
DocumentEndpointTest, WindowEndpointBoundaryTest and DocumentLeaseGuiTest all pass,
as does the pinned real-broker restart/replacement/native-close probe. Its manifest
preserves source/binary hashes, tool versions, commands, raw output and screenshot.

WindowEndpointBoundaryTest covers lease metadata, failed-open sequence consumption,
256 sequence keys, 16 pending peers, idle expiry and disconnect timing.
DocumentLeaseGuiTest covers dirty-main rejection without a modal, failed-open lease
retention, release retry, 256-release admission/recovery, both-panel destruction and
unavailable-broker shutdown. Test-only linker wrappers inject foreign SO_PEERCRED
and failed release send; actual same-UID traffic and connect failures use real sockets.
No-acknowledgment and shutdown checks confirm best-effort limits rather than durable
delivery. Real different-UID access and arbitrary crash/commit timing remain untested.
No full-suite/sanitizer or complete Verified status is claimed from this focused run.

## 8. Status, risks and change impact

Implemented in P049/P050 on 2026-09-22; reconstructed against `47a245a` on
2026-09-24. Status is not Verified. Linux is the first IPC profile. Regular startup
keeps the ordinary document workflow; navigator mode is explicit. Changes to the
lease wire contract must be coordinated with SDL's reader/broker and identified
integration evidence. Best-effort release limitations must not be hidden by a
stronger documentation claim.
