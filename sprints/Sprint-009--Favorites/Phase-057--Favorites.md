# Phase 057 — Favorites

Branch: `sprint/009/phase/057-favorites`; base cf11709. M1 follows requirements →
FUNC-010 blueprint → application adapter/wiring → verification.

## Selected behavior and ownership

Favorites is the third lower tab, after Folders and Files. Add… selects one existing
regular file or directory (including the current chooser directory). Canonical
absolute paths prevent duplicates and symlink aliases. Sort folders first, then
files, by case-insensitive basename with absolute path as a deterministic tie-break.
Display basename and parent path, abbreviating home as ~. Use an actual disabled
horizontal separator only when both groups exist. Remove forgets the selected entry.
No target is deleted. Empty lists explain how to add a favorite.
Arrow/Home/End keys only select; click, Enter or Space activates. The list adapter
suppresses FOX navigation-key command notifications while retaining selection updates.

FavoritesPanel owns this FOX adapter, entries and a separate Favorites registry
section (Count, PathN, KindN). Retain missing restored entries and their saved type;
activation reports missing/changed types without changing document/root. Limit 512
entries with explicit refusal, never MRU eviction. Registry failures report that
changes are session-only. Live synchronization across processes is outside scope.

WorkspacePanel composes the tab; Application::wireWorkspace injects current root,
folder activation via setWorkPath, and file activation via openTarget. Copy activation
requests and defer until native dispatch finishes; cancel pending timers on teardown.
OpenPathDialog gains optional labels, preserving Open defaults. Existing interpreter,
renderer, document transaction and history contracts are reused unchanged.

## M1 verification

Add FavoritesGuiTest for native Add/Cancel/Remove and activation, mixed alphabetical
ordering, real separator, duplicates, restart restoration, missing/changed targets,
dirty cancellation and folder navigation retaining dirty document. Inspect screenshots
in light/dark. Adapt the old Files-tab test's position assumption. Run focused GUI
regressions, blueprint/symbol/layer and board checks, plus lifetime sanitizer coverage.
Record actual evidence before completing KB-XFMD-018.

## M1 outcome

Locally complete with seven release checks and a dedicated sanitizer check.
[Evidence](evidence/P057.md) records source hashes, actual results, captures and limits.
KB-XFMD-018 is completed for the explicitly selected scope.

## M2 — Owner interaction correction (2026-09-27)

Collaborative follow-up on the same phase branch and PR #41, base 456e095.
The owner's right-click workflow supersedes M1's chooser/buttons and empty hint.
Favorites is only a list. Tree and both history lists offer Add to Favorites on the
clicked file/folder; Favorites offers Remove from Favorites on the clicked entry.
No action on empty space/separator; dismissing the menu preserves state. Adding or
removing does not navigate, prompt for edits or change histories. Keep tree Set work
path and existing persisted data. No new chooser/shortcut/drag-and-drop scope.

Application adapters: SidebarPathList owns shared wheel bars and right-click
hit testing for the three lower lists, suppressing FOX right-drag behavior before
showing a popup. PathContextMenu owns the short-lived native popup and returns the
chosen action after it closes. Callers copy paths before entering its modal loop;
no tree/list item pointer survives into an action. WorkspacePanel wires tree/history
add requests to FavoritesPanel::add; FavoritesPanel removes by copied path.

Revise FavoritesGuiTest to drive native menus from all four surfaces, including
clicked-versus-selected rows, dismissal, empty/separator input, dirty preservation,
normal activation and restart persistence. Retain focused history/tree/Open checks,
separate sanitizer coverage, screenshots and blueprint/board validators.

The popup owns a temporary keyboard grab for Escape/arrow input and releases it
before the chosen action runs. OpenPathDialog returns to its original two-argument
constructor because Favorites no longer needs custom chooser labels.

M2 is locally complete. [Evidence](evidence/M2/README.md) records seven release
checks, dedicated sanitizer coverage and inspected screenshots. KB-XFMD-018 is
completed for this owner-selected correction; no merge or version bump.
