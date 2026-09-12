---
id: FUNC-007
kind: Functionality
audience: System
role: Workflow
owner: application
status: Proposed
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

`PreviewCoordinator::schedule(Revision)`, `refresh(PreviewToken)`, `relayout(LayoutRequest)`, `invalidate(DocumentId)`. Token består av dokument-ID, revisjon og relevant generasjon. Koordinatoren eier cached SemanticDocument og siste publiserte frame, men ikke dokumentteksten eller FOX-parenting.

## 4. Atferd, tilstand og feil

Åpning kjører refresh uten debounce. Edits går via timer. Refresh tar snapshot og kaller parser/layout, kontrollerer token før publisering og erstatter frame atomisk. Resize bruker cached modell bare hvis revisjonen fortsatt stemmer. Ved feil beholder editoren teksten; gammelt frame merkes stale og mister lenke/sync-interaksjon. Skjult preview utsetter layout til panelet vises, men modellen kan fortsatt være gyldig.

## 5. Plumbing

Alle symboler og kildefiler i tabellen er **planlagte**, ikke implementert kode.
Bibliotekskall verifiseres mot valgt dependency-versjon før implementering.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `DocumentOpened / EditController::applyEdit` | `PreviewCoordinator::schedule / refresh` | `src/application/preview/PreviewCoordinator.cpp` | Revisjon eller åpnet snapshot | Åpning straks; edits debounces. | Planned |
| 2 | `PreviewCoordinator::schedule` | `FoxScheduler::restart` | `src/application/adapters/FoxScheduler.cpp` | 300 ms + PreviewToken | Ny edit erstatter planlagt arbeid. | Planned |
| 3 | `PreviewCoordinator::refresh` | `DocumentSession::snapshot` | `src/application/document/DocumentSession.cpp` | Aktiv ID/revisjon → eid snapshot | Foreldet token avvises. | Planned |
| 4 | `PreviewCoordinator::refresh` | `IInterpreter::parse` | `src/contracts/IInterpreter.h` | Snapshot → SemanticDocument | Parsefeil markerer preview stale. | Planned |
| 5 | `PreviewCoordinator::refresh / relayout` | `IRenderer::layout` | `src/contracts/IRenderer.h` | Modell + bredde + metrics → frame | Gammel modell/generasjon kan ikke publiseres. | Planned |
| 6 | `PreviewCoordinator::refresh / relayout` | `FoxRenderHost::present` | `src/application/adapters/FoxRenderHost.cpp` | Kontrollert frame → visning | Publiserer FrameReady for scroll-restore. | Planned |

## 6. Gjenbruk og avhengigheter

[FUNC-001](../functionality/Functionality-001--Document-Session.md), [FUNC-003](../functionality/Functionality-003--Markdown-Interpretation.md), [FUNC-004](../functionality/Functionality-004--Render-Layout.md), [FUNC-005](../functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-006](../functionality/Functionality-006--Event-Scheduling.md)

FTR-001 og FTR-002 bruker denne tjenesten. View mode/resize bruker relayout. Framtidig IPC skal levere nye snapshots gjennom dokumentlaget, ikke omgå pipeline.

## 7. Verifikasjon

AT-002, AT-004, AT-011, AT-012, AT-013, AT-018, AT-020, AT-021, AT-023: fake ports beviser riktig rekkefølge; out-of-date token, resize uten parsing og parserfeil. Planlagt `tests/application/PreviewCoordinatorTest.cpp`.

Bevis: ingen applikasjonstest kjørt; testfiler ovenfor er planlagte. Ved implementering
oppgis kommando, fixture, miljø, commit og faktisk utfall. Strukturkontroll alene
oppfyller ikke atferdskravene.

## 8. Status, risiko og endringskonsekvenser

Proposed, revisjon 0.1, 2026-09-12. Single-threaded første design. Revisionskontroll er nødvendig også uten workers pga. køede timer/events. Ikke påstå at debounce gjør parsing ikke-blokkerende.

Ved endret offentlig kontrakt: oppdater konsumentene i registeret, dette kallkartet,
berørte krav og kontrakttester i samme endring. Før status Ready skal relevante
P0-spørsmål være avgjort; før Verified skal kapittel 7 inneholde testbevis.
