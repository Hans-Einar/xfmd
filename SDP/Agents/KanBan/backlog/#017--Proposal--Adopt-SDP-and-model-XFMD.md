# KB-XFMD-017 — Adopt SDP and describe XFMD in SDL

| Field | Value |
| --- | --- |
| id | KB-XFMD-017 |
| project | XFMD |
| type | Proposal |
| CardState | backlog |
| created | 2026-09-25T11:02:19Z |
| source | owner-conversation-2026-09-25 |
| next_review | At the next XFMD agent planning session |

## Owner assignment and starting point

The owner requests that the next XFMD agent take the new [SDP area](../../../README.md)
into real use and create XFMD's .design sources. SDP1-M1 establishes directories
and board compatibility only. Read AGENTS.md, SDP/README.md, the phase entries,
requirements, architecture, working method, blueprint registry and current Git
status before selection. Preserve unrelated work and current product behavior.

## Scope and proposed milestones

1. Inventory requirements, use cases, features/functionality, blueprints, contracts,
   code owners, sprint plans and evidence. Produce an authority/migration map with
   stable existing IDs; separate current facts, dated proposals and unknowns.
2. Establish the mandate/stakeholder/actor context and requirement/feature links.
   Model XFMD as its own system. Identify real containers and external tool
   contracts; application/interpreter/renderer are source layers unless a runtime
   boundary is actually present. SDL, SDUI and SDPTool are separate cooperating
   systems, not owned XFMD implementations.
3. Author readable .design sources in the numbered phases: requirements/intent,
   architecture topology, per-container detail and implementation relationships.
   Keep shared contracts once and preserve UR/SR/AT/FEAT/FUNC references. If current
   SDL cannot encode a fact, retain a clearly marked Markdown design requirement;
   do not invent accepted System/import/Stakeholder syntax. Design-core 0.5 does
   not implement System; multi-file input is currently an unfinished SDP workstream.
4. Run the Go SDL toolkit to validate/format the model and generate relevant
   viewpoints. Start with context/actors-usecases, feature contributions,
   containers/channels, detailed collaboration and delivery/evidence views where
   supported by explicit facts. Generated Markdown/Mermaid/SVG must come from tools,
   not hand-written substitute viewpoints. Record commands and exact versions.
5. Reconcile maintained docs and process instructions, source destinations and
   traceability. Make one source authoritative per fact; preserve dated evidence.
   Capture outstanding language/tool gaps in the responsible board. Commit bounded
   milestones on phase branches following the existing XFMD working method.

## Acceptance

- All five phase entries point to substantive, current design or explicitly
  documented gaps; no empty claims that migration is complete.
- Existing identities, requirements, blueprints, evidence and ownership remain
  traceable; implementation status is supported by evidence, not inferred from
  parser success or diagrams.
- Design inputs parse with the identified supported Go SDL profile. Generated
  viewpoints have source provenance; current links and KanBan replay pass.
- Design detail follows actual container ownership without duplicated shared
  library/contract definitions. A0–A5 is not inferred from folder numbers.
- Remaining decisions and unsupported SDL constructs have an explicit home.

## Dependencies and exclusions

[KB-XFMD-014](%23014--Proposal--SDP-sidebar-and-generated-navigation.md) owns the
native SDP sidebar. [KB-XFMD-015](%23015--Proposal--SDL-design-file-preview.md)
owns direct .design preview. Neither feature is delivered by this adoption card.
[KB-XFMD-012](%23012--Ref--SDP--014--KanBan-format-sync.md) retains future KanBan
release alignment. FOX remains XFMD's GUI toolkit; do not switch it to Fyne.
Do not implement SDL/SDUI parsers in XFMD or edit the producer repositories from
this task without separate selection. Existing pipeline guidance:

file:///home/warloc/git/SDP-vNow/Toolkit/SDPTool/Navigation-and-Design-Preview.md

## Worklog

- Registered on 2026-09-25 for another XFMD agent. CardState remains backlog.
  No XFMD .design files or generated model viewpoints have been delivered yet.
