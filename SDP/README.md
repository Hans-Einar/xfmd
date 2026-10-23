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

[KanBan](KanBan/README.md), [Traceability](Traceability/README.md),
[Verification](Verification/README.md) and [bootstrap evidence](Maintenance/SDP1/Plan-and-Evidence.md)
keep distinct responsibilities. Phase homes do not assign A0–A5 to every object.
The process can revisit earlier decisions; it does not require a waterfall.

Bootstrap created navigation to current authorities, not a second specification.
[KB-XFMD-017](KanBan/active/%23017--Proposal--Adopt-SDP-and-model-XFMD.md) is now
active under [MAINT-XFMD-0004](Maintenance/MAINT-XFMD-0004.md). A
[partial SDL/SDUI navigation pilot](SDL/XFMD/README.md) has checked structural
sources and generated static views. It is not a complete System model or native
SDUI runtime integration. Native SDP sidebar and direct .design preview remain
separate cards. No complete SDL source set is registered in SDPTool yet.

The latest 2026-09-29 owner refinement selects complete SDL modeling and SDUI
composition wherever feasible. The SDL baseline now focuses on functionality, ownership and
dependency boundaries to keep feature growth maintainable. SDUI may use labeled editor/preview regions without active file editing; see the MaintenancePlan for current acceptance.

The signed Toolkit 1.0.0 installation is recorded in
[MAINT-XFMD-0003](Maintenance/MAINT-XFMD-0003.md); the installed manifest controls
current version claims. Earlier [installation](Maintenance/MAINT-XFMD-0001.md)
and [reconciliation](Maintenance/MAINT-XFMD-0002.md) retain their historical scope.

Current model checkpoint (2026-09-30): [XfmdDesktop model](SDL/XFMD/README.md)
now covers all 37 existing blueprints structurally and includes 22 declared
workflow examples plus five feasible SDUI concepts. Full semantic closure and
independent review remain pending. The earlier pilot remains historical evidence.
