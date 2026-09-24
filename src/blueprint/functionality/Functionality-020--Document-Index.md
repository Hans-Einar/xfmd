---
id: FUNC-020
kind: Functionality
audience: System
role: Service
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-021, UR-022, UR-023, SR-020
uses: FUNC-002, FUNC-003, FUNC-008
---

# Functionality-020: Document index

## 1. Purpose and scope

Heading/reference metadata, independent of layout.

## 2. Requirements and acceptance

UR-021, UR-022, UR-023, SR-020; see the [requirements](../../../xfmd_requirements.md).

## 3. Contracts and ownership

DocumentIndex is value-based semantic metadata. ReferenceWorker receives IInterpreter
and LocalFileStore from the composition root; it owns no widgets or active session.
FUNC-010 owns UI. The service returns owned strings, levels and byte anchors.

## 4. Behavior, state and failures

Headings nest under the nearest shallower level without invented intermediate
levels. References groups Markdown and Hyperlinks; file children load lazily, one
level deep. Stale revisions disable activation and discard jobs; errors appear
under the file. Single click/Enter navigates, arrows select, dirty-cancel retains
the active file.

P054 Index Refresh cancels queued/completed reference work and rebuilds the index
from the accepted model when its token matches the current buffer. If edits are
pending, PreviewCoordinator::refresh requests that buffer's model. Neither route
reads the main file from disk or mutates text/undo. Rebuilding IndexPanel removes
loaded reference children so subsequent expansion requests fresh headings. Files
Refresh remains a separate active-tab action.

## 5. Plumbing

| Step | Event / caller | Called symbol | Source or contract file | Data / result | Failure / side effect | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | PreviewCoordinator modelReady | `DocumentIndex::build` | `src/application/index/DocumentIndex.cpp` | Semantics → headings/links | No extra parse | Implemented |
| 2 | IndexPanel referenceRequested | `ReferenceWorker::submit` | `src/application/index/ReferenceWorker.cpp` | Path → lazy read | Queue bound/error result | Implemented |
| 3 | ReferenceWorker::run | `LocalFileStore::read` | `src/application/io/LocalFileStore.cpp` | Path → snapshot | Regular file, read/encoding/8 MiB checks | Implemented |
| 4 | ReferenceWorker::run | `IInterpreter::parse` | `src/contracts/IInterpreter.h` | Snapshot → semantics | Parser error as result | Implemented |
| 5 | Application::pollReferences | `ReferenceWorker::take` | `src/application/index/ReferenceWorker.cpp` | Result queue → owned headings | Generation rejects stale jobs | Implemented |
| 6 | Application::wireWorkspace indexRefresh callback | `ReferenceWorker::cancel` | `src/application/index/ReferenceWorker.cpp` | new reference generation → discard stale results | No document read or mutation | Implemented |
| 7 | Application::wireWorkspace indexRefresh callback | `IndexPanel::present` | `src/application/ui/IndexPanel.cpp` | current accepted model → index | Clear lazy-reference children | Implemented |
| 8 | indexRefresh with pending buffer model | `PreviewCoordinator::refresh` | `src/application/preview/PreviewCoordinator.cpp` | current snapshot → model request | Existing stale-token rejection | Implemented |

## 6. Reuse and dependencies

FUNC-003 supplies parsing, FUNC-008/009 navigation/source mapping, FUNC-010 workspace.
FTR-008/009 consume the service; there are no feature-private calls.

## 7. Verification

AT-040, AT-041, AT-042, AT-043: DocumentIndexTest, ReferenceWorkerTest and IndexGuiTest
cover semantic levels, table links, lazy failures/stale results and native activation.
Evidence: [P15](../../../docs/evidence/P15.md).

P054 adds active-tab Refresh against the dirty buffer and loaded references; acceptance
is recorded in [P054](../../../sprints/Sprint-007--Workspace-UI/Phase-054--Workspace-Layout.md).

## 8. Status, risks and change impact

Implemented in P15, 2026-09-13. Its M1 model and M2 GUI checks were completed and M3
collected evidence. Reference headings are snapshots; expansion rereads the file.
Blocking filesystem calls cannot be interrupted mid-call. P054 refresh reuses
existing cancellation/model owners and does not reload the main document.

P054 local acceptance: [workspace evidence](../../../sprints/Sprint-007--Workspace-UI/evidence/P054.md)
records native interaction/visual checks, focused ASan/UBSan checks and the final
source/binary manifest. Earlier phase placement descriptions retain their dated
scope. Status remains Implemented; this is not blanket physical-display verification.
