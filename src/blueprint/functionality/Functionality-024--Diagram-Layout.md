---
id: FUNC-024
kind: Functionality
audience: System
role: Service
owner: renderer
status: Implemented
scope: FirstRelease
requirements: UR-039, UR-040, UR-041, SR-021, SR-022, SR-023, SR-024
uses: none
---

# Functionality-024: Diagramlayout og tegnedata

## 1. Hensikt og avgrensning

Renderer-tjeneste for grafplassering, rutegeometri og bibliotekgenerert SVG. Gjenbruker bibliotekets layout; har ingen dependency til interpreter og mottar aldri Mermaid-kilde.

## 2. Krav og akseptanse

UR-039–041, SR-021–023; AT-059–064. Se [design og kontrakter](../../../docs/design/mermaid-integration.md) og [krav](../../../xfmd_requirements.md).

## 3. Kontrakter og eierskap

IDiagramLayout::layout(DiagramModel, DiagramLayoutRequest, ITextMetrics&) → DiagramScene. Modellen og resultatet er XFMD-eide verdier. Rust-bridge rekonstruerer upstream Graph fra den rene modellen og bruker routed::compute med separat rutervalg. Layout inneholder tekstbokser/edge points; MermaidDiagramLayout dekoder inspeksjonsgeometri og ferdig SVG; DiagramPainter viser SVG gjennom librsvg/Cairo. DiagramPlacement lager én visual-run.

## 4. Atferd, tilstand og feil

Layout forberedes i worker. Tekstmåling skjer med samme fontgrunnlag som brødtekst/PDF; forkens measurements::with_measurements mater målte TextBlock-verdier til layout. Ingen oversettelse fra kildestreng til layout i denne tjenesten. Ingen SVG-rasterisering eller separate Pango-tekstobjekter for diagrametiketter. Ukjent form, NaN, ugyldig kant eller budsjettbrudd gir blokklokal feil.

## 5. Plumbing

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `IDiagramLayout virtual dispatch` | `MermaidDiagramLayout::layout` | `src/renderer/diagram/MermaidDiagramLayout.cpp` | modell + font/request → immutable scene | checkpoint før/etter Rust | Implemented |
| 2 | `MermaidDiagramLayout::layout` | `DiagramTextLayout::measure` | `src/renderer/diagram/DiagramTextLayout.cpp` | etiketter → formede linjer og mål | samme ITextMetrics-port | Implemented |
| 3 | `MermaidDiagramLayout::layout` | `xfmd_diagram_layout_v1` | `src/application/composition/mermaid/src/lib.rs` | ren graf + labelmål → LayoutResult | ingen parserkall/opaque parserhandle | Implemented |
| 4 | `xfmd_diagram_layout_v1` | `layout` | `src/renderer/diagram/rust/src/lib.rs` | mapping → forkens routed::compute/render_svg → geometri og SVG | inputpayload 4 / scene 3; maks 8 MiB | Implemented |
| 5 | `MermaidDiagramLayout::layout` | `Reader::finish` | `src/contracts/diagram/DiagramWire.h` | ferdig dekodet scene → kontrollert buffer | trailing data avvises; scene er XFMD-eid | Implemented |
| 6 | `BlockLayout::layout` | `DiagramPlacement::append` | `src/renderer/diagram/DiagramPlacement.cpp` | SVG-scene → én skalert visual-run | Approximate blokkanker, ingen sideklipping | Implemented |

| 7 | `layout` | `render_svg` | `src/renderer/diagram/rust/src/lib.rs` | eksisterende Layout → SVG-bytes | ingen ny parsing | Implemented |

| 8 | `layout` | `with_measurements` | `src/renderer/diagram/rust/src/lib.rs` | scoped TextBlock-mål og frist → valgt layout | TLS gjenopprettes også ved panic | Implemented |
| 9 | `layout` | `compute` | `src/renderer/diagram/rust/src/lib.rs` | graph + Engine + RoutingControl → RoutedLayout | Libavoid-feil blir synlig kildefallback | Implemented |
| 10 | `layout` | `render_svg_with_crossings` | `src/renderer/diagram/rust/src/lib.rs` | ferdige logiske ruter → valgfrie presentasjonshopp | ingen endring av rutepunkter | Implemented |

| 11 | `layout` | `add_label_leaders` | `src/renderer/diagram/rust/src/lib.rs` | ferdig SVG + immutable Layout → SVG med pekere og antall utelatelser | ingen ruteflytting; blokkerte pekere utelates med diagnostikk | Implemented |

## 6. Gjenbruk og avhengigheter

Konsumenter: FUNC-025, eksisterende MarkdownRenderer og PDF via vanlig frame. ITextMetrics er en ren kontrakt; Application injiserer implementasjonen. Bruker ikke parserens crate, kilde eller skjulte handle. Biblioteket eier SVG-former/piler/etiketter; Application eier SVG-leseren.

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

P31/P32: [Gjeldende SVG-/rutebeslutning](../../../docs/design/mermaid-svg-routing.md) erstatter tidligere native etiketttegning. Historiske tester nedenfor gjelder P25–P30; ny atferd er implementert; P31/P32-bevis beskriver faktisk verifikasjon.

[Rutestudien](../../../docs/design/mermaid-routing-study.md) dokumenterer
eksisterende sidevalg/A*/portfinjustering og foreslått felles kostnadspolicy.
En eventuell ny strategi utvikles i en separat bibliotekfork etter beslutning.
Planlagte symboler i studien er ikke del av dagens plumbing.

Implementert. Versjonsbundet patch leverer måleseam og kooperativ tidsgrense. P25–P30 bygget scene i adapteren og separate DrawRuns; dette erstattes i P31. Se P26-bevis og videre P28/P29-verifikasjon.

Akseptanse: AT-059, AT-060, AT-061, AT-062, AT-063, AT-064.

AT-065 dekkes av P32 og forkens rapport.

### P32: autoritativ ruting

Standardmotor er Libavoid; `XFMD_MERMAID_ROUTER=legacy` velger gammel motor
eksplisitt. Ukjente verdier avvises. `XFMD_MERMAID_CROSSING_JUMPS=1` slår på
presentasjonshopp. Valgene inngår i scenecachen. Resultatet eier diagnostikk
med faktisk motor og begrensninger; feil gir synlig kildefallback, ikke motorbytte.

Rust `layout` → `measurements::with_measurements` → `routed::compute` →
backendens samlede libavoid-transaksjon → målte etiketter/validering →
`render_svg` eller `render_svg_with_crossings` → payload 3. Ingen gamle
reparasjonspass endrer Libavoid-rutene. Native tidsavbrudd er kooperativt;
XFMDs dokumentkansellering kontrolleres før/etter FFI, mens native callback
kontrollerer fristen under transaksjonen.

Før native arbeid avviser adapteren over 256 kanter / 2048 kandidatporter.
DiagramLayoutTest skiller denne admission-feilen fra faktisk tidsavbrudd;
fristens nedre tidsassert gjelder bare tidsavbrudd. Etter begge feilveier
skal en vanlig modell fortsatt kunne rendres.

Et siste etikett-only-pass kan plassere på de ferdige rutene etter tredje
transaksjon; det kan ikke starte flere transaksjoner. Ubuntu/Pango-fixture
verifiserer dette i forken. Self-loop-endepunkter er distinkte punkter på
formgrensen; libavoid eier fortsatt ruting og segmentforskyvning.

Gjeldende P31/P32-verifikasjon: [samlet testbevis](../../../docs/evidence/P32.md).

### P33: målte, ombrutte kantetiketter

DiagramTextLayout::measure bryter kanttekst ved ordgrenser før måling.
MermaidDiagramLayout::layout serialiserer original nøkkel og faktiske linjer
i inputpayload 4; Rust layout bruker linjene direkte i TextBlock. Eksisterende
plumbing i kapittel 5 gjelder. Node-/gruppetitler prioriteres ved delt tekstnøkkel.
Se [policy og akseptanse](../../../docs/design/mermaid-label-wrap.md).

P33-verifikasjon: [før/etter-SVG og 8/8 relevante tester](../../../docs/evidence/P33.md).

### P34: beskyttet etikettilhørighet

SR-024 presiseres med en beskyttet sone mellom etikett og tilordnet segment.
Forkens labels::place prioriterer fri sone; labels::obstacles reserverer den
under eksisterende avgrensede pass; labels::validate avviser fremmede kanter
i sonen etter omruting. Implementert i fork-pin `0f23b7e`; verifikasjon dokumenteres i P34.
Ingen nye kall i XFMDs plumbing; endringen eies av bibliotekets rutepipeline.

P34-verifikasjon: [soneregresjoner og før/etter-SVG](../../../docs/evidence/P34.md).

### P35: synlige etikettpekere

Implementert: Rust layout kaller forkens add_label_leaders etter SVG-rendering.
Steget bruker ferdige ruter og mål, uten ny layout. Se
[geometri og feilvei](../../../docs/design/mermaid-label-leaders.md).
