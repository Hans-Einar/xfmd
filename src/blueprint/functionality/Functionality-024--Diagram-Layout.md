---
id: FUNC-024
kind: Functionality
audience: System
role: Service
owner: renderer
status: Implemented
scope: FirstRelease
requirements: UR-039, UR-040, UR-041, SR-021, SR-022, SR-023
uses: none
---

# Functionality-024: Diagramlayout og tegnedata

## 1. Hensikt og avgrensning

Renderer-tjeneste for grafplassering, rutegeometri og lesbar diagramtekst. Gjenbruker bibliotekets layout; har ingen dependency til interpreter og mottar aldri Mermaid-kilde.

## 2. Krav og akseptanse

UR-039–041, SR-021–023; AT-059–064. Se [design og kontrakter](../../../docs/design/mermaid-integration.md) og [krav](../../../xfmd_requirements.md).

## 3. Kontrakter og eierskap

IDiagramLayout::layout(DiagramModel, DiagramLayoutRequest, ITextMetrics&) → DiagramScene. Modellen og resultatet er XFMD-eide verdier. Rust-bridge rekonstruerer upstream Graph fra den rene modellen og bruker compute_layout. Layout inneholder tekstbokser/edge points; MermaidDiagramLayout dekoder til semantiske former og kantruter; DiagramPainter utfører primitivene med Cairo. DiagramPlacement gjenbruker normal frame-tekst/shaping.

## 4. Atferd, tilstand og feil

Layout forberedes i worker. Tekstmåling skjer med samme fontgrunnlag som brødtekst/PDF; en versjonsbundet upstream-seam mater målte TextBlock-verdier til layout. Ingen oversettelse fra kildestreng til layout i denne tjenesten. Ingen SVG-rasterisering eller glyph-konturer som erstatning for kopierbar tekst. Ukjent form, NaN, ugyldig kant eller budsjettbrudd gir blokklokal feil.

## 5. Plumbing

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `IDiagramLayout virtual dispatch` | `MermaidDiagramLayout::layout` | `src/renderer/diagram/MermaidDiagramLayout.cpp` | modell + font/request → immutable scene | checkpoint før/etter Rust | Implemented |
| 2 | `MermaidDiagramLayout::layout` | `DiagramTextLayout::measure` | `src/renderer/diagram/DiagramTextLayout.cpp` | etiketter → formede linjer og mål | samme ITextMetrics-port | Implemented |
| 3 | `MermaidDiagramLayout::layout` | `xfmd_diagram_layout_v1` | `src/application/composition/mermaid/src/lib.rs` | ren graf + labelmål → LayoutResult | ingen parserkall/opaque parserhandle | Implemented |
| 4 | `xfmd_diagram_layout_v1` | `layout` | `src/renderer/diagram/rust/src/lib.rs` | mapping → upstream compute_layout → ren geometri | mål-seam må verifiseres i P26 | Implemented |
| 5 | `MermaidDiagramLayout::layout` | `Reader::finish` | `src/contracts/diagram/DiagramWire.h` | ferdig dekodet scene → kontrollert buffer | trailing data avvises; scene er XFMD-eid | Implemented |
| 6 | `BlockLayout::layout` | `DiagramPlacement::append` | `src/renderer/diagram/DiagramPlacement.cpp` | scene → skalert frame og lesetekst | Approximate blokkanker, ingen sideklipping | Implemented |


## 6. Gjenbruk og avhengigheter

Konsumenter: FUNC-025, eksisterende MarkdownRenderer og PDF via vanlig frame. ITextMetrics er en ren kontrakt; Application injiserer implementasjonen. Bruker ikke parserens crate, kilde eller skjulte handle. Form-/pilgeometri eies her, Cairo utfører bare kommandoene.

## 7. Verifikasjon

P30-regresjon: `docs/design/mermaid/traceability.mmd` (13 noder, 17 merkede
kanter) skal gi en full scene innen produksjonens eksisterende tidsbudsjett.
Undersøk og begrens kostbar ruteforbedring; ikke skjul feilen med lengre frist.

Implementert rettelse: tidskontrollen sampler klokken ved første og hvert
64. kontrollpunkt per layoutkall. Geometri og tidsbudsjett er uendret.
Regresjonen inngår også i MermaidPdfTest; TLS-testen dekker et helt
samplingsintervall slik at etterlatt deadline fortsatt ville blitt oppdaget.

DiagramLayoutTest dekker parserfri modell, begge brukerdiagrammer, nested grupper, fontmål, 128-noders kjede og tidsavbrudd med etterfølgende normal layout. DiagramReadingTest og MermaidPdfTest dekker skalering, merking og native tekst. Se [P29](../../../docs/evidence/P29.md).

P29 utvider fristkontrollene til indre løkker i rangering, kantpipeline, ruting
og etterbehandling etter at CI avdekket et langt intervall uten kontroll.
Tidsassertene gjelder fortsatt både produksjon og sanitizer-bygg.

## 8. Status, risiko og endringskonsekvenser

Implementert. Versjonsbundet patch leverer måleseam og kooperativ tidsgrense. Scene bygges i adapteren; DiagramPlacement lager vanlige DrawRuns. Se P26-bevis og videre P28/P29-verifikasjon.

Akseptanse: AT-059, AT-060, AT-061, AT-062, AT-063, AT-064.
