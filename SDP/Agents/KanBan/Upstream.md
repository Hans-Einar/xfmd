# SDP KanBan — selected upstream baseline

Adopted by owner instruction on 2026-09-25. Upstream repository: Hans-Einar/SDP,
commit `1a3f5e88364ee219b063dd4615fac19ced33d89b`. Source area: SDP/Agents/KanBan.
This is a pinned consumer adaptation, not a newly released overall KanBan version.
[KB-XFMD-012](backlog/%23012--Ref--SDP--014--KanBan-format-sync.md) tracks future
shared compatibility/distribution decisions owned by KB-SDP-014.

| Contract part | Supported baseline |
| --- | --- |
| Board metadata | 0.1; existing board.json retained |
| Event envelope | 1.0; local schema copy |
| Historical ledger payload | 0.1; old bytes retained |
| New ledger payload | 0.2, including typed merge/split lineage |
| Card format | Visible Markdown metadata with exactly one CardState row |
| Work lifecycle | Ready/in-progress/gate-review, queued backlog, worklogs and Git revisions |
| Process location | SDP/Agents/KanBan under XFMD's own SDP area |

Local adaptations: XFMD index/cards retained; workflow links point to local
schemas and documentation; History examples use XFMD paths; lineage checker
loads the local envelope. No upstream production cards or historical ledger
are copied. Project requirements, blueprints and sprint records keep authority
until explicitly reconciled in KB-XFMD-017. A folder alone does not implement
SDPTool discovery or make native XFMD expose an SDP sidebar.

## Upstream source fingerprints

SHA-256 identifies source bytes before local link/example adaptation.

| Source | SHA-256 |
| --- | --- |
| Card-template.md | `5e4b60f85e7567e6623d4030dd65c39a45cf6579f223baf2d183cae63767c383` |
| Lineage.md | `0d6aaadc6793a16203dacc2fecd91b4fbefb8e7e9cddba0384822175c1342afa` |
| ledger-payload.schema.json | `45f7776f6aff35fccc5599d844447497d8602c5c8323ecf7d762f37f53a097b2` |
| ledger-payload-0.2.schema.json | `db7dbba2c781461350e651b982ca2cef0ba1b56bbb4d56a45fa0cbf8952cd0bd` |
| examples/lineage.ndjson | `17eecbcffe0fe63a6afb73d9a16a6f791b22fcef0bda7b7113046cf33923681a` |
| examples/verify_lineage.py | `614c517d03c9f3602dffbd9282eecef36523b7b54308bf151a81d993f2180b30` |
| History.md | `3382d3f3e8a4a697009952ce216810e1070f5cfdf1ec95742066eb8a79f215f1` |
| README.md | `9e2db5d1ffd8efac15d977c8c324a6eb6c159af6e6b5f20cff1eb91c1015db97` |
| Toolkit/schemas/ledger-event.schema.json | `68cfc05bafd2308a3c03db726581b3049a0e7085ea0228535570e776d77a2bdc` |
