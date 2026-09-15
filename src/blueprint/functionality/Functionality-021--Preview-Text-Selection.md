---
id: FUNC-021
kind: Functionality
audience: System
role: Mechanism
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-041, UR-035
uses: none
---

# Functionality-021: Preview-Text-Selection

## 1. Hensikt og avgrensning

Merke og kopiere lesetekst fra immutable render-frame.

## 2. Krav og akseptanse

UR-035. Normativ akseptanse i [kravene](../../../xfmd_requirements.md).

## 3. Kontrakter og eierskap

Renderer legger logiske lesetekst-offsets på DrawRun. PreviewSelection bruker glyph-clustere for hit-testing og markeringsrektangler. FOX-host eier clipboard-snapshot og input; ingen parsing her.

## 4. Atferd, tilstand og feil

Ny dokumentrevisjon nullstiller merking; fargeendring bevarer merking. Clipboard-snapshot overlever ny frame. Drag kansellerer lenkeklikk.

## 5. Plumbing

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `FoxRenderHost input` | `PreviewSelection::hit` | `src/application/preview/PreviewSelection.cpp` | Dokumentpunkt → UTF-8-posisjon | Avvis ugyldig input | Implemented |
| 2 | `FOX clipboard request` | `FoxRenderHost::onClipboardRequest` | `src/application/adapters/FoxPreviewInput.cpp` | Eid tekst → UTF-8 clipboard | Avvis ugyldig input | Implemented |

| 99 | `BlockLayout::layout` | `DiagramPlacement::append` | `src/renderer/diagram/DiagramPlacement.cpp` | Diagrametiketter i logisk leserekkefølge | Blokkfeil og stale-data følger Mermaid-designet | Implemented |

## 6. Gjenbruk og avhengigheter

Konsument: Markdown-presentasjon. Rene kontrakter beholder laggrensene.

## 7. Verifikasjon

AT-055. Planlagt: målrettede unit- og native FOX-tester samt full regresjon.

## 8. Status, risiko og endringskonsekvenser

Implemented i brukerbestilt P23. M3 samler testbevis og kjente grenser.

P23: [testbevis og visuell kontroll](../../../docs/evidence/P23.md).

P25 (Proposed): Diagrametiketter inngår i normal readingText med eksplisitt leserekkefølge og skalerte DrawRuns; eksisterende PreviewSelection og clipboard brukes. Krav: UR-041; AT-061.
Se [design](../../../docs/design/mermaid-integration.md). Eksisterende Implemented-rader
og eldre bevis gjelder baseline; ny plumbing er ikke implementert eller testet.

P28: glyph-advance multipliseres med DrawRun.textScale ved hit-testing og
markeringsrektangler. MermaidGuiTest viser native drag, PRIMARY, Ctrl+C og
Ctrl+A; DiagramReadingTest kontrollerer skalerte grenser og Approximate-anker.
