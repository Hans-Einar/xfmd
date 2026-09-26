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
