---
id: FUNC-017
kind: Functionality
audience: System
role: Service
owner: renderer
status: Implemented
scope: FirstRelease
requirements: UR-040, UR-017, UR-018, SR-001, SR-008, SR-009, SR-016, SR-019
uses: none
---

# Functionality-017: Sidekomposisjon og paginering

## 1. Hensikt og avgrensning

Dele en målt dokumentflyt i fysiske sider med eksplisitte kildeankre. Layouten er gjenbrukbar for preview og eksport og har ingen fil-I/O eller FOX-binding.

## 2. Krav og akseptanse

UR-017, UR-018, SR-001, SR-008, SR-009, SR-016, SR-019. Definisjoner: [krav](../../../xfmd_requirements.md).
Akseptanse: AT-011, AT-018, AT-019, AT-031, AT-032, AT-036, AT-039.

## 3. Kontrakter og eierskap

`PageComposer::compose(RenderFrame&, PaperSpec, cancelled)` fyller `PageLayout` fra frame.flow med sidebokser, paginerte glyph-/dekorasjonsreferanser og ankerregioner. FUNC-004 produserer FlowLayout og kaller tjenesten; PageComposer kaller ikke FUNC-004 tilbake. Kontraktene bor i `src/contracts/PageLayout.h` og `LayoutProfile.h`.

`LayoutUnit` er double points, 1/72 inch; mm konverteres med 72/25.4. A4 er 210×297 mm, omtrent 595.276×841.890 pt. Første profil er portrett med 20 mm marger. Alle marger må være endelige og gi positivt innholdsområde. `LayoutMode` er Continuous eller Paged; continuous er én logisk flyt. Page gap/skygge og zoom er viewer-dekorasjon utenfor sidemodellen.

`PageAnchor{SourceRange,pageIndex,bounds,quality}` bruker uendrede original-byteoffsets. Flere visuelle deler kan peke til samme kildelinje. FrameKey inkluderer profil/fontsett; paper mode har ingen viewportbredde som layoutavhengighet.

## 4. Atferd, tilstand og feil

Brødtekst brytes til innholdsbredde. Kode bevarer whitespace, men visual-wrap i A4 må kunne dele overlang linje ved glyph-clustergrense; kilden endres ikke. Overskrifter beholdes med neste innholdslinje når det er plass; avsnitt forsøker minst to linjer per side ved deling. Lange lister/kodeblokker deles ved visuelle linjer, ikke klippes som store blokker. For høy linje/udelelig primitiv som ikke får plass gir eksplisitt layoutfeil fremfor stille klipping. Tomt dokument gir én blank A4-side. Ingen sidesplitt inne i cluster. Marger og blanke mellomrom bindes til nærmeste gyldige kildeanker; paper chrome har ingen source-range.

## 5. Plumbing

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `MarkdownRenderer::layout Paged` | `PageComposer::compose` | `src/renderer/PageComposer.cpp` | målt flyt + papir → sider | valider innholdsmål/ressurser | Implemented |
| 2 | `PageComposer::compose` | `PageBreaker::breakLines` | `src/renderer/PageBreaker.cpp` | visuelle linjer/keep-regler → sidegrenser | stor blokk splittes; udelelig overflow feiler | Implemented |
| 3 | `PageComposer::compose` | `PageAnchorIndex::build` | `src/renderer/PageAnchorIndex.cpp` | flytankre → sideankre | behold source og mappingkvalitet | Implemented |

| 99 | `MarkdownRenderer::layout` | `DiagramPlacement::append` | `src/renderer/diagram/DiagramPlacement.cpp` | Implementert diagramutvidelse | Blokkfeil og stale-data følger Mermaid-designet | Implemented |

## 6. Gjenbruk og avhengigheter

FUNC-004 bruker denne functionality; FUNC-005/009/018 konsumerer PageLayout-data. Eksport får hele dokumentet, ikke et screenshot av viewport. Ingen separat Markdown-/PDF-layoutmotor.

## 7. Verifikasjon

Fixtures for tom side, eksakt sidegrense, heading ved bunn, lang kode/listenesting og Unicode. Samme papirprofil gir identisk paginering ved resize/zoom/fullscreen. Sidegap og begge sync-retninger testes med FUNC-009.

AT-011, AT-018, AT-019, AT-031, AT-032, AT-036, AT-039: se [P11](../../../docs/evidence/P11.md).

## 8. Status, risiko og endringskonsekvenser

Revisjon 1.1, 2026-09-13. Kallene er implementert i P11-M2.
P11. PageComposer skal bruke en flyt med eksplisitte visuelle linjer; dagens flate DrawRun-liste alene gir ikke robuste sideskift. Ressursgrenser/målinger avklares i P9 før Ready.
[Integrasjonsdesign](../../../softwareDesign.md) og [faseplan](../../../implementationPlan.md) gir kontekst.

P21: PageComposer::compose kopierer DecorationRole sammen med color når dekorasjoner
deles over sider. Det lar A4-skjermen bruke samme lesepalett som continuous, mens
PDF beholder opprinnelige farger. Sidegeometri og tekstplassering endres ikke.
ReadingColorsTest kontrollerer alle dekorasjonsroller etter sidekomposisjon.

P25 (Proposed): Diagrammet er udelelig sideblokk; flytt/skaler til tilgjengelig hel side uten å klippe, og behold samme transform for tekst og treff. Krav: UR-040; AT-060.
Se [design](../../../docs/design/mermaid-integration.md). Eksisterende Implemented-rader
og eldre bevis gjelder baseline; ny plumbing er ikke implementert eller testet.
