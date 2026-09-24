# Move the document path into a full-width navigation and filter row

| Field | Value |
| --- | --- |
| id | KB-XFMD-008 |
| project | XFMD |
| type | Change |
| created | 2026-09-24T18:01:41+02:00 |
| source | Owner conversation, 2026-09-24 Europe/Oslo: XFMD UI wishlist; Owner item 3 |
| next_review | Before selecting the next XFMD UI work round; resolve this card's named interaction questions during requirements/blueprint planning |

## Need and owner direction

Move the current document label out of the menu/icon toolbar into its own row
immediately below. The row spans the whole XFMD window, with the field expanding
to fill available width and room for buttons on either side.

- At rest, show the full absolute path of the currently open document.
- Left-click makes the field editable. Accept an absolute document path or a
  relative path resolved from the folder XFMD is currently navigated to (work
  path), not from the current document's directory or process working directory.
- Right-click copies the open document's path to the clipboard.
- The same field also performs the filename filtering currently in Files. Remove
  the duplicate sidebar search field once that function is available here.

Example of the required distinction: with work path `/work/project` and open
file `/notes/todo.md`, entering `README.md` addresses `/work/project/README.md`.
A relative Markdown link in `todo.md` still resolves from `/notes`.

## Current evidence and affected scope

The current control is an `FXLabel`, not an editable path input.
`XfmdWindow::setDocumentLabel` displays the basename (plus dirty marker), puts the
full path in a tooltip and hides the label at narrow widths. `WorkspacePanel`
owns the separate filter field and its delayed application to the tree.
See [XfmdToolbar.cpp](../../../src/application/ui/XfmdToolbar.cpp),
[WorkspacePanel.cpp](../../../src/application/ui/WorkspacePanel.cpp) and
[screenshot references](../evidence/2026-09-24-ui-wishlist/README.md).

Review UR-014/032, the dialog-root change in KB-XFMD-006 and
[FUNC-010](../../../src/blueprint/functionality/Functionality-010--Workspace-Controls.md)/
[FUNC-013](../../../src/blueprint/functionality/Functionality-013--Filtered-File-Tree.md).
Existing filename substring/wildcard and `.md`/`.txt` filter combination semantics
should be reused, not replaced with full-document search.

Owner clarification in the same conversation: automatically interpret what is
typed. Do not add a separate Path/Filter mode toggle. This replaces the agent
suggestion of an explicit mode button; no exact classification algorithm has
yet been selected.

## Agent recommendations and open questions

- Automatic interpretation must handle `notes.md` being both a relative path
  and a useful filter. Recommended interaction: typing updates the filename
  filter; Enter opens an exact, valid file path (absolute or work-path-relative).
  Do not open files merely while typing or choose an arbitrary partial match.
  This algorithm is an agent recommendation, not an owner-approved rule.
- Define precedence for a real filename containing `*`/`?`, a non-existing typed
  path, multiple filter matches and directory input. Give visible feedback when
  Enter cannot resolve a target; do not silently open an unintended result.
- Recommend Escape to abandon editing/restore the current document path. Define
  when the display returns from filter text to document path, whether the previous
  filter is restored on Escape, and whether filtering survives a document change.
  The resting path must remain discoverable without a separate mode selector.
- Right-click should copy the committed document path, not a half-edited path,
  filter text or dirty marker. Clarify its behavior while actively editing or
  filtering and show brief feedback after copying.
- Define the empty/unsaved/generated-document case without inventing a filesystem
  path. Preserve dirty indication elsewhere if the label previously supplied it.
- The work path and document directory can differ, while KB-XFMD-009 removes the
  folder label. Recommend exposing the relative-path base through a tooltip or
  input hint rather than silently changing it.
- Decide whether submitting a directory changes work path, and whether opening a
  typed document changes work path. The owner's mandatory parent-folder update
  currently applies specifically to the dialog (KB-XFMD-006).

## Next review and completion criteria

Review the automatic path/filter interpretation before this card is selected, then coordinate
placement with KB-XFMD-009/011 and opening policy with KB-XFMD-006/007. These open
interaction details do not prevent recording the owner's requested behavior.

Candidate acceptance: full-width geometry at wide/narrow sizes, a long Unicode
path, left-click/edit/Enter/Escape, absolute and work-path-relative opening,
right-click clipboard contents, retained dirty state on cancel/error, wildcard
filter combinations, no duplicate sidebar search input, and behavior with Files
hidden or Index selected. Define the latter interactions before implementation.
No new widget or command is implemented by this note.

## Outcome and related cards

Recorded in backlog only. Implementation has not been selected or started by
this capture. Findings describe the inspected working tree, not the installed
binary shown in the screenshots; see the [source context](../evidence/2026-09-24-ui-wishlist/README.md).

Related: [KB-XFMD-006](%23006--Change--Open-files-and-folders.md), [KB-XFMD-007](%23007--Change--File-types-and-external-opening.md), [KB-XFMD-009](%23009--Change--Sidebar-controls-and-recent-tabs.md), [KB-XFMD-011](%23011--Change--Toolbar-layout-and-color-popup.md).
