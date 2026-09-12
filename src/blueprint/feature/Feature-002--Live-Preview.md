---
id: FTR-002
kind: Feature
audience: User
role: Workflow
owner: application
status: Proposed
scope: FirstRelease
requirements: UR-003, UR-004, UR-009, SR-002, SR-008, SR-010, SR-011, SR-012, SR-013
uses: FUNC-001, FUNC-003, FUNC-004, FUNC-005, FUNC-006, FUNC-007, FUNC-011
---

# Feature-002: Revisjonssikker live preview

## 1. Hensikt og avgrensning

Brukeren redigerer Markdown og ser en oppdatert framstilling etter en kort pause uten å miste fokus eller endringer. Feature-grensen er den sammenhengende reaksjonen på redigering. Splitter og save er functionality, ikke feature-eide spesialløsninger.

## 2. Krav og akseptanse

Krav: UR-003, UR-004, UR-009, SR-002, SR-008, SR-010, SR-011, SR-012, SR-013. Definisjoner og normativ akseptanse finnes i
[kravspesifikasjonen](../../../xfmd_requirements.md). Kapittel 7 konkretiserer beviset.

## 3. Kontrakter og eierskap

Inngang er bekreftet Edit/Revision. Utgang er gyldig frame for nyeste revisjon eller synlig stale/feilstatus. Bruk EditController og PreviewCoordinator; ingen ny LivePreviewManager. Offentlige kontrakter eies av konsumert functionality.

## 4. Atferd, tilstand og feil

Hver edit oppdaterer dirty og restarter 300 ms debounce. Undo/redo går samme vei. Når timeren leveres, brukes aktivt snapshot. Dokumentbytte kansellerer gammelt arbeid. Feil beholder editorinnhold og gjør gammel preview ikke-interaktiv. Save endrer baseline men skal ikke i seg selv skape ny tekstedit. Skjult preview kan utsette layout til den blir synlig.

## 5. Plumbing

Alle symboler og kildefiler i tabellen er **planlagte**, ikke implementert kode.
Bibliotekskall verifiseres mot valgt dependency-versjon før implementering.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `EditorWidget::onChanged` | `EditController::applyEdit` | `src/application/document/EditController.cpp` | Edit → ny Revision | FUNC-011 hindrer projection-loop. | Planned |
| 2 | `EditController::applyEdit` | `PreviewCoordinator::schedule` | `src/application/preview/PreviewCoordinator.cpp` | Revision → preview-token | Ny edit erstatter frist. | Planned |
| 3 | `PreviewCoordinator::schedule` | `FoxScheduler::restart` | `src/application/adapters/FoxScheduler.cpp` | 300 ms → pending callback | FUNC-006 håndterer cancel/levetid. | Planned |
| 4 | `FoxScheduler::onTimeout` | `PreviewCoordinator::refresh` | `src/application/preview/PreviewCoordinator.cpp` | Token → snapshot/parse/layout | FUNC-007 forkaster feil token. | Planned |
| 5 | `PreviewCoordinator::refresh` | `FoxRenderHost::present` | `src/application/adapters/FoxRenderHost.cpp` | Nyeste frame → visning | Fokus/markør skal ikke flyttes. | Planned |

## 6. Gjenbruk og avhengigheter

[FUNC-001](../functionality/Functionality-001--Document-Session.md), [FUNC-003](../functionality/Functionality-003--Markdown-Interpretation.md), [FUNC-004](../functionality/Functionality-004--Render-Layout.md), [FUNC-005](../functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-006](../functionality/Functionality-006--Event-Scheduling.md), [FUNC-007](../functionality/Functionality-007--Preview-Pipeline.md), [FUNC-011](../functionality/Functionality-011--Text-Editing.md)

Samme tolkning/layout/host som FTR-001. Timer kan gjenbrukes av andre kravfestede operasjoner. Nye redigeringshandlinger skal inn via FUNC-011.

## 7. Verifikasjon

AT-003, AT-004, AT-009, AT-012, AT-018, AT-020, AT-021, AT-022, AT-023: klokkesekvens 0/100/250→550 ms, kontinuerlig typing, undo/redo, dirty-save, dokumentbytte og parsefeil. Planlagt `tests/acceptance/LivePreviewTest.cpp` med fake clock og FOX-fokuskontroll.

Bevis: ingen applikasjonstest kjørt; testfiler ovenfor er planlagte. Ved implementering
oppgis kommando, fixture, miljø, commit og faktisk utfall. Strukturkontroll alene
oppfyller ikke atferdskravene.

## 8. Status, risiko og endringskonsekvenser

Proposed, revisjon 0.1, 2026-09-12. SR-011-ytelse er foreslått og ikke målt. Timeren lover start etter stillhet, ikke ikke-blokkerende parsing. P0 avgjør om single-threaded strategi holder.

Ved endret offentlig kontrakt: oppdater konsumentene i registeret, dette kallkartet,
berørte krav og kontrakttester i samme endring. Før status Ready skal relevante
P0-spørsmål være avgjort; før Verified skal kapittel 7 inneholde testbevis.
