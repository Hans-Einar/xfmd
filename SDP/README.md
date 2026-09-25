# XFMD — System Design Process

Owner adoption: 2026-09-25. XFMD is its own software system with its own SDP
process area. SDL, SDUI and SDPTool are collaborating systems maintained in
SDP-vNow's shared process area. This does not transfer XFMD implementation
ownership to those projects or turn its library layers into runtime containers.

| Process home | Current entry |
| --- | --- |
| 01--Mandate | [Owner assignment](01--Mandate/Mandate.md) |
| 02--Requirements | [Existing needs and acceptance](02--Requirements/README.md) |
| 03--Architecture | [System and container boundaries](03--Architecture/README.md) |
| 04--Design | [Existing detailed design](04--Design/README.md) |
| 05--Implementation | [Plans and adoption handoff](05--Implementation/README.md) |

[KanBan](Agents/KanBan/README.md), [Traceability](Traceability/README.md),
[Verification](Verification/README.md) and [bootstrap evidence](Maintenance/SDP1/Plan-and-Evidence.md)
keep distinct responsibilities. Phase homes do not assign A0–A5 to every object.
The process can revisit earlier decisions; it does not require a waterfall.

Bootstrap created navigation to current authorities, not a second specification.
No .design model has been created or claimed parsed yet. The next agent starts
with [KB-XFMD-017](Agents/KanBan/backlog/%23017--Proposal--Adopt-SDP-and-model-XFMD.md).
Native project detection/sidebar and .design preview remain separate cards.
No automatic SDPTool discovery marker or installer version is invented here.
