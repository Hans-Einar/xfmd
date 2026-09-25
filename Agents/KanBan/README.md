# XFMD conversation board

## Current notes

| ID | Type | Status | Topic |
| --- | --- | --- | --- |
| KB-XFMD-001 | Proposal | completed | [Documentation audit and reconciliation](completed/%23001--Proposal--Documentation-reconciliation.md) |
| KB-XFMD-002 | Study | completed | [BoxUI branch disposition and reusable changes](completed/%23002--Study--BoxUI-branch-disposition.md) |
| KB-XFMD-003 | Question | backlog | [SDUI widget ownership and XFMD's boundary](backlog/%23003--Question--SDUI-widget-ownership.md) |
| KB-XFMD-004 | Change | completed | [Establish the conversation-note workflow](completed/%23004--Change--Conversation-note-workflow.md) |
| KB-XFMD-005 | Study | completed | [Remaining SDL acceptance evidence](completed/%23005--Study--SDL-acceptance-evidence.md) |
| KB-XFMD-006 | Change | completed | [Open documents and folders from the same dialog](completed/%23006--Change--Open-files-and-folders.md) |
| KB-XFMD-007 | Change | completed | [Open plain text internally and route other targets consistently](completed/%23007--Change--File-types-and-external-opening.md) |
| KB-XFMD-008 | Change | completed | [Move the document path into a full-width navigation and filter row](completed/%23008--Change--Document-path-and-file-filter.md) |
| KB-XFMD-009 | Change | completed | [Compact the sidebar and tab recent folders and files](completed/%23009--Change--Sidebar-controls-and-recent-tabs.md) |
| KB-XFMD-010 | Change | completed | [Provide normal zoom gestures and percentage controls in Wrap and A4](completed/%23010--Change--Document-zoom-controls.md) |
| KB-XFMD-011 | Change | completed | [Regroup toolbar actions and hide reading-color sliders in the theme popup](completed/%23011--Change--Toolbar-layout-and-color-popup.md) |
| KB-XFMD-012 | Ref | backlog | [Synchronize XFMD cards with the shared KanBan contract](backlog/%23012--Ref--SDP--014--KanBan-format-sync.md) |
| KB-XFMD-013 | Change | completed | [Compact Files header and single Markdown filter](completed/%23013--Change--Sidebar-header-markdown-filter.md) |
| KB-XFMD-014 | Proposal | backlog | [SDP sidebar and on-demand generated navigation](backlog/%23014--Proposal--SDP-sidebar-and-generated-navigation.md) |
| KB-XFMD-015 | Proposal | backlog | [SDL design-file source and diagram preview](backlog/%23015--Proposal--SDL-design-file-preview.md) |

This index shows current location; [Ledger.ndjson](Ledger.ndjson) records history.
Read relevant notes at session start. Review backlog after a topic discussion and
before selecting the next round. KB-XFMD-001 completed its bounded documentation reconciliation. KB-XFMD-002 completed isolated tooling extraction and branch assessment.
KB-XFMD-005 completed its identified SDL acceptance evidence and records residual
limits in P052. KB-XFMD-007 completed P053 target routing. KB-XFMD-006/008/009/011
completed P054 workspace integration with documented checks. KB-XFMD-010 completed
P055 shared zoom; KB-XFMD-003 remains deferred. KB-XFMD-012 is the separate
concurrently captured process Ref and does not expand the selected UI goal.


## UI wishlist review — 2026-09-24

Owner items 1–7 are captured in KB-XFMD-006–011, with
[source mapping and the supplied screenshots](evidence/2026-09-24-ui-wishlist/README.md).
Each card separates owner direction, current source evidence, agent recommendations
and unresolved details. Recording these Changes is not implementation selection.

Overlap review: existing KB-XFMD-003 concerns SDUI widget ownership and remains
separate; an SDUI description is optional later work, not a dependency. KB-XFMD-005
concerns SDL acceptance evidence, is selected by another agent, and remains separate. Completed KB-XFMD-002's
BoxUI/tooling disposition is not reopened by this FOX UI request. No existing
card is replaced, moved or partially consolidated by this capture.

Suggested planning order (agent recommendation): agree shared opening/root policy
in 006/007 and automatic path/filter interpretation in 008; coordinate the row/sidebar/toolbar
placement across 008/009/011; define and deliver zoom in 010 with its controls in
011. A single selected sprint may group these cards, but this capture creates no
sprint, product requirements or implementation branch. The implementation agent
must review the current board/working tree, resolve selected interaction details,
and follow requirements → blueprint → contracts/plumbing → code → verification.

## Purpose and authority

This is our working notebook for questions, findings, ideas and follow-up work from
conversations. Capture incomplete notes during discussion without forcing them into
XFMD's product documentation or implementation plan. Separate owner instructions,
observed evidence, agent recommendations and open questions. A card is not product
truth, implementation authorization, or proof that a feature works.

Requested on 2026-09-24 (Europe/Oslo), this adapts only the KanBan conventions
inspected in SDP-vNow at `321e193258966bea049ea02178192404f8daad99`: stable IDs,
visible Markdown metadata, status directories and an append-only ledger. It imports
no other SDP procedures, schemas, tooling or directory layout. The board works
without the SDP checkout. Reference location:

file:///home/warloc/git/SDP-vNow/SDP/Agents/KanBan/README.md

All new/revised documentation and ledger prose is English. Preserve dated evidence
and identify corrections rather than silently rewriting past decisions.

## Cards and identity

Use [Card-template.md](Card-template.md). Metadata is a visible Markdown table,
not YAML. Required fields: `id`, `project`, `type`, `created`, `source`, plus
`next_review` for unresolved cards. Optional fields include `tags`, `primary`,
`owner`, `sources` and `superseded_by`. Directory and ledger determine status.

- IDs: `KB-XFMD-NNN`; filenames: `#NNN--Type--Topic.md`. Allocate across all status
  directories and ledger events. Never reuse an ID or renumber a published card.
- Types: `Idea`, `Proposal`, `Question`, `Study`, `Change`, `Bug`, `Decision`, `Ref`.
  Type describes content, not approval, priority or status.
- Keep one coherent topic per card. Update the existing note for the same topic;
  create another when scope or ownership is meaningfully different.
- Record the conversation date/topic or issue reference, not an invented chat URL.
  `created` is actual registration time; do not backdate reconstructed notes.
- A `Ref` has its own ID and links directly to an existing primary card with its
  stable ID and local impact. Do not create a Ref chain or invent a foreign card.
- Branch findings include repository, branch and commit. Distinguish committed
  evidence, local unversioned artifacts and unverified recollection.
- Encode filename `#` as `%23` in Markdown link targets. IDs survive path changes.

## Status directories

| Directory | Meaning |
| --- | --- |
| backlog | Recorded for discussion/selection; specify the next review trigger or date. |
| active | Selected, bounded work; specify owner, scope and completion criteria. |
| onHold | Deferred/blocked; record reason, resumption trigger and next review. |
| completed | The agreed outcome is achieved, with decision/delivery/evidence reference. |
| canceled | Previously relevant work deliberately stopped; retain the reason. |
| superseded | Replaced/consolidated; link the successor ID and card. |
| irrelevant | Reviewed as outside current needs/scope; retain the reason. |

An answered Question or delivered Study can be completed without implementation.
A Change promising implementation cannot be completed merely because a plan exists.
Reopening requires a reason and new event. Age never closes a card automatically.
Do not delete notes to clear the board or infer status from GitHub PR state.

## Discussion, review and selection

1. Capture unresolved topics during chat with enough context to resume without the
   transcript. Do not copy every message.
2. At a discussion boundary, read related backlog notes. Check overlap,
   contradictions, later decisions and project ownership. Record what remains
   current, what is superseded, and what still needs deciding.
3. Keep separate notes for separate decisions. Consolidate when a shared scope
   improves the next piece of work. Record substantive review with a `reviewed`
   event, including the reason for keeping or changing scope.
4. Select work already authorized by the owner, or leave it in backlog. Selection
   can activate an existing card or a new synthesis of several cards.
5. When work becomes an implementation delivery, link its requirements, blueprints
   and sprint/phase plan and follow XFMD's existing working method. Do not turn the
   board into a second product specification or duplicate plans across source cards.
6. Record actual outcome, checks and limitations when completing work. Update
   the card, location, ledger and index together.

## Consolidation and changed direction

For a many-to-one consolidation, create a new card with a new ID and a source table:
source ID, retained information, superseded statements, unresolved remainder and
reason. Link every source card. A synthesis is not approval of all source ideas.

If its scope is already selected, create the synthesis in `active`; otherwise use
`backlog`. Append its `created` event with source IDs in `payload.links`. Preserve
useful facts, decisions and evidence before closing sources. Move fully replaced
sources to `superseded`, add `superseded_by` and a successor link, then append one
`moved` event per source linking the successor ID. Update incoming links and index.
This records both directions without a special event type.

For partial consolidation, keep the source open with explicit remaining scope
(or create a linked residual card). Append a `reviewed` event naming the successor
and transferred scope. Never supersede a whole card while dropping unresolved
questions. Use the same source table for splits.

Retain dated conflicting statements and identify the later decision, date and
authority replacing them. Agent recommendations are not owner decisions. Record
corrections as new reviews; never rewrite old ledger events.

## Ledger contract 0.1

[board.json](board.json) identifies the project and ledger. Each line of
`Ledger.ndjson` is a JSON object with:

- `schemaVersion`: `1.0`; `eventId`: `EVT-KB-XFMD-NNNNNN`, unique and increasing.
- `eventType`: `x-kanban:created`, `x-kanban:moved`, or `x-kanban:reviewed`.
- `subjectId`: card ID; `occurredAt`: actual RFC3339 time with timezone;
  `actor`: recorder; `commit`: null at registration. Git records the introducing
  commit; do not later amend the event to insert its own commit hash.
- `payload`: `schemaVersion` (`0.1`), `projectId` (`XFMD`), `previousEventId`
  (same card), `from`, `to`, `fromPath`, `toPath`, `reason`, `links`.
- Paths are literal UTF-8 paths relative to this board, without traversal or
  absolute prefixes. `links` contains stable card IDs or board-relative outcome
  paths. Card bodies can link to product documentation and external evidence.

`created` starts in backlog or active: previous event/status/path are null.
`moved` references the latest event and exact old status/path, then the new state.
`reviewed` preserves status/path and records substantive review or correction.
Completion and supersession require outcome/successor references; supersession
must name an existing successor card. Old paths remain historical data.

Replay follows line order and per-card predecessor chains. The latest event must
match exactly one physical card, its ID, directory and index entry. Move the card,
append the event and update incoming links/index in one working change. Check IDs
before allocating in parallel branches; resolve unintegrated collisions before
merging without altering published event history.

No automation is introduced. For each board change, check JSON parsing, unique IDs,
predecessor chains, from/to states, metadata, physical placement, successor links
and current Markdown links. Run `git diff --check`. Do not rewrite historical paths
or check them as if they were current document links.

## Active goal — 2026-09-24

The owner authorized implementing and verifying all six UI Changes, KB-XFMD-006–011.
This supersedes the capture-only status in the dated review above. Sprint 007
tracks the delivery; individual cards keep their identity and completion evidence.
SDUI widget ownership (KB-XFMD-003) is excluded.
