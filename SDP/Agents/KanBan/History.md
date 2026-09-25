# Worklogs, revisions and diffs for KanBan cards

A card is a working document while active. Keep one file with a stable ID through
its lifecycle. Git owns content revisions; the KanBan ledger owns processing
events; Traceability owns implementation and verification. This is K4's manual
workflow, not a new version-control system or an implemented sdptool command.

## While a card is active

The opening sections describe current needs, agreed scope, owner, next action and
completion criteria. Add a concise worklog with time, actor, event ID, result and
evidence. Update it for important findings, scope changes, decisions, checks and
milestones even when status does not change. It is not a chat transcript.

- Distinguish observations, agent proposals and owner decisions; name the decider.
- Use `x-kanban:reviewed` for substantive processing without a move, preserving
  path/status and linking the outcome. Group related small edits where sensible;
  spelling and link repairs do not each need an event.
- Update current text with new knowledge and explain replacements in a new log
  entry. Do not remove earlier rationale or rewrite ledger history.
- State remaining work at each milestone. A finished subtask does not complete
  the entire card. Preserve the original request and separate delivered/planned work.
- Commit card, related documents, index and ledger together at milestones. Include
  both milestone ID and card ID in the commit. Between milestones, local drafts
  are not durable Git snapshots; Git cannot show revisions never committed.

The worklog is a readable summary. Correct earlier entries through additions.
Do not create snapshot copies of cards in a revision directory. Do not insert a
commit's own hash into its contents. `commit: null` remains valid in the ledger:
the Git commit introducing its unique event ID identifies the revision. Later
log entries may refer to already existing commits.

## Maintenance and active cards

`SDP/Maintenance/<phase>/` may own a coherent phase plan, inventory and larger
verification evidence. The card links there and summarizes progress, next steps
and remaining work. A reader should understand its situation without searching
random notes. Do not duplicate whole plans or raw test logs inside the card.
Preserve cards and evidence on closure. Maintenance is a delivery location,
not another mandatory SDP phase or abstraction level.

## Inspect history with Git today

Run from the repository root, substituting the current card address:

```sh
git log --follow -p -- 'SDP/Agents/KanBan/backlog/#017--Proposal--Adopt-SDP-and-model-XFMD.md'
git diff -- 'SDP/Agents/KanBan/backlog/#017--Proposal--Adopt-SDP-and-model-XFMD.md'
git diff --cached -- 'SDP/Agents/KanBan/backlog/#017--Proposal--Adopt-SDP-and-model-XFMD.md'
```

For historical revisions use the original commit:path from Git and ledger
history. [The migration record](../../Maintenance/SDP1/Plan-and-Evidence.md)
identifies the move from Agents/KanBan to SDP/Agents/KanBan.

`--follow` infers renames for one file. It is useful for ordinary moves, but is
not authoritative for card identity and does not follow semantic merge/split.
When uncertain, obtain historical paths from the ledger and compare exact
`commit:path` blobs. If the board itself moved, consult the repository migration map.

## Multiple sources, repositories and incomplete history

[Lineage](Lineage.md) governs merge/split. New targets and preserved sources have
separate revisions. A lifecycle view must show their branches, rather than pretend
there was one file. Partial transfer leaves the source's worklog active.

A future tool must resolve the project's Git repository through project registration,
then the stable card ID, ledger path and revision. Pair repository identity with
commit hashes; a hash alone is not a cross-repository address. Refs link the
primary card's history while retaining their own local processing history.

Label local drafts, unavailable repositories, shallow clones, missing objects and
broken history explicitly. Never reconstruct content from mtime or present a
missing revision as empty. A Markdown-only export includes the worklog, not full
Git history. Rebase/squash may change commit identities; preserve the owner's
phase/milestone commits. Card/event IDs remain stable.

Integrated native KanBan navigation remains in
[KB-XFMD-014](backlog/%23014--Proposal--SDP-sidebar-and-generated-navigation.md).
This bootstrap does not implement a history/diff UI.
