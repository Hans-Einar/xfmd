---
id: FUNC-016
kind: Functionality
audience: System
role: Adapter
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-040, UR-041, UR-036, UR-037, UR-030, UR-031, UR-002, UR-017, UR-018, SR-001, SR-010, SR-016
uses: none
---

# Functionality-016: Delt typografi og tegnegrunnlag

## 1. Hensikt og avgrensning

Gi renderer rene tekstmål og formede glypher, og utføre det samme tegnede dokumentet på skjerm og PDF. Trekk felles font-/tegneansvar ut av FOX-host slik at fontmåling og PDF-fontvalg ikke divergerer.

## 2. Krav og akseptanse

UR-002, UR-017, UR-018, SR-001, SR-010, SR-016. Definisjoner: [krav](../../../xfmd_requirements.md).
Akseptanse: AT-002, AT-011, AT-020, AT-031, AT-032, AT-036.

## 3. Kontrakter og eierskap

`ITextShaper::shape(text, FontSpec)` med `ITextMetrics::fontSetId()` returnerer rene `ShapedText`/`GlyphSegment`-data med UTF-8-clusters, fontface-id, glyph-id, advances og offsets i points. ITextMetrics::measure bruker de samme shaped resultatene. `FontCatalog` eier konkret fontidentitet/fallback; frame inneholder bare FontSetId og rene glyphdata. FontCatalog holder native fontreferanser i sin tråd. Immutable frame bærer fontfilidentitet; en ny eksportkatalog avviser endrede/manglende fontfiler i stedet for stille fallback. Ingen FOX-, Pango- eller Cairo-peker krysser contracts.

Valgt backend etter P9 er Cairo + PangoCairo/Fontconfig for shaping/fallback, i application-adaptere. `DisplayListPainter::paint(frame, target)` bruker samme glyphplassering på en skjermoverflate i FOX-host og på en PDF-overflate. Renderer bestemmer layout/primitiver; adapteren utfører dem. FOX forblir toolkit; ingen GTK-widget eller browser engine innføres.

## 4. Atferd, tilstand og feil

P9/P11/P12 dokumenterer fontfallback, glyph-/clusteruttrekk, PDF-fontembedding/tekstuttrekk og målekonsistens. Stille fontbytte mellom preview og eksport er feil: rapporter manglende font og bygg begge på nytt med nytt FontSetId. Fontcache er bounded; fontressurser/contexts er trådeide. GUI eier FOX og skjermoverflate. Eksport-worker får egne output-contexts og immutable data, aldri FXFont. Legacy FoxTextMetrics er fjernet etter continuous-regresjonene. RenderFrame eier en flat liste med ekstra glyphfragmenter; DrawRun holder indeks/lengde. Samme immutable shaping gjenbrukes uten å allokere en vector per linjefragment.

## 5. Plumbing

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `InlineLayout::layout` | `ITextShaper::shape` | `src/contracts/ITextShaper.h` | UTF-8/fontsett → glypher/clusters | unsupported glyph markeres likt i begge mål | Implemented |
| 2 | `ITextShaper adapter` | `SharedTextMetrics::shape` | `src/application/adapters/SharedTextMetrics.cpp` | ren kontrakt → konkret shaping | fontcatalog/cache eies i application | Implemented |
| 3 | `FoxRenderHost paint / PDF output` | `DisplayListPainter::paint` | `src/application/adapters/DisplayListPainter.cpp` | frame + target → samme glyphplassering | targetfeil rapporteres | Implemented |
| 20 | `FoxRenderHost::onPaint` | `ReadingPalette::from` | `src/application/preferences/ReadingColors.cpp` | Lesefarger → repaint/profil | Ingen dokumentmutasjon | Implemented |


P23: DisplayListPainter tegner CairoVisual i samme adapter som tekst. Matematikkressursen brukes som vektormaske med aktuell tekstfarge; bilder tegnes i egne farger. PDF bruker samme ressurs uten skjermpalett.

| 99 | `DisplayListPainter::paint` | `DiagramPainter::paint` | `src/application/adapters/DiagramPainter.cpp` | DiagramScene og semantisk palett | Blokkfeil og stale-data følger Mermaid-designet | Implemented |

## 6. Gjenbruk og avhengigheter

Konsumenter: FUNC-004, FUNC-005 og FUNC-018. Samme shapingkontrakt kan brukes av en erstattet renderer; interpreter påvirkes ikke. Editorens FXText-font forblir separat.

## 7. Verifikasjon

Latin/æøå, CJK-fallback, sammensatte tegn, fet/kursiv og monospace testes. Sammenlign font-id, advances, linjebrudd og PDF-uttrekk; raster sammenlignes med avtalt antialias-toleranse, ikke krav om identiske skjermpiksler.

AT-002, AT-011, AT-020, AT-031, AT-032, AT-036: se [P11](../../../docs/evidence/P11.md).

## 8. Status, risiko og endringskonsekvenser

Revisjon 1.1, 2026-09-13. Kallene er implementert i P11-M1.
P9 teknisk gate, P11 migrering, P12 PDF-konsument. P12 beviser samsvar med uavhengig PDF-leser, og P13 kontrollerer den endelige tegnestien.
[Integrasjonsdesign](../../../softwareDesign.md) og [faseplan](../../../implementationPlan.md) gir kontekst.

P21 utvider samme eier med UR-030/031, AT-050, AT-051. ReadingColors er rene
lesepreferanser; PreviewColorControls bruker UiRow/UiContext. Bare FOX-host gir
DisplayListPainter en skjermpalett; PDF beholder standardfargene. DecorationRole
bevarer semantisk rolle gjennom PageComposer, uten FOX-typer i renderer/kontrakter.
Profiler lagres additivt i ReadingLight/ReadingDark via eksisterende preferences-service.
Live endring er repaint; commit ved release, med rollback ved skrivefeil.

P21: [AT-050/051, regresjoner og skjermbilder](../../../docs/evidence/P21.md).

P23 akseptanse: AT-056, AT-057. Tester: RichPreviewTest og PreviewSelectionGuiTest.

P25 (Proposed): Diagramtekst formes med trådeid SharedTextMetrics. Vanlige DrawRuns beholdes; DiagramPainter utfører bare rene stier med semantisk paintrolle. Krav: UR-040, UR-041; AT-060, AT-061.
Se [design](../../../docs/design/mermaid-integration.md). Eksisterende Implemented-rader
og eldre bevis gjelder baseline; ny plumbing er ikke implementert eller testet.

P28 DiagramReadingTest kontrollerer nodefyll ved light/dark/egendefinert
palett på samme frame. Formen tegnes i Cairo; tekst bruker vanlig DrawRun.
