---
id: FTR-002
kind: Feature
audience: User
role: Workflow
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-003, UR-004, UR-009, SR-002, SR-008, SR-010, SR-011, SR-012, SR-013, SR-019
uses: FUNC-001, FUNC-003, FUNC-004, FUNC-005, FUNC-006, FUNC-007, FUNC-011
---

# Feature-002: Revisjonssikker live preview

## 1. Hensikt og avgrensning

Brukeren redigerer Markdown og ser en oppdatert framstilling etter en kort pause uten å miste fokus eller endringer. Feature-grensen er den sammenhengende reaksjonen på redigering. Splitter og save er functionality, ikke feature-eide spesialløsninger.

## 2. Krav og akseptanse

Krav: UR-003, UR-004, UR-009, SR-002, SR-008, SR-010, SR-011, SR-012, SR-013. Definisjoner og normativ akseptanse finnes i
[kravspesifikasjonen](../../../xfmd_requirements.md). Kapittel 7 konkretiserer beviset.

## 3. Kontrakter og eierskap

Live preview bruker EditController, PreviewCoordinator, IScheduler/FOX-adapter og ParserWorker. Ingen egen featureklasse eller separat renderer. Worker-resultater og frame har dokument/revisjon, layout har generasjon.

**Implementert utvidelse 1.1 (P11):** Live preview validerer full FrameKey, også papirprofil/fontsett. Eksport får separat livssyklus og bruker ikke denne featurens private worker.

## 4. Atferd, tilstand og feil

Etter siste edit går 300 ms før snapshot sendes til parser-worker. Ny pending jobb erstatter eldre. Bare nyeste resultat går til FOX-måling/layout. Undo/redo bruker samme vei. Markør/fokus beholdes. Feil, dokumentbytte og teardown gir ingen gammel interaktiv preview eller dangling callback.

## 5. Plumbing

Implemented-rader beskriver gjeldende plumbing; historiske fasebevis identifiserer tidligere baseline. Navngitte hendelser er injiserte callbacks, ikke en global event bus.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `EditorWidget::onChanged` | `EditController::applyProjectedText` | `src/application/document/EditController.cpp` | FXText → edit/revisjon | Projeksjon korrigeres ved feil | Implemented |
| 2 | `EditController changed callback` | `PreviewCoordinator::schedule` | `src/application/preview/PreviewCoordinator.cpp` | Ny revisjon → debounce | Stale preview deaktiveres | Implemented |
| 3 | `PreviewCoordinator::schedule` | `FoxScheduler::restart` | `src/application/adapters/FoxScheduler.cpp` | 300 ms → refresh | Frist flyttes ved ny edit | Implemented |
| 4 | `PreviewCoordinator::refresh` | `ParserWorker::submit` | `src/application/preview/ParserWorker.cpp` | Snapshot → latest-job queue | Ingen GUI-kall i worker | Implemented |
| 5 | `PreviewCoordinator::poll` | `PreviewCoordinator::relayout` | `src/application/preview/PreviewCoordinator.cpp` | Gjeldende resultat → layout | Gamle tokens forkastes | Implemented |
| 6 | `PreviewCoordinator present callback` | `FoxRenderHost::present` | `src/application/adapters/FoxRenderHost.cpp` | Gyldig frame → preview | Fokus og cursor beholdes | Implemented |
| 7 | `Preview completed` | `PreviewCoordinator::acceptFrame` | `src/application/preview/PreviewCoordinator.cpp` | FrameKey → present | avvis gammel profil | Implemented |

## 6. Gjenbruk og avhengigheter

[FUNC-001](../functionality/Functionality-001--Document-Session.md), [FUNC-003](../functionality/Functionality-003--Markdown-Interpretation.md), [FUNC-004](../functionality/Functionality-004--Render-Layout.md), [FUNC-005](../functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-006](../functionality/Functionality-006--Event-Scheduling.md), [FUNC-007](../functionality/Functionality-007--Preview-Pipeline.md), [FUNC-011](../functionality/Functionality-011--Text-Editing.md)

Samme tolkning/layout/host som FTR-001. Timer kan gjenbrukes av andre kravfestede operasjoner. Nye redigeringshandlinger skal inn via FUNC-011.

## 7. Verifikasjon

Relevante akseptanse-ID-er: AT-003, AT-004, AT-009, AT-012, AT-018, AT-020, AT-021, AT-022, AT-023.

`PreviewTest` og utvidet `PresentationTest` passerer. Ytelsesbudsjettet måles samlet i P7; worker beskytter eventflyten mot parsing, ikke mot ubegrenset layout.

Evidence: [Fase P4](../../../docs/evidence/P4.md). Samlet kravdekning og eventuelle gjenstående begrensninger kontrolleres i P7; Implemented er ikke automatisk Verified.

Utvidelsen krever AT-039. Dette er planlagt dekning, ikke nye testbevis.

## 8. Status, risiko og endringskonsekvenser

**Implemented 1.1:** [P11-bevis](../../../docs/evidence/P11.md) beskriver ny kode og kontroller. Historiske bevis nedenfor gjelder baseline, ikke automatisk de nye kravene.


Implemented i P4. Oppdater kontrakter, kallkart, konsumenter og tester i samme endring.
Rene porter og tydelig rolleeierskap er obligatorisk. Eventuelle senere avvik står i fasens bevisrapport.
