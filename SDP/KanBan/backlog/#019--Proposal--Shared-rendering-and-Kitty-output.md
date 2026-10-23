# Shared XFMD rendering and Kitty output

| Field | Value |
| --- | --- |
| id | KB-XFMD-019 |
| project | XFMD |
| type | Proposal |
| CardState | backlog |
| created | 2026-09-27T23:04:02Z |
| source | Owner conversation 2026-09-28: shared XFMD rendering interface and Kitty Markdown/SDL/SDUI output |
| next_review | At selection of a shared rendering / terminal output delivery |
| tags | rendering, terminal, Kitty, SDL, SDUI |

## Owner need

Display formatted Markdown directly in Kitty, including SDL diagrams and SDUI
static dumps alongside ASCII output. Use a shared interface to the rendering
libraries used by XFMD so that rendering/style changes developed in XFMD can
also appear in a Kitty client. The owner asks about existing viewers and the
feasibility of a custom kitten; this capture does not select implementation.

## Inspected foundation

Source inspection on 2026-09-28, XFMD worktree HEAD 41fa494:

- `CMakeLists.txt` already separates xfmd_interpreter, xfmd_renderer,
  xfmd_typography, xfmd_media and xfmd_diagrams. Top-level configuration still
  requires FOX/X11; a headless target needs an appropriate build boundary.
- `src/contracts/RenderFrame.h` carries layout runs, source ranges, links,
  reading text and anchors. It is an in-process representation with shared
  resources, not an existing serializable interchange format.
- `src/application/adapters/DisplayListPainter.*` paints a RenderFrame to Cairo;
  `FontCatalog.*` and `SharedTextMetrics.*` supply Pango font resources/shaping.
  `PdfOutput.cpp` already reuses this painter for another output surface.
- `src/application/navigation/DocumentViews.cpp` requests generated SDL views
  from a registered producer and consumes Markdown/resources.
- SDUI's inspected `SDUI/go/cmd/sdui/main.go` in SDP-vNow supports dump, markdown
  and svg. `presentation/markdown.go` emits a cell-layout overview and semantic
  content in reading order; it does not preserve measured GUI geometry.
  `svg.Render` is the existing path for static visual layout.
- `SDUI/docs/handoff-xfmd-sdui.md` preserves independent Go language/runtime
  ownership, Fyne as interactive host, and SVG delivery to XFMD. The older BoxUI
  worktree must not be mistaken for the current integration boundary.

The source and paths above are inspection evidence, not a terminal runtime test.
Local `kitty --version` reports 0.43.1. No viewer was installed or exercised.

## Recommended boundary (agent proposal)

Expose one headless rendering facade over existing XFMD components, with
Markdown text, explicit resource base, width, scale and a shared appearance
profile as inputs. Keep parsing, layout, typography, diagram preparation and
painting shared with the GUI; merely using the same third-party libraries is
insufficient to ensure the same appearance.

Provide a small CLI consumer first, then an optional kitten for launching,
viewport sizing and reload. Proposed command names such as `xfmd-cat` and
`xfmd-render` are illustrative and do not exist yet.

Candidate outputs:

1. Visual fidelity: paint bounded image strips or viewport tiles through Cairo
   and transmit raster data through Kitty Graphics Protocol. Same fonts,
   resources, width and profile are prerequisites for visual comparison.
2. Terminal text: adapt semantic content to terminal cells, retaining selectable
   text and an ASCII fallback. This output cannot promise identical XFMD layout.
   A hybrid may use text for prose and graphics for diagrams.

Kitty accepts raster graphics rather than an SVG document directly; rasterize
SVG resources in the shared pipeline. Full-image text is not ordinary terminal
text: selection, search and link activation need explicit viewer behavior or
separate text output. Existing RenderFrame metadata can support later interaction.
Keep initial static dumping separate from an interactive SDUI runtime.

SDL and SDUI remain external producers. Consume their Markdown plus SVG/resource
bundles so SDL-specific symbols and SDUI visual layout survive. Share producer
adapters where justified; do not implement new language parsers in the kitten.
For stdin, require a resource base when relative assets need resolution. Cache
keys must include source/assets, width, scale, profile and renderer version.
Saved-file watch can pick up XFMD edits; unsaved-buffer sharing requires an
explicit later snapshot/IPC contract. Persist and reload appearance profiles if
GUI preference changes are expected to propagate between processes.

## Alternatives and sources

- [Kitty graphics protocol](https://sw.kovidgoyal.net/kitty/graphics-protocol/)
  and [icat](https://sw.kovidgoyal.net/kitty/kittens/icat/) provide image delivery.
- [Custom kittens](https://sw.kovidgoyal.net/kitty/kittens/custom/) document the
  extension mechanism; a standalone CLI can also use the graphics protocol.
- [KMV / cat-md](https://github.com/parf/Kitty-Markdown-Viewer) supplies Markdown
  terminal formatting with large headings and inline images.
- [markdown-on-kitty](https://github.com/H1rla/markdown-on-kitty) renders complete
  documents with Cairo/Pango to graphics. It demonstrates the approach but does
  not reuse XFMD's parsing/layout/style implementation.
- [Kitty integrations](https://sw.kovidgoyal.net/kitty/integrations/) also lists
  mdfried and other Markdown tools. Listings are not local compatibility tests.

## Overlap review

[KB-XFMD-015](%23015--Proposal--SDL-design-file-preview.md) owns editable SDL
source/preview pairing; [KB-XFMD-014](../active/%23014--Proposal--SDP-sidebar-and-generated-navigation.md)
owns native navigation. Both can reuse producer delivery, but terminal output
is a separate consumer. [KB-XFMD-003](../superseded/%23003--Question--SDUI-widget-ownership.md)
remains the SDUI control ownership question. No existing card is superseded.

## Next action and completion criteria

Select a bounded headless-rendering spike and decide whether the first client
prioritizes visual fidelity, selectable text or both. Define the facade/profile
contract before changing callers. Compare the same Markdown fixture through GUI
and headless Cairo with matched fonts, width and appearance; include a table,
image, SDL diagram and SDUI SVG, preserving all relative resources. Exercise
stdin, resize, long-document tiling and graphics-unavailable behavior. Verify
actual Kitty output and distinguish a persistent shell dump from a fullscreen
viewer. No implementation or runtime parity is claimed by this proposal.

## Worklog

| Time (RFC3339) | Actor / event | Work, finding or decision | Evidence / remaining work |
| --- | --- | --- | --- |
| 2026-09-27T23:04:02Z | codex; EVT-KB-XFMD-000078 | Registered owner idea and source-backed feasibility analysis in backlog | Code inspection and upstream links above; implementation remains unselected |
