---
id: FUNC-011
kind: Functionality
audience: User
role: Service
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-032, UR-033, UR-011, UR-003, UR-004, UR-009, SR-002, SR-006, SR-008, SR-013, UR-016, SR-019
uses: FUNC-001, FUNC-005, FUNC-007, FUNC-015
---

# Functionality-011: Text editing, undo and search

## 1. Purpose and scope

Provide ordinary editing through one path to document revisions. The FOX editor
is a projection, not competing source state or a replacement for xfw's advanced tools.

## 2. Requirements and acceptance

Requirements are listed in metadata and defined in the [requirements](../../../xfmd_requirements.md).
AT-025, AT-003, AT-004, AT-009, AT-012, AT-016, AT-018, AT-023 cover the baseline. P11 added AT-030, AT-039;
P22 added UR-032, UR-033 and AT-052, AT-053. P055 revises zoom under the same IDs.

## 3. Contracts and ownership

EditController owns applyEdit/applyProjectedText/undo/redo/find. TextProjection maps
LF display to raw offsets and original newline policy. EditorWidget owns FXText
projection. P11 retained undo/source ownership and consumed the shared FUNC-015
wheel profile; source restore/search use direct positions with explicit origins.
EditorPresentation owns fonts/wrap independently from edit operations (P22).

## 4. Behavior, state and failures

One undo stack has a 32 MiB history budget. Programmatic projection produces no edit.
Preserve BOM and untouched mixed newline sequences; new lines use the first newline
format. UTF-8 differences expand to character boundaries. Search does not dirty;
opening resets undo, view changes do not. TextProjection reserves offset/text
capacity from known raw length to avoid duplicate growth peaks (P14 measurement).

FoxWheelScrollBar owns both axes and fractional wheel state; widgets replace bars
before create and own them. P055 intercepts Ctrl+wheel only for document zoom,
retaining normal/Alt/Shift scrolling. Ctrl+plus/minus/keypad is consumed before FXText
can mutate text. DocumentZoom supplies a shared factor to setViewProfile; both Wrap
and A4 scale the base font. A4 source columns remain fixed, Wrap reflows. Font pixel
rounding is visible but does not create a second fit policy. Preserve source anchor,
caret and selection during layout. No edit/undo or preference mutation from zoom.

## 5. Plumbing

| Step | Event / caller | Called symbol | Source or contract file | Data / result | Failure / side effect | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `FXText edit event` | `EditorWidget::onChanged` | `src/application/ui/EditorWidget.cpp` | FXString → edited callback | Ignore programmatic projection | Implemented |
| 2 | `Application edited callback` | `EditController::applyProjectedText` | `src/application/document/EditController.cpp` | LF text → raw edit | Error restores projection | Implemented |
| 3 | `EditController::applyProjectedText` | `TextProjection::difference` | `src/application/document/TextProjection.cpp` | New text → raw edit | UTF-8/newline mapping | Implemented |
| 4 | `EditController::apply` | `DocumentSession::applyEdit` | `src/application/document/DocumentSession.cpp` | Edit → revision | Validate input | Implemented |
| 5 | `EditController changed callback` | `Application::updateUi` | `src/application/Application.cpp` | Snapshot → editor/title | contentChanged schedules preview | Implemented |
| 6 | `Application::execute` | `EditController::find` | `src/application/document/EditController.cpp` | Query → projected offset | No dirty change | Implemented |
| 7 | `EditorWidget constructor` | `FoxWheelScrollBar::replace` | `src/application/adapters/FoxWheelScrollBar.cpp` | Standard bar → adapter | Parent owns bar; before create | Implemented |
| 8 | `FOX wheel dispatch` | `FoxWheelScrollBar::onMouseWheel` | `src/application/adapters/FoxWheelScrollBar.cpp` | Delta/remainder → target/timer | Clamp; retain fractional remainder | Implemented |
| 9 | `FoxWheelScrollBar::onMouseWheel` | `ScrollDynamics::advance` | `src/application/scroll/ScrollDynamics.cpp` | Input → scroll target | No text mutation | Implemented |
| 10 | `Application::applyAppearance / changeReadingColors` | `EditorWidget::setReadingColors` | `src/application/ui/EditorPresentation.cpp` | Palette → editor/cursor | No text mutation | Implemented |
| 11 | `EditorWidget::layout` | `EditorWidget::applyViewProfile` | `src/application/ui/EditorPresentation.cpp` | Paper/scale → font/wrap | Preserve selection/source anchor | Implemented |
| 12 | `DocumentZoom::refresh` | `EditorWidget::setViewProfile` | `src/application/ui/EditorPresentation.cpp` | Shared manual/fit factor → presentation | No independent fit calculation for this consumer | Implemented |

## 6. Reuse and dependencies

FUNC-001/005/007/015 retain their owners. Live preview consumes edits; storage is
unchanged. New editing actions are Edit operations, not separate FXText/session
mutations. P055 workspace zoom uses FUNC-010's coordinator and existing scroll restore.

## 7. Verification

DocumentTest/WorkspaceTest cover Unicode edits, mixed EOL/BOM, dirty baseline,
native events, undo/redo and search. Historical evidence: [P2](../../../docs/evidence/P2.md),
[gestures/bounds](../../../docs/evidence/wheel-scrolling.md), [P11](../../../docs/evidence/P11.md),
[P22](../../../docs/evidence/P22.md). P7 assessed aggregate baseline coverage.
P055 zoom acceptance is recorded in [evidence](../../../sprints/Sprint-007--Workspace-UI/evidence/P055.md).

## 8. Status, risks and change impact

Original implementation P2; revision 1.1 in P11 and presentation split P22. Earlier
evidence retains dated scope. Keep calls/contracts/consumers/tests together; pure
ports and explicit ownership remain mandatory. P055 implements the shared zoom plumbing; native acceptance remains separately
recorded. No broad Verified claim.
