# Regroup toolbar actions and hide reading-color sliders in the theme popup

| Field | Value |
| --- | --- |
| id | KB-XFMD-011 |
| project | XFMD |
| type | Change |
| created | 2026-09-24T18:01:41+02:00 |
| source | Owner conversation, 2026-09-24 Europe/Oslo: XFMD UI wishlist; Owner item 7 |
| next_review | Before selecting the next XFMD UI work round; resolve this card's named interaction questions during requirements/blueprint planning |

## Need and owner direction

After KB-XFMD-008 moves the document field into a separate lower row, retain
breathing room in the upper toolbar. Do not fill all released space with controls.

- Place the Sidebar toggle at the far left of the icon controls, closest to the
  dropdown menus.
- Visually group Editor, Split and Preview using spacing and/or vertical
  separators; keep them distinct from unrelated actions.
- Put Back and Forward to the right of that view-mode group.
- Move the Light/Dark toggle to the far right of the upper toolbar.
- Right-clicking that toggle opens a dropdown/popover containing the background
  and text color sliders currently exposed on the toolbar. Ordinary activation
  continues to toggle Light/Dark. Remove the always-visible sliders from the row.

This is a rearrangement of existing FOX controls and reading preferences, with a
new popup interaction. The owner prioritizes delivery of these changes; describing
the layout in SDUI would be interesting but must not become a prerequisite.

## Current evidence and affected scope

[XfmdToolbar.cpp](../../../src/application/ui/XfmdToolbar.cpp) currently builds
file actions, navigation/sidebar, document label, view modes, theme, Wrap/A4/scale
and `PreviewColorControls` on the same compact toolbar. See
[Image 2](../evidence/2026-09-24-ui-wishlist/toolbar-before.png).
[PreviewColorControls.cpp](../../../src/application/ui/controls/PreviewColorControls.cpp)
and [ApplicationAppearance.cpp](../../../src/application/ApplicationAppearance.cpp)
already provide live reading colors with separate light/dark profiles.

Review UR-025/026/030/031/032 and
[FUNC-010](../../../src/blueprint/functionality/Functionality-010--Workspace-Controls.md)/
[FUNC-014](../../../src/blueprint/functionality/Functionality-014--Application-Preferences.md).
The two-row request revises the existing single-low-top-line placement policy.
Retain its intent of usable geometry at narrow widths.

## Agent recommendations and open questions

- Candidate ordering: menus, Sidebar, Open/Save, separated Editor/Split/Preview,
  Back/Forward, Wrap/A4/Zoom, flexible free space, Light/Dark. This is a proposed
  complete arrangement; the owner fixed relative placement, not every slot.
- Reuse the current BG and Text hue/brightness sliders and persistence callbacks
  inside a popup that supports dragging. Do not introduce new color semantics.
- Right-click must open the popup without also toggling the theme. Decide keyboard
  access to the popup, Escape/focus dismissal and whether it stays open while
  dragging; ordinary left-click/Space must retain the theme action.
- Preserve live repaint in editor and preview, theme-specific saved values,
  existing preference error handling and unchanged PDF palette.
- Keep enabled/checked state and shortcuts shared with menus through existing
  command routing. Define narrow-window wrapping/overflow and right-edge theme
  placement without clipping the new path row or view-mode group.

## Next review and completion criteria

Review together with KB-XFMD-008/010 so field relocation, zoom controls and toolbar
geometry form one consistent layout. Link selected sprint/blueprint changes.

Candidate acceptance: screenshot comparison at wide and narrow widths, all view
modes and themes, distinct view-mode grouping, rightmost theme toggle, left-click
versus right-click/keyboard popup behavior, live slider dragging, separate palette
persistence after restart, and unchanged document/undo/scroll/PDF state.
No runtime or visual acceptance was executed for the requested arrangement.

## Related scope review

[KB-XFMD-003](%23003--Question--SDUI-widget-ownership.md) remains an SDUI-owned
question. These XFMD UI requests neither answer it nor revive historical BoxUI.
The optional wish to describe the resulting UI in SDUI is retained here for a
later review once the FOX interaction/layout is settled; no SDUI implementation
or new SDP process is selected.

## Outcome and related cards

Recorded in backlog only. Implementation has not been selected or started by
this capture. Findings describe the inspected working tree, not the installed
binary shown in the screenshots; see the [source context](../evidence/2026-09-24-ui-wishlist/README.md).

Related: [KB-XFMD-008](%23008--Change--Document-path-and-file-filter.md), [KB-XFMD-010](%23010--Change--Document-zoom-controls.md).
