---
id: FUNC-026
kind: Functionality
audience: Integration
role: Service
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-042, SR-025
uses: none
---

# Functionality-026: Build identity

## 1. Purpose and scope

Give CLI, window title and status bar the same concise product version, with
installed JSON identifying the actual built code. Build tools read Git; the
application uses embedded values.
This is a service, not a new feature.

## 2. Requirements and acceptance

UR-042, SR-025; AT-066, AT-067 in the [requirements](../../../xfmd_requirements.md).
[Versioning](../../../docs/versioning.md) defines counting and special cases.

## 3. Contracts and ownership

VERSION owns major/minor. tools/build_identity.py generates header/JSON under
build/generated. Application's BuildVersion exposes buildVersion(). No Git,
Python, network or mutable counter is needed at runtime.

## 4. Behavior, state and failures

Display only VERSION's major.minor, as requested by the owner on 2026-11-10.
JSON retains all reachable Git commit counts, branch, main's first-parent PR when
available, SHA and dirty state. Source archives retain branch=source and null
commit/count. Reject shallow history. Do not rewrite identical generated content;
metadata can change after a commit without changing the display header.

## 5. Plumbing

| Step | Event / caller | Called symbol | Source or contract file | Data / result | Failure / side effect | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | CMake build target | `identity` | `tools/build_identity.py` | VERSION/Git → identity | shallow/invalid version fails | Implemented |
| 2 | CLI main / Application::updateUi / XfmdWindow::buildUi | `buildVersion` | `src/application/build/BuildVersion.cpp` | embedded text → CLI/window/status | no runtime I/O | Implemented |

## 6. Reuse and dependencies

CLI, title and right-aligned status field share the application service. CPack/man
use VERSION's major/minor. Interpreter/renderer acquire no dependency.

## 7. Verification

[Sprint 001 / P042](../../../sprints/Sprint-001--Versioning/Phase-042--Build-Identity.md)
records the seven isolated Git scenarios and CLI/window checks against build 620c7be.
[Sprint 002 / P043](../../../sprints/Sprint-002--Status-Version/Phase-043--Status-Version.md)
records build 3bb28f5 and six focused CTests, including CompactWorkspaceTest at
four window widths. The separate status label preserves link hover/messages.
These are historical identified results, not evidence for the revised display.
[2026-11-10 evidence](../../../SDP/KanBan/evidence/2026-11-10-product-version/README.md)
records the concise display, preserved JSON provenance, three focused CTests and
verified local installation under KB-XFMD-031.

## 8. Status, risks and change impact

Implemented. Merges can increase the count by more than one; parallel branches
can share counts. Published history must not be rewritten. The number is not a
global unique sequence. Main includes Sprint 001 via 4e21e99 and Sprint 002 via
c245fd9; application build metadata remains tied to the binary's actual build.
