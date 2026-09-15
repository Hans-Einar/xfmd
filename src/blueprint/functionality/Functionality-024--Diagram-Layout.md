---
id: FUNC-024
kind: Functionality
audience: System
role: Service
owner: renderer
status: Proposed
scope: Future
requirements: UR-039, UR-040, UR-041, SR-021, SR-022, SR-023
uses: none
---

# Functionality-024: Diagramlayout og tegnedata

## 1. Hensikt og avgrensning

Renderer-tjeneste for grafplassering, rutegeometri og lesbar diagramtekst. Gjenbruker bibliotekets layout; har ingen dependency til interpreter og mottar aldri Mermaid-kilde.

## 2. Krav og akseptanse

UR-039–041, SR-021–023; AT-059–064. Se [design og kontrakter](../../../docs/design/mermaid-integration.md) og [krav](../../../xfmd_requirements.md).

## 3. Kontrakter og eierskap

IDiagramLayout::layout(DiagramModel, DiagramLayoutRequest, ITextMetrics&) → DiagramScene. Modellen og resultatet er XFMD-eide verdier. Rust-bridge rekonstruerer upstream Graph fra den rene modellen og bruker compute_layout. Layout inneholder tekstbokser/edge points; DiagramSceneBuilder lager støttede former og pilspisser med semantiske farger. DiagramPlacement gjenbruker normal frame-tekst/shaping.

## 4. Atferd, tilstand og feil

Layout forberedes i worker. Tekstmåling skjer med samme fontgrunnlag som brødtekst/PDF; en planlagt, versjonsbundet upstream-seam mater målte TextBlock-verdier til layout. Ingen oversettelse fra kildestreng til layout i denne tjenesten. Ingen SVG-rasterisering eller glyph-konturer som erstatning for kopierbar tekst. Ukjent form, NaN, ugyldig kant eller budsjettbrudd gir blokklokal feil.

## 5. Plumbing

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `IDiagramLayout virtual dispatch` | `MermaidDiagramLayout::layout` | `src/renderer/diagram/MermaidDiagramLayout.cpp` | modell + font/request → immutable scene | checkpoint før/etter Rust | Planned |
| 2 | `MermaidDiagramLayout::layout` | `DiagramTextLayout::measure` | `src/renderer/diagram/DiagramTextLayout.cpp` | etiketter → formede linjer og mål | samme ITextMetrics-port | Planned |
| 3 | `MermaidDiagramLayout::layout` | `xfmd_diagram_layout_v1` | `src/application/composition/mermaid/src/lib.rs` | ren graf + labelmål → LayoutResult | ingen parserkall/opaque parserhandle | Planned |
| 4 | `xfmd_diagram_layout_v1` | `layoutGraph` | `src/renderer/diagram/rust/src/layout.rs` | mapping → upstream compute_layout → ren geometri | mål-seam må verifiseres i P26 | Planned |
| 5 | `MermaidDiagramLayout::layout` | `DiagramSceneBuilder::build` | `src/renderer/diagram/DiagramSceneBuilder.cpp` | layout → stier + semantiske etiketter | forme-/piltester | Planned |
| 6 | `MarkdownRenderer::layout` | `DiagramPlacement::append` | `src/renderer/diagram/DiagramPlacement.cpp` | scene → skalert frame og lesetekst | Approximate blokkanker, ingen sideklipping | Planned |


## 6. Gjenbruk og avhengigheter

Konsumenter: FUNC-025, eksisterende MarkdownRenderer og PDF via vanlig frame. ITextMetrics er en ren kontrakt; Application injiserer implementasjonen. Bruker ikke parserens crate, kilde eller skjulte handle. Form-/pilgeometri eies her, Cairo utfører bare kommandoene.

## 7. Verifikasjon

Planlagt DiagramLayoutTest og DiagramPlacementTest, plus Rust/C-ABI-tester. Sammenlign geometri/kanter mot upstream, og mål tekstoverløp med norske etiketter. Native merking og PDF er AT-060/061. Parserfri alternativ modell er AT-062.

## 8. Status, risiko og endringskonsekvenser

Proposed. Måleseam er ikke et eksisterende upstream-API. Scene-API-et render_scene er bevisst ikke valgt som produksjonsport: det parser igjen og gjør tekst til konturer. P26 gate må bevise valgt vei før Ready.

Planlagt akseptanse: AT-059, AT-060, AT-061, AT-062, AT-063, AT-064.
