# XFMD — SDP KanBan

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
| KB-XFMD-016 | Change | completed | [Initialize SDP and align KanBan](completed/%23016--Change--SDP-process-bootstrap.md) |
| KB-XFMD-017 | Proposal | backlog | [Adopt SDP and describe XFMD in SDL](backlog/%23017--Proposal--Adopt-SDP-and-model-XFMD.md) |
| KB-XFMD-018 | Change | completed | [Sidebar favorites](completed/%23018--Change--Sidebar-favorites.md) |

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

Capture ideas, questions and requested changes from conversations as cards, even
when details remain unresolved. Registration does not adopt a language rule or
authorize implementation. Distinguish owner decisions, agent recommendations and
open choices. Do not re-register completed product phases as new requests.

This is XFMD's local SDP board. It works without an SDP-vNow checkout.
[Upstream baseline and compatibility](Upstream.md) identifies the selected
SDP rules, schema versions and local adaptations. The owner adopted this
location and workflow on 2026-09-25. The older note-only restriction is superseded.

**All card and workflow documentation must be English**, including metadata
values, worklogs, queue explanations and new event descriptions. Preserve old
append-only event bytes and clearly identified verbatim source quotations.

## Types and tags

The primary `type` appears in the filename. Optional `tags` may identify topics
such as tooling, language, process or visualization. Type is not priority or status.

| Type | Purpose |
| --- | --- |
| Idea | A possibility to evaluate; no finished solution required |
| Proposal | A concrete proposal with purpose, open choices and acceptance criteria |
| Question | A clarification requiring a recorded answer |
| Study | A bounded investigation with questions and expected outcomes |
| Change | An authorized change with known scope |
| Bug | An observed deviation with expected behavior and reproduction |
| Decision | Decision, decision-maker, date, rationale and consequences; the type alone is not approval |
| Ref | Local impact and a link to one primary card in another project |

Use UserStory and other model concepts in SDL only when its profile supports
them. KanBan types are not new SDL keywords. Add types only for a concrete need.

## Identity and document format

Example filename: `#003--Idea--KanBan-graph.md`; reference card:
`#001--Ref--SDP--002--sdptool.md`. Allocate increasing numbers per project across
all statuses/types, with at least three digits. Never reuse closed/deleted IDs.
The stable ID is `KB-<PROJECT>-<number>`; filenames and locations may change.
Check every directory and ledger before allocation. Resolve parallel ID/event
collisions before committing; never overwrite another registration.

Use the [card template](Card-template.md). Metadata is a visible Markdown table
immediately below the title, with `Field` and `Value` columns. Keep field names
`id`, `project`, `type`, `created`, `source` and optional `next_review`, `primary`
and `tags`, plus mandatory `CardState`. Do not duplicate metadata in YAML frontmatter. The ledger remains
JSON and owns event history.

Each primary card owns one coherent need. A Ref has its own ID/status, `primary`
containing the primary card ID, a clickable link and local impact. Link directly
to the primary card, never through a Ref chain. Prefer the nearest responsible
project, but do not duplicate a need merely to obtain perfect placement.
Completing a Ref does not complete its primary card. Link implementation slices
back to the card where needed. Include card IDs in related issues/PRs; GitHub
status does not automatically change local KanBan status.

## Lifecycle directories

| Directory | Meaning and requirements |
| --- | --- |
| backlog | Registered, awaiting prioritization or clarification; specify the next review |
| active | Selected, bounded work; specify scope, owner and completion criteria |
| onHold | Blocked or deferred; specify reason, restart condition and review date |
| completed | Agreed outcome achieved, with linked decision/delivery/evidence |
| canceled | Previously relevant or selected work deliberately stopped; explain why |
| superseded | Fully replaced, merged or split; name and link every successor |
| irrelevant | Reviewed as outside scope or no longer relevant; explain why |

Names are case-sensitive: use `superseded`, not `superseeded`. An Idea, Question
or Study can complete when its agreed evaluation is delivered; that does not
mean the proposed product feature is implemented. Implementation cards require
actual implementation and agreed verification, not just a plan. Link authorized
follow-up work and obtain implementation status from Traceability.

Any status may be reopened with a reason. Do not delete cards to empty backlog.
Move duplicates to superseded with a primary-card link. Follow [Lineage](Lineage.md)
for merge/split: new targets, preserved sources and explicit remaining work.
Only fully replaced sources close. Review related cards before selecting work.

## Work rhythm and scope

Use active cards as working documents, with current scope, next action and a
worklog. [History and diff](History.md) connects log entries, ledger events and
Git revisions, including reviews without a status change. Git stores content;
KanBan stores processing history; Traceability stores implementation evidence.

At startup, read the board and affected Refs, select bounded work and activate
it with explicit scope. Record scope-changing discoveries before changing focus.
At milestones, update outcomes, ledger and references. Review backlog/onHold
before the next phase and at the agreed review date. Decide the next work,
defer with a new date, cancel, supersede or mark irrelevant. The initial
2026-09-30 review date is not a delivery deadline. Age alone does not justify
deleting or rejecting an idea. Aim for a small, well-understood backlog.

## Moving cards — manual workflow

1. Read the card and its latest event; note ID, state and old path.
2. Record reason/outcome, successors if any, plan and evidence in the card.
3. Move the whole file, preserving its ID and normally its filename.
4. Append an event with actual time, actor and old/new status and paths.
5. Update the index and incoming Markdown links across registered boards. Search
   both stable ID and old filename; encode `#` as `%23` in URL targets. Do not
   rewrite historical ledger paths.
6. Verify links, replay and physical placement. Commit the document, ledger and
   indexes together. Investigate mismatches; do not infer state from mtime.

The durable reference is the card ID; its Markdown link is its current address.
Moves within one board preserve relative depth. For cross-project transfer,
retain the original primary card/ID and create a Ref for now; no general
transfer/ID-migration contract has been implemented.

## Ledger contract — payload 0.1 and 0.2

Each board has [board.json](board.json) and append-only Ledger.ndjson, one JSON
event per line. Use the [SDP envelope](ledger-event.schema.json)
with schemaVersion 1.0. New events use [payload 0.2](ledger-payload-0.2.schema.json);
historical events retain [0.1](ledger-payload.schema.json). This does not change
the Toolkit envelope or the implementation ledger in Traceability.

- `eventId`: unique, increasing `EVT-KB-<PROJECT>-<number>` within the board.
- `eventType`: `x-kanban:created`, `x-kanban:moved` or `x-kanban:reviewed`.
- `subjectId`: stable card ID. `occurredAt`: actual RFC3339 timestamp with timezone
  (UTC here). `actor`: recorder. `commit` may be null; the introducing Git commit
  records the event. Never rewrite an event to add its own commit hash later.
- Payload: `schemaVersion`, `projectId`, `previousEventId` for the same card,
  `from`, `to`, `fromPath`, `toPath`, `reason`, `links`. Version 0.2 also permits
  typed `lineage`; see [its completeness rules](Lineage.md).
- Paths are literal UTF-8 board-relative paths, with no `..` or absolute prefix,
  not URLs. `links` contains stable card/slice IDs or board-relative evidence paths.
- created: first event; previous event/status/path are null. Start in backlog or
  active. Do not invent an earlier history.
- moved: predecessor matches the latest event; origin matches previous status/path;
  target represents a real move or rename.
- reviewed: same status/path, with outcome/reason and any new review date in the
  card. Correct state through a new explained event, not by rewriting history.
- completed/superseded transitions require an outcome/successor reference in
  `links`. Superseded must name every successor, each an existing primary card.

Replay follows line order and the predecessor chain, not timestamps alone.
Resolve new, unintegrated event-ID collisions before merging. The last event
must match exactly one physical card at the recorded location. Schema validation
alone does not prove chain rules, references, status transitions or placement.
No interactive graph is delivered. Run the local lineage examples and
[board check](verify_board.py) for contract, replay, card-state and placement checks.

## CardState — current work state

The card's visible metadata table owns **one** `CardState`. No gate file, queue
folder, duplicate YAML field or state column in this index. Directory/ledger
still own lifecycle placement/history; CardState refines work within a stage.

| CardState | Directory | Meaning |
| --- | --- | --- |
| backlog | backlog | Registered, not selected next |
| queued | backlog | Proposed next bounded work; add a Queue section with reason, predecessor, prerequisites and next step |
| ready | active | Selected, currently awaiting work |
| in-progress | active | An agent/person is working on the bounded scope |
| gate-review | active | Concrete result awaits owner review; explain the decision and evidence |
| onHold | onHold | Deferred/blocked, with restart condition and review date |
| completed | completed | Agreed outcome delivered |
| canceled | canceled | Deliberately stopped |
| superseded | superseded | Fully replaced, with successors |
| irrelevant | irrelevant | Reviewed as outside scope/no longer relevant |

Set in-progress when starting; return to ready only with a recorded pause/handoff.
Use gate-review when review is required, not as a synonym for unresolved future
implementation. Owner acceptance leads to completed; requested changes return to
in-progress. Do not infer acceptance from elapsed time. Existing explicitly
accepted or objectively authorized deliveries may close with evidence.

Prefer at most one queued primary card per board. A Queue section explains why
it is next and its prerequisites; the predecessor's Next selection section records
why it selected that card. Queued is prioritization, not automatic authorization.
Do not keep a completed predecessor active merely because it has a successor.

For state-only changes, update metadata/worklog and append a reviewed event whose
English reason records old → new state. For moves, update folder/state/index/links
and append moved. No new ledger schema or duplicate authoritative state is needed.
Git preserves exact document revisions; ledger events explain transitions.

For a quick text search: `rg '^\| CardState \|' backlog active`.
The separately installed `kanban status` command may list the board; it is not
required for this self-contained workflow. No new CLI is installed by the bootstrap.
