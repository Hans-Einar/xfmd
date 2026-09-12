---
id: FUNC-007
kind: Functionality
audience: System
role: Workflow
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-002, UR-004, SR-001, SR-002, SR-003, SR-008, SR-010, SR-011, SR-013
uses: FUNC-001, FUNC-003, FUNC-004, FUNC-005, FUNC-006
---

# Functionality-007: Preview-orkestrering

## 1. Hensikt og avgrensning

Koble dokument, interpreter, renderer og host sammen på ett sted. Ingen parserregler eller layoutmatematikk her. Både første visning og live preview bruker samme pipeline.

## 2. Krav og akseptanse

Krav: UR-002, UR-004, SR-001, SR-002, SR-003, SR-008, SR-010, SR-011, SR-013. Definisjoner og normativ akseptanse finnes i
[kravspesifikasjonen](../../../xfmd_requirements.md). Kapittel 7 konkretiserer beviset.

## 3. Kontrakter og eierskap

PreviewCoordinator eier cached immutable model, viewportbredde og generasjon. ParserWorker eier én tråd med maksimalt én aktiv og én pending snapshot. IInterpreter brukes bare av worker; IRenderer/ITextMetrics brukes på GUI-tråden. present/invalidated/failed er injiserte callbacks.

## 4. Atferd, tilstand og feil

schedule invaliderer med en gang og debouncer 300 ms. refresh tar snapshot og sender jobb. Worker ticket hindrer publisering av superseded resultat; poll kontrollerer også aktivt dokumenttoken. Resize gjør kun layout av riktig cached model. Feil gir status; gammel preview er ikke interaktiv. Destructor kansellerer timere før worker join og før FOX-host slettes.

## 5. Plumbing

Tabellen beskriver implementerte kall. Navngitte hendelser er injiserte callbacks, ikke en global event bus.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `EditController changed callback` | `PreviewCoordinator::schedule` | `src/application/preview/PreviewCoordinator.cpp` | Revision → invalidering/debounce | Kun nyeste edit startes | Implemented |
| 2 | `FoxScheduler timeout / DocumentOpened` | `PreviewCoordinator::refresh` | `src/application/preview/PreviewCoordinator.cpp` | Snapshot → worker-submit | Kanseller gammel debounce | Implemented |
| 3 | `PreviewCoordinator::refresh` | `ParserWorker::submit` | `src/application/preview/ParserWorker.cpp` | Snapshot → latest pending | Overskriv kun pending, aldri aktiv data | Implemented |
| 4 | `ParserWorker::run` | `IInterpreter::parse` | `src/contracts/IInterpreter.h` | Snapshot → immutable model | Unntak fanges i worker | Implemented |
| 5 | `PreviewCoordinator::poll` | `ParserWorker::take` | `src/application/preview/ParserWorker.cpp` | Completion → tokencheck | Gammelt svar forkastes | Implemented |
| 6 | `PreviewCoordinator::relayout` | `IRenderer::layout` | `src/contracts/IRenderer.h` | Model + metrics + generation → frame | Kun GUI-tråd | Implemented |
| 7 | `PreviewCoordinator present callback` | `FoxRenderHost::present` | `src/application/adapters/FoxRenderHost.cpp` | Frame → visning | Riktig token/bredde kreves | Implemented |

## 6. Gjenbruk og avhengigheter

[FUNC-001](../functionality/Functionality-001--Document-Session.md), [FUNC-003](../functionality/Functionality-003--Markdown-Interpretation.md), [FUNC-004](../functionality/Functionality-004--Render-Layout.md), [FUNC-005](../functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-006](../functionality/Functionality-006--Event-Scheduling.md)

FTR-001 og FTR-002 bruker denne tjenesten. View mode/resize bruker relayout. Framtidig IPC skal levere nye snapshots gjennom dokumentlaget, ikke omgå pipeline.

## 7. Verifikasjon

Relevante akseptanse-ID-er: AT-002, AT-004, AT-011, AT-012, AT-013, AT-018, AT-020, AT-021, AT-023.

`PreviewTest` tester debounce, feil, resize uten reparse, latest-job replacement og at metrics kjøres på opprettende tråd. PresentationTest kontrollerer faktisk live preview, fokus og cursor.

Evidence: [Fase P4](../../../docs/evidence/P4.md). Samlet kravdekning og eventuelle gjenstående begrensninger kontrolleres i P7; Implemented er ikke automatisk Verified.

## 8. Status, risiko og endringskonsekvenser

Implemented i P4. Oppdater kontrakter, kallkart, konsumenter og tester i samme endring.
Rene porter og tydelig rolleeierskap er obligatorisk. Eventuelle senere avvik står i fasens bevisrapport.
