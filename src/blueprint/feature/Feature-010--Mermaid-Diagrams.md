---
id: FTR-010
kind: Feature
audience: User
role: Workflow
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-039, UR-040, UR-041, SR-021, SR-022, SR-023, SR-024
uses: FUNC-003, FUNC-004, FUNC-005, FUNC-007, FUNC-016, FUNC-017, FUNC-018, FUNC-021, FUNC-023, FUNC-024, FUNC-025
---

# Feature-010: Mermaid-diagrammer

## 1. Hensikt og avgrensning

UC-008: lese, redigere og kopiere diagramkilde og publisere dem i PDF. Samlet brukerresultat med syntax-/feilpolicy og akseptanse; ingen egen featureklasse. Første profil er flowchart/graph, ikke alle Mermaid-dialekter.

## 2. Krav og akseptanse

UR-039–041 og SR-021–023; AT-059–064. Se [design og kontrakter](../../../docs/design/mermaid-integration.md) og [krav](../../../xfmd_requirements.md).

## 3. Kontrakter og eierskap

Feature bruker IDiagramInterpreter, IDiagramLayout og DiagramPreparation. DiagramModel er semantisk verdi, DiagramScene er visuell verdi; Application eier jobber og FOX/Cairo. cmark beholder Markdown-rollen. Ingen feature kaller interne metoder i andre features.

## 4. Atferd, tilstand og feil

Edit → eksisterende debounce → tolkning → forberedelse → normal dokumentlayout → presentasjon. Kun komplett svar for riktig token/font/profil publiseres. Blokkfeil gir kildetekst og forklaring; resten av dokumentet overlever. Fargebytte er repaint. PDF bruker samme scene. Bibliotekets interne typer krysser aldri laggrensen.

## 5. Plumbing

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `ModelBuilder::appendNode` | `MermaidBlockBuilder::build` | `src/interpreter/mermaid/MermaidBlockBuilder.cpp` | mermaid-gjerde → kilde og modell | vanlig kode bevares | Implemented |
| 2 | `MermaidBlockBuilder::build` | `IDiagramInterpreter::parse` | `src/contracts/diagram/IDiagramInterpreter.h` | UTF-8 → DiagramModel/diagnostic | lokal fallback | Implemented |
| 3 | `DiagramServices prepare chain` | `DiagramPreparation::prepare` | `src/application/diagrams/DiagramPreparation.cpp` | modell + request key → forberedt scene | stale/cancel forkastes | Implemented |
| 4 | `DiagramPreparation::prepare` | `IDiagramLayout::layout` | `src/contracts/diagram/IDiagramLayout.h` | ren modell + målte etiketter → scene | ingen kildeparsing | Implemented |
| 5 | `BlockLayout::layout` | `DiagramPlacement::append` | `src/renderer/diagram/DiagramPlacement.cpp` | SVG-scene → visual-run og source anchor | bevar aspekt og sidegrenser | Implemented |
| 6 | `DisplayListPainter::paint` | `DiagramPainter::paint` | `src/application/adapters/DiagramPainter.cpp` | bibliotekets SVG + palett → librsvg/Cairo | samme primitive kontrakt for preview/PDF | Implemented |


## 6. Gjenbruk og avhengigheter

Gjenbruk FUNC-003/004/005/007 for Markdown, layout, host og scheduling; FUNC-016/017/018 for tekst og PDF; FUNC-021 for markering. Nye eiere: [tolkning](../functionality/Functionality-023--Mermaid-Interpretation.md), [diagramlayout](../functionality/Functionality-024--Diagram-Layout.md), [forberedelse](../functionality/Functionality-025--Diagram-Preparation.md).

## 7. Verifikasjon

AT-059–064 dekkes av Rust-profil-/ABI-kontroller, DiagramLayoutTest, DiagramPreparationTest, DiagramWorkerTest, DiagramReadingTest, MermaidGuiTest og MermaidPdfTest. Faktiske kjøringer og begrensninger står i [P29](../../../docs/evidence/P29.md).

## 8. Status, risiko og endringskonsekvenser

P31/P32: [Gjeldende SVG-/rutebeslutning](../../../docs/design/mermaid-svg-routing.md) erstatter tidligere native etiketttegning. Historiske tester nedenfor gjelder P25–P30; ny atferd er implementert; P31/P32-bevis beskriver faktisk verifikasjon.

Implementert i P26/P27. Commit/toolchain/patch er låst. P28/P29 samler lese-, eksport- og full profilverifikasjon; ikke full Mermaid-kompatibilitet.

Akseptanse: AT-059, AT-060, AT-061, AT-062, AT-063, AT-064.

AT-065 dekkes av P32 og forkens rapport.

Gjeldende P31/P32-verifikasjon: [samlet testbevis](../../../docs/evidence/P32.md).

P36: [Typed dekning og Sequence 1](../../../docs/design/mermaid-coverage.md)
utvider samme porter. Sequence 1 er Implemented; verifikasjon dokumenteres separat.
