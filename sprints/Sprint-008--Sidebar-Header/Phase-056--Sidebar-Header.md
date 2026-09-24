# Phase 056 — Sidebar header

Branch: `sprint/008/phase/056-sidebar-header`; base a77e08d. M1 follows
requirements → blueprints → existing application controls → native verification.

## Selected behavior

Files/Index tabs and Refresh share a single header with Markdown, Up and Open.
Files-only controls hide on Index and return with their previous filter state.
Remove the separate type/action row and the .txt toggle. Markdown on restricts
matches to .md (case-insensitive); off admits every type. Existing name filtering
still intersects the result. Text opening and routing remain unchanged.

The Markdown control is an oval icon button with visible selected, focus and hover
states and a tooltip explaining the filter. Its glyph uses the familiar M/down
arrow/box mark, not literal angle brackets. Reuse IconCatalog and UiButton; add a
pill button role to the existing painter and expose its existing checked state.
WorkspacePanel owns toggling/filter dispatch and tab visibility. No new class,
parser/renderer dependency or replacement filter implementation is needed.

## M1 verification

Adapt existing WorkPathGuiTest for the single toggle. Extend native workspace
acceptance for same-row unclipped geometry, Index hide/Files restore, mouse/Space
filtering, continued Up/Open/Refresh behavior, and screenshots in both themes.
Run relevant existing GUI/control/filter checks, blueprint checks and diff checks.
Record actual results/source identity before completing the selected card.

## M1 outcome

Implemented and locally accepted: four native checks, five supplemental regressions
and the final appearance/capture rerun pass. [Evidence](evidence/P056.md) identifies
the dirty-source build on a77e08d and inspected light/dark/Index screenshots. The
final committed build is installed separately with an installation receipt.
