---
id: FUNC-015
kind: Functionality
audience: System
role: Mechanism
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-011, UR-015, UR-016, SR-002, SR-015, SR-019
uses: FUNC-014, FUNC-006
---

# Functionality-015: Shared scroll input and motion policy

## 1. Purpose and scope

Own normalization, speed, acceleration, remainder, target and termination for wheel
scrolling. FUNC-009 retains source-anchor synchronization. P055 document zoom consumes
Ctrl+wheel before scroll motion while other surfaces retain existing policy.

## 2. Requirements and acceptance

UR-011, UR-015, UR-016, SR-002, SR-015, SR-019; [requirements](../../../xfmd_requirements.md).
AT-012, AT-025, AT-029, AT-030, AT-035, AT-039. P055 narrows the Ctrl=page rule on document surfaces only.

## 3. Contracts and ownership

FOX-free ScrollInput carries axis/delta/timestamp/origin (source Unknown); ScrollProfile
carries speed/acceleration/strength/maxGain, with internal v0=8. ScrollDynamics::advance
and reset are pure and receive a monotonic clock. FoxWheelScrollBar normalizes
FXEvent.code/120, owns FOX notifications and separate dynamics per bar/axis. Core
FOX events cannot reliably identify fingers or lift; Unknown is valid.

The original proposed tuning was speed=1.5 (0.25–4), acceleration=false, k=0.5 (0–2),
v0=8 wheel units/s, maxGain=3 (1–5); P10 compared compatibility speed=1/no acceleration.
For normalized increment q: g=acceleration ? clamp(1+k*max(0,v/v0-1),1,maxGain) : 1;
dTarget=-q*baseUnit*speed*g. baseUnit follows FOX line/wheelLines/page content pixels.
Integrate only new increments, never re-multiply accumulated distance. Sync uses an
absolute target without gain. Velocity uses absolute input over 80 ms in 8 ms bins;
coalesced input does not invent physical events. Idle 200 ms, axis/target/sign changes
and preferences reset velocity/remainder. Same-timestamp events accumulate using
local monotonic time, not assumed physical gesture timestamps.

## 4. Behavior, state and failures

Alt=line and Ctrl=page bypass personal gain/acceleration. P055's explicit document
zoom callback overrides Ctrl=page for editor/preview and their bars, cancels pending
motion and consumes the event once. Trees/MRU/preferences sample have no such callback.
Keyboard, drag, Sync and Restore retain direct precise commands; respect input sign
without another natural-scroll inversion.

The adapter owns one retargetable timer (8 ms ticks, up to 80 ms settling), with no
parallel inherited onTimeWheel. Motion clamps and stops at the exact integer target;
no added kinetic distance. Drag/key/range/document changes cancel old motion. Preserve
fractional remainder within bounds, discard excess at endpoints; reject infinite
input and unsafe integer conversion. No global inertia engine.

## 5. Plumbing

| Step | Event / caller | Called symbol | Source or contract file | Data / result | Failure / side effect | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `FOX SEL_MOUSEWHEEL` | `FoxWheelScrollBar::onMouseWheel` | `src/application/adapters/FoxWheelScrollBar.cpp` | FXEvent → normalized input | Consume once | Implemented |
| 2 | `FoxWheelScrollBar::onMouseWheel` | `ScrollDynamics::advance` | `src/application/scroll/ScrollDynamics.cpp` | Input/profile/range → target | Per-bar remainder/velocity | Implemented |
| 3 | `FOX timeout` | `FoxWheelScrollBar::onMotionTick` | `src/application/adapters/FoxWheelScrollBar.cpp` | Target → position/SEL_CHANGED | Finish SEL_COMMAND; no duplicate timer | Implemented |
| 4 | `document Ctrl+wheel callback` | `DocumentZoom::step` | `src/application/zoom/DocumentZoom.cpp` | Signed fractional steps → scale | No simultaneous scrolling | Implemented |

## 6. Reuse and dependencies

SidebarWidget, WorkPathList, EditorWidget, FoxRenderHost and preferences sample share
policy types/profile. FTR-006 owns the user result; FTR-004 still uses FUNC-009 after
actual viewport movement. P055 zoom is explicit per-document-surface wiring, not a
new global modifier rule or a feature-private call.

## 7. Verification

Deterministic series cover base/gain, ±1/120 increments, batching, reversal, idle,
overflow and endpoints. Native FOX/X11 tests exercise tree/text/preview surfaces,
not only adapter methods. Physical touchpad measurements remain separate: synthetic
fine deltas do not prove driver behavior. Historical AT evidence: [P10](../../../docs/evidence/P10.md).
P055 zoom and unchanged sidebar motion checks follow its [plan](../../../sprints/Sprint-007--Workspace-UI/Phase-055--Document-Zoom.md).

## 8. Status, risks and change impact

Revision 1.1, 2026-09-13: P10 implements the original calls. [P9](../../../docs/evidence/P9.md)
resolved the core-event path; P10-M1 extracted remainder/clamp, M3 added acceleration
and timer. XI2 remains a later adapter if measurement proves missing resolution;
it must replace, not duplicate, the same core stream. No libinput/xfw/system-FOX change.
[Design](../../../softwareDesign.md), [historical plan](../../../implementationPlan.md).

P055 local zoom acceptance and bounded limits: [evidence](../../../sprints/Sprint-007--Workspace-UI/evidence/P055.md).
