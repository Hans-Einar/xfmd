# P057-M2 — Favorites context menus

UR-045 / AT-073, FUNC-010, KB-XFMD-018. Base 456e095 plus the changes identified
in [verification.json](verification.json). Tested identity:
`0.3 sprint/009/phase/057-favorites:204+dirty`. This is the collaborative correction
of M1's initial interaction; M1 evidence remains historical.

## Delivered behavior and checks

Favorites contains only its list. Right-click file/folder entries in the Files
tree or Folders/Files history to Add to Favorites; right-click an existing favorite
to Remove from Favorites. No chooser, buttons or empty-list text remains. Menu
requests copy the clicked path before entering the modal loop and apply the action
after the popup closes; they neither open nor delete the target. Existing folder
Set work path remains first in the tree menu. Registry fields and sorting are unchanged.

- [FavoritesGuiTest](favorites-tests.log): passed in 10.04 s. Native file/folder tree
  menus, both history menus, removal from Favorites, Escape dismissal, selected vs.
  clicked entries, root bookmarking, separator/empty-space nonactions, dirty buffer
  and history preservation. Retains mixed sorting, duplicates/symlink aliases,
  changed/missing targets, normal activation, pending-timer teardown and fresh-exec
  persistence. Assertions confirm no buttons or empty hint in the panel.
- [Six regressions](regression-tests.log): passed in 20.38 s. WorkPathGuiTest,
  PathWorkspaceGuiTest, OpenPathDialogGuiTest, TreeActionsTest, RecentFilesGuiTest,
  BuildVersionTest. Includes existing Set work path and shared Open behavior.
- [ASan/UBSan FavoritesGuiTest](sanitizer-tests.log): passed in 9.56 s, using the
  existing Fontconfig-only leak suppression. Modal popup and deferred activation
  lifetimes are covered.
- [Structural checks](checks.log): blueprint metadata/matrix/links, documented
  symbols, layer boundaries, board schema/replay/CardState and whitespace.

Inspected captures: [list/light](favorites-light.png), [list/dark](favorites-dark.png),
[tree/file](tree-file-menu.png), [tree/folder](tree-folder-menu.png),
[file history](file-history-menu.png), [folder history](folder-history-menu.png),
[remove](favorites-remove-menu.png). The list captures retain the status from the
preceding deliberate changed-target test. Menu captures show the preserved dirty buffer.

## Corrections and limits

Initial Escape tests timed out because the context popup did not receive keyboard
input while X focus remained with the document window. PathContextMenu now owns a
short-lived keyboard grab and releases it before running an action. The test driver
also queues the right-click before scheduling menu input, so the click-speed delay
cannot fire the driver before a popup exists. Final checks cover both corrections.

Automated Xvfb/FOX 1.6 evidence does not claim physical desktop or all DPI/font
combinations. Space/Enter activation, registry bounds, failure policy and absent
cross-process synchronization retain M1's documented scope. Source/binary hashes
identify the dirty test build; clean installation identity is recorded separately.
No renderer/interpreter change, migration, merge or blanket Verified status.
