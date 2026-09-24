# Phase 049 — navigation and addressed document display

Sprint 004. Branch `sprint/004/phase/049-sdl-navigation`, from main `c245fd9`.
Mode: autonomous. Implemented; acceptance evidence is partial and qualified below.
UR-044/SR-027, FUNC-031; AT-070/071. SDL's G6-M3 is the external consumer.

## Milestones

| Milestone | Commit | Delivered scope |
| --- | --- | --- |
| P049-M1 | 820d86f | Requirements/blueprint, two Markdown panels, registered SDL tool and XFMD1 endpoint. |
| P049-M2 | d6f75b8 | Native/IPC tests, shared reading colors, restricted ASCII heading anchors, documented phase results and X11 capture. |

The original milestone report says a Release build from 820d86f (CMake/Ninja,
FOX 1.6.57, Pango/Cairo) passed 63 CTests in 145.69 seconds, then three affected
native tests passed again in 6.38 seconds. Blueprint counts were 37 objects,
69 requirements and 239 callees. These are the historical reported results, not
new runs performed during documentation reconciliation.

## Evidence provenance correction — 2026-09-24

DocumentEndpointTest and its CTest registration were first added in d6f75b8.
That commit also changes production code and the GUI test. A clean 820d86f tree
cannot contain that endpoint test. The original complete tested tree, dirty state,
full-suite log and exact compiler/dependency inputs have not been recovered.
Therefore the 63-test report cannot be attributed confidently to clean 820d86f
or transferred to d6f75b8 as an independently identified run.

The recovered local three-test log belongs to the later P050 evidence bundle; it
must not be reused as proof of a P049 full-suite run. A new run could establish a
new baseline, but cannot repair the identity of the historical run retroactively.

## Reported scenarios and preserved sources

DocumentViewsGuiTest sends real X11 press/release events to a Markdown link and
uses a registered Go SDL tool. It checks the generated main document, retained
navigator, immediate focus change, invalid project/viewpoint and heading VP02.
DocumentEndpointTest starts two FOX processes and checks explicit window/panel,
stale/malformed/oversized requests, missing-file retention and a closed target.
The source remains under tests/gui; [the original X11 image](navigation.png) is
preserved. Its historical report says it was visually inspected; it has not been
reclassified as newly inspected evidence.

Not all acceptance cases are covered by these sources: dirty-main rejection,
peer-UID rejection, client/sequence saturation and disconnect during delivery need
explicit failure-path evidence. Multiple windows are demonstrated as independent
processes, not a general multiwindow API inside one Application.

## Profile and reproduction

Linux XFMD1/SOCK_SEQPACKET: 32 KiB packets, same UID, 16 pending peers, 256
client/panel sequence keys, two-second idle deadline. One generation process group,
65-second deadline. Direct bundles live until normal window destruction. P050 adds
broker leases. There is no global --active target lookup.

Follow the [P050 reproduction instructions](Phase-050--Document-Leases.md), with
an identified source checkout and registered SDL executable. In particular,
DocumentViewsGuiTest is omitted unless SDL_TOOL_EXECUTABLE is configured. Preserve
actual build metadata and logs when rerunning; do not copy this historical count.

## New identified evidence — 2026-09-24

[P052](../Sprint-006--SDL-Acceptance/Phase-052--SDL-Acceptance.md) records a fresh
local build, 5/5 focused CTests and a real broker probe with source/binary hashes.
It adds dirty-main rejection, invalid lease metadata, client/sequence bounds and
disconnect evidence. Foreign-UID rejection is fault-injected, not demonstrated
between actual users. This supplements the history above; it does not repair the
identity of the original 63-test run or promote FUNC-031 to Verified.
