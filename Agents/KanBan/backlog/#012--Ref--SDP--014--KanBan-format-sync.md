# Synchronize XFMD cards with the shared KanBan contract

| Field | Value |
| --- | --- |
| id | KB-XFMD-012 |
| project | XFMD |
| type | Ref |
| created | 2026-09-24T17:40:43Z |
| source | Owner conversation, 2026-09-24 Europe/Oslo: keep XFMD backlog cards compatible with SDP KanBan |
| next_review | When SDP defines or changes its KanBan contract/template, and before adopting the next shared KanBan capability in XFMD |
| primary | KB-SDP-014 |
| tags | process, kanban, compatibility |

## Need and owner direction

Keep XFMD's KanBan card format up to date so its backlog supports functionality
defined by the shared KanBan workflow. XFMD borrowed KanBan independently of the
unfinished SDP restructuring; this request is about synchronizing that borrowed
system, not adopting the rest of SDP.

Primary: KB-SDP-014 — KanBan version contract and distribution

file:///home/warloc/git/SDP-vNow/SDP/Agents/KanBan/backlog/%23014--Proposal--KanBan-version-contract-and-distribution.md

This is a
direct reference to the SDP-owned proposal, with a separate XFMD adaptation outcome.
Completing this Ref does not complete the shared contract or its migration.

## Current evidence and upstream direction

The current upstream sources are the SDP KanBan workflow, card template,
history/worklog rules and merge/split lineage:

file:///home/warloc/git/SDP-vNow/SDP/Agents/KanBan/README.md

file:///home/warloc/git/SDP-vNow/SDP/Agents/KanBan/Card-template.md

file:///home/warloc/git/SDP-vNow/SDP/Agents/KanBan/History.md

file:///home/warloc/git/SDP-vNow/SDP/Agents/KanBan/Lineage.md

XFMD's [local template](../Card-template.md) and [workflow](../README.md) use
visible metadata and payload 0.1, but do not yet provide the same explicit worklog
section or typed payload-0.2 lineage contract. Record these as gaps to evaluate;
do not claim the local cards are already compatible with every upstream capability.

The owner notes that proper KanBan version contracts are not yet established.
Existing upstream schema labels are partial definitions, not a settled overall
consumer contract. Until one is published, identify the inspected upstream
revision and distinguish current rules from proposals.

The intended future shared location is `SDP-vNow/KanBan/`, with templates under
`SDP-vNow/Template/sdp-root/Agents/KanBan/`. These are planned locations, not
available dependencies. Follow KB-SDP-014 for migration and update these references
when the canonical source changes.

## Agent recommendations and local scope

- Compare the local template, existing cards, ledger and workflow against the
  agreed upstream version/capabilities: metadata, stable IDs, worklogs/revisions,
  Refs, merge/split lineage, review/completion rules and validation.
- Record the supported upstream baseline and any deliberate local differences.
  Select and document the relevant adaptation once the shared contract is clear;
  do not invent local version fields as if SDP had already standardized them.
- Update the card template and instructions for future cards, and plan a bounded
  migration of existing cards across statuses where compatibility requires it.
  Preserve owner decisions, evidence, identities, status and historical ledger
  bytes. Do not fabricate old worklogs, events or missing revision evidence.
- Keep XFMD's standalone `Agents/KanBan/` location and its existing implementation
  workflow. Do not introduce `SDP/`, switch product process or copy real SDP cards
  into the local board. This is not part of the active UI implementation scope.
- Verify metadata, links/Refs, ledger schemas and replay, physical/index agreement
  and retained history using the agreed contract. Add only selected functionality;
  referencing future graph/tooling needs does not implement those tools.

## Related-card review

[KB-XFMD-004](../completed/%23004--Change--Conversation-note-workflow.md) records
the initial adaptation and remains completed. This Ref captures later upstream
alignment; it does not reopen that delivery or alter KB-XFMD-006–011 UI work.
No existing local card is superseded or partially consolidated.

## Next review and completion criteria

Review on the upstream contract/template change trigger above. At selection,
record the upstream revision/version, capability-gap matrix, bounded migration
scope and next review trigger for later releases. Complete the local adaptation
only with documented compatibility checks, preserved history and explicit
remaining differences; upstream publication alone is not completion of this Ref.

## Outcome and references

Registered in backlog. No local card-format migration or ledger upgrade performed.
Upstream ownership and future paths are retained in KB-SDP-014; this card keeps the
XFMD consumer follow-up visible independently of the ongoing product work.
