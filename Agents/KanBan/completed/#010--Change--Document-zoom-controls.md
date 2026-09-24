# Provide normal zoom gestures and percentage controls in Wrap and A4

| Field | Value |
| --- | --- |
| id | KB-XFMD-010 |
| project | XFMD |
| type | Change |
| created | 2026-09-24T18:01:41+02:00 |
| source | Owner conversation, 2026-09-24 Europe/Oslo: XFMD UI wishlist; Owner item 6 |
| owner | Codex, selected P055 delivery |

## Need and owner direction

Ctrl+mouse-wheel and Ctrl++ / Ctrl+- must zoom document text in both the editor
and Markdown preview. This must work in Wrap and A4 modes. The toolbar's scale
dropdown should show the current zoom percentage and offer preset percentages at
the top, then a separator, then width-fit and height-fit choices.

The owner suggested `25%, 50%, 100%, 200%, 300%` as an example preset list, not a
final exhaustive range or step algorithm. The wording used was “fit actual width”
and “fit actual height”; settle clear UI labels while retaining both intentions.
100% represents actual size.

Add a View → Zoom submenu containing only the chosen percentage presets plus
Zoom in and Zoom out, displaying Ctrl++ / Ctrl+- beside those commands. The
requested View submenu does not include the fit choices from the toolbar dropdown.

## Current evidence and affected scope

The reported fast Ctrl+scroll has a concrete implementation:
[FoxWheelScrollBar.cpp](../../../src/application/adapters/FoxWheelScrollBar.cpp)
uses `page` as the wheel unit when Ctrl is held and bypasses personal acceleration.
[FUNC-015](../../../src/blueprint/functionality/Functionality-015--Scroll-Motion.md)
also documents Ctrl=page. This is static source evidence, not a runtime test.

[PreviewControls.cpp](../../../src/application/ui/controls/PreviewControls.cpp)
currently offers only Fit page width and Actual size, disables zoom outside A4,
and displays Fit width or 100%.
[EditorPresentation.cpp](../../../src/application/ui/EditorPresentation.cpp)
scales the editor font for A4 fit-width; its remaining scale is 1. The new manual
scale must reach both document surfaces, not just the preview's transform.

Review UR-011/017/033, SR-016/019 and
[FTR-007](../../../src/blueprint/feature/Feature-007--Paged-Publication.md),
[FUNC-010](../../../src/blueprint/functionality/Functionality-010--Workspace-Controls.md),
[FUNC-011](../../../src/blueprint/functionality/Functionality-011--Text-Editing.md),
[FUNC-005](../../../src/blueprint/functionality/Functionality-005--FOX-Presentation-Host.md).

## Agent recommendations and open questions

- Recommend a shared document zoom state for editor and preview in split mode,
  with the same commands for wheel, keys, dropdown and menu. The request covers
  document text, not UI-control font sizes. Decide state persistence explicitly.
- Consume Ctrl+wheel as zoom on document surfaces so it cannot also page-scroll.
  The scrollbar adapter is shared with trees/MRU lists: do not accidentally change
  their behavior. Decide whether wheel over those lists should retain its current
  meaning; this request does not require sidebar zoom.
- Define wheel direction, incremental step, min/max and behavior between presets.
  Ctrl+plus must work with the actual keyboard layout, including plus reached via
  Shift; cover keypad equivalents where supported. Ctrl+- must not edit text.
- Manual zoom should leave a fit mode and show the actual applied percentage.
  In automatic fit mode, show its computed percentage with a discoverable mode
  indication and update it after resize. These display details are recommendations.
- Clarify fit semantics for continuous Wrap (no physical page height) and for the
  editable A4 source surface (not paginated like preview). A reasonable proposal
  is width/height fit of one A4 page, with explicitly defined availability in Wrap;
  manual zoom must remain available in both modes regardless of that decision.
- Preserve caret/selection, dirty/undo and reading anchor. A4 zoom is a display
  transform, not a change to paper size, pagination or PDF output. Wrap may need
  reflow to keep the enlarged text within the viewport; retain source mapping.

## Next review and completion criteria

Review zoom-state ownership and fit semantics before selecting the zoom delivery;
coordinate toolbar/menu placement with KB-XFMD-011. Amend the existing Ctrl=page
policy and document the scope of that replacement.

Candidate acceptance: editor-only/preview-only/split × Wrap/A4; native wheel and
keyboard zoom, all presets, separators and menu contents, actual percentage on
resize/fit, boundary values, reading-position/selection preservation, no duplicate
scroll event, unchanged saved bytes/PDF geometry, and scrolling regression checks.
The current observation is explained from source; none of these new checks ran.

## Outcome and related cards

Recorded in backlog only. Implementation has not been selected or started by
this capture. Findings describe the inspected working tree, not the installed
binary shown in the screenshots; see the [source context](../evidence/2026-09-24-ui-wishlist/README.md).

Related: [KB-XFMD-011](../completed/%23011--Change--Toolbar-layout-and-color-popup.md).

## Goal selection — 2026-09-24

The owner selected KB-XFMD-006–011 as six subgoals of the active chat goal.
KB-XFMD-003 and optional SDUI modeling are excluded. Delivery is tracked in
[Sprint 007](../../../sprints/Sprint-007--Workspace-UI/README.md).
Implementation is selected; this card stays queued until its phase starts.

## P055 selection — 2026-09-24

Selected under the existing goal after P054. [P055](../../../sprints/Sprint-007--Workspace-UI/Phase-055--Document-Zoom.md)
resolves the interaction questions and defines bounds, fit semantics and acceptance.
Reuse the completed toolbar; no consolidation, SDUI work or KB-XFMD-012 migration.

## P055 outcome — 2026-09-24

Shared session-local zoom now reaches editor and preview in Wrap and A4 through
one application coordinator. Native Ctrl wheel/keys/keypad, percentage presets,
current percentage, A4 width/height fit and the distinct View submenu are checked
at 96/144 DPI. Caret/selection, source anchor, dirty/undo and saved bytes survive;
actual exported page/word geometry is identical across four zoom modes.

[P055 evidence](../../../sprints/Sprint-007--Workspace-UI/evidence/P055.md) records
the 73-test regression run, subsequent visual correction and focused acceptance,
sanitisers, inspected screenshots and identified source/binaries. The
[six-card audit](../../../sprints/Sprint-007--Workspace-UI/evidence/Completion.md)
completes the selected UI scope. Physical device/monitor trials remain outside
this automated acceptance. No merge or installation performed.
