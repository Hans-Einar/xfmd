---
id: FUNC-004
kind: Functionality
audience: System
role: Service
owner: renderer
status: Proposed
scope: FirstRelease
requirements: UR-005, UR-002, UR-008, SR-001, SR-003, SR-005, SR-008, SR-009, SR-011, SR-013, UR-017, UR-018, SR-016, SR-019
uses: FUNC-017
---

# Functionality-004: Layout og visuell dokumentmodell

## 1. Hensikt og avgrensning

Beregn presentasjon fra semantisk modell uten FOX-avhengighet. Eie fontvalg som semantiske FontSpec-verdier, block/inline-layout, hit-regioner og mapping. Ingen Markdown-parsing, filåpning eller dokumenttilstand.

## 2. Krav og akseptanse

Krav: UR-005, UR-002, UR-008, SR-001, SR-003, SR-005, SR-008, SR-009, SR-011, SR-013. Definisjoner og normativ akseptanse finnes i
[kravspesifikasjonen](../../../xfmd_requirements.md). Kapittel 7 konkretiserer beviset.

## 3. Kontrakter og eierskap

MarkdownRenderer implementerer IRenderer::layout/hitTest. BlockLayout og InlineLayout eier layout; HitTester eier lenketreff. Bare rene kontrakter konsumeres. DrawRun inneholder FontSpec og dokumentkoordinater, ikke FOX-ressurser.

**Planlagt utvidelse 1.1:** Trekk ut målt FlowLayout med visuelle linjer fra dagens layout. LayoutUnit blir points; PageComposer fordeler flyten over sider. Shaping injiseres via rene porter. Renderer eier plassering, aldri native fontressurser.

## 4. Atferd, tilstand og feil

Layout bryter vanlig tekst ved ord/UTF-8-grenser; kode beholder whitespace og kan scrolle horisontalt. Varierende heading-fonter, nested lister/sitater, inline-kode og inert HTML støttes. Frame inneholder både run-ranges og block-ranges for hidden syntax. Ingen parsing, I/O eller utføring av lenker.

LinkMarker lager #, /# eller Globe-primitiven. InlineRun.linkId skiller
separate lenker fra stilfragmenter i samme lenke. Markør og tekst har samme
lenkemål; syntetisk markør har tomt, tilnærmet kildeanker.

## 5. Plumbing

Implemented-rader beskriver baseline 0712c29; Planned-rader beskriver utvidelsen. Navngitte hendelser er injiserte callbacks, ikke en global event bus.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `PreviewCoordinator::relayout` | `MarkdownRenderer::layout` | `src/renderer/MarkdownRenderer.cpp` | Model/request/metrics → frame | Width valideres | Implemented |
| 2 | `MarkdownRenderer::layout` | `BlockLayout::layout` | `src/renderer/BlockLayout.cpp` | Blocks → vertikal geometri | Ressursgrenser | Implemented |
| 3 | `BlockLayout::layout` | `InlineLayout::layout` | `src/renderer/InlineLayout.cpp` | Runs → linjer og ankere | UTF-8 splitter aldri inne i tegn | Implemented |
| 4 | `InlineLayout::layout` | `ITextMetrics::measure` | `src/contracts/ITextMetrics.h` | Tekst/font → extent | Ren port | Implemented |
| 5 | `MarkdownRenderer::hitTest` | `HitTester::hitTest` | `src/renderer/HitTester.cpp` | Frame/point → HitResult | Ingen navigasjonssideeffekt | Implemented |
| 6 | `InlineLayout::layout` | `LinkMarker::make` | `src/renderer/LinkMarker.cpp` | Lenke/font → markør-run | Ingen I/O; syntetisk source-range | Implemented |
| 7 | `MarkdownRenderer::layout` | `PageComposer::compose` | `src/renderer/PageComposer.cpp` | FlowLayout + PaperSpec → PageLayout | begrens store blokker | Planned |

## 6. Gjenbruk og avhengigheter

Ingen andre functionality-kontrakter konsumeres; delte datatyper følger arkitekturen.

Preview bruker layout; FOX-host bruker hitTest; mappingtjenesten leser RenderFrame. Renderer skal ikke vokse til applikasjonskontroller. Nye primitiver krever kontraktreview.

## 7. Verifikasjon

Relevante akseptanse-ID-er: AT-005, AT-002, AT-008, AT-011, AT-013, AT-015, AT-018, AT-019, AT-021, AT-023.

`RendererTest` bruker deterministiske fontmål og kontrollerer wrapping, fontstiler, UTF-8, dekorasjoner, ankere og lenketreff. PresentationTest bruker ekte FOX-mål.

Evidence: [Fase P3](../../../docs/evidence/P3.md). Samlet kravdekning og eventuelle gjenstående begrensninger kontrolleres i P7; Implemented er ikke automatisk Verified.

Ny regresjonskontroll: [Native lenker og markører](../../../docs/evidence/document-links.md).

Utvidelsen krever AT-031, AT-032, AT-036, AT-039. Dette er planlagt dekning, ikke nye testbevis.

## 8. Status, risiko og endringskonsekvenser

**Proposed 1.1:** Historisk Implemented/evidence nedenfor gjelder baseline. Nye kontrakter er beskrevet i [designrevisjonen](../../../softwareDesign.md); gamle bevis verifiserer ikke disse.


Implemented i P3. Oppdater kontrakter, kallkart, konsumenter og tester i samme endring.
Rene porter og tydelig rolleeierskap er obligatorisk. Eventuelle senere avvik står i fasens bevisrapport.
