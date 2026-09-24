# Establish the conversation-note workflow

| Field | Value |
| --- | --- |
| id | KB-XFMD-004 |
| project | XFMD |
| type | Change |
| created | 2026-09-23T23:44:59Z |
| source | Owner conversation, 2026-09-24 Europe/Oslo: XFMD audit, BoxUI/SDUI direction and KanBan notes |

## Owner instruction and scope

The owner requested on 2026-09-24 a KanBan notebook for unresolved chat topics,
using SDP's KanBan as a reference without adopting the changing SDP process.
Capture the preceding audit and current BoxUI/SDUI discussion. Document review,
consolidation, selection into active and source/successor history in a ledger.
All new/revised documentation is English. Owner: Codex for this bounded delivery.

## Completion criteria

- Self-contained Agents/KanBan board, card template, stable IDs, seven status
  directories, current index and append-only event history.
- Explicit distinction between working notes, decisions, plans and product evidence.
- Many-to-one consolidation and partial-scope handling preserve sources and record
  both source and successor IDs; no silent loss of unresolved questions.
- Register the audit, BoxUI reuse assessment and remaining SDUI widget question.
- Agent instructions and working method make capture/review part of future sessions;
  preserve the existing requirements/blueprint/sprint workflow and FOX boundary.
- Check ledger replay, metadata, placement and links, existing blueprint validators
  and diff whitespace. No product-code change or program build is needed.

## Outcome

Delivered locally on 2026-09-24. The board has seven status directories, a visible
card template, stable IDs, an index, local board metadata and a chained event ledger.
Three backlog notes preserve the audit, branch assessment and unresolved SDUI
question. They were reviewed and deliberately kept separate because their owners
or completion outcomes differ. No product follow-up has been activated.

The [board rules](../README.md) specify full and partial consolidation, bidirectional
source/successor references, changed-direction review and selection. AGENTS.md and
docs/working-method.md now connect this notebook to the existing implementation
workflow and use English. Requirements, product blueprints and implementation plans
were not rewritten as part of note capture.

Validation: both existing blueprint commands passed (37 objects, 69 requirements,
239 Implemented callee names). A one-off Python check passed for four cards,
predecessor chains, metadata, index/placement, seven directories, local source paths
and all current Markdown links in the board and revised process instructions.
Ledger placement and links were rechecked after completion. `git diff --check`
passed. These are document/structure checks, not new application or GUI test runs.
No general KanBan automation was added.

No merge, push, commit, dependency upgrade, installation or BoxUI code extraction
is included. The three backlog cards remain the input to the next work selection.
