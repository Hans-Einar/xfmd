---
id: FUNC-006
kind: Functionality
audience: System
role: Adapter
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-004, SR-008, SR-010, SR-013
uses: none
---

# Functionality-006: Debounce og hendelseslevetid

## 1. Hensikt og avgrensning

Gi tidsstyrt arbeid en entydig FOX-timer og kansellering. Avgrenset adapter, ikke en generell task engine eller skjult trådpool.

## 2. Krav og akseptanse

Krav: UR-004, SR-008, SR-010, SR-013. Definisjoner og normativ akseptanse finnes i
[kravspesifikasjonen](../../../xfmd_requirements.md). Kapittel 7 konkretiserer beviset.

## 3. Kontrakter og eierskap

IScheduler er en lokal application-port. FoxScheduler implementerer restart(key, milliseconds, callback), cancel og cancelAll; FakeScheduler brukes i tester. Én schedulerinstans eies av Application, preview bruker nøkkel 1 (debounce) og 2 (worker polling).

## 4. Atferd, tilstand og feil

Restart erstatter timeren. Callback fjernes før dispatch, slik at den kan rearmere trygt. Preview kansellerer begge nøkler ved destruksjon; scheduler kansellerer alle rester. Ingen callback eier FOX-widgeten. Timere kjører på GUI-tråden.

## 5. Plumbing

Tabellen beskriver implementerte kall. Navngitte hendelser er injiserte callbacks, ikke en global event bus.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `PreviewCoordinator::schedule` | `FoxScheduler::restart` | `src/application/adapters/FoxScheduler.cpp` | Key 1 + 300 ms → timer | Tidligere deadline erstattes | Implemented |
| 2 | `FXApp timeout` | `FoxScheduler::onTimeout` | `src/application/adapters/FoxScheduler.cpp` | Selector → callback | Fjernes før callback | Implemented |
| 3 | `PreviewCoordinator::~PreviewCoordinator` | `FoxScheduler::cancel` | `src/application/adapters/FoxScheduler.cpp` | Key 1/2 → ingen timer | Ingen dangling receiver | Implemented |
| 4 | `FoxScheduler::~FoxScheduler` | `FoxScheduler::cancelAll` | `src/application/adapters/FoxScheduler.cpp` | Alle keys → avregistrering | App lever lenger enn scheduler | Implemented |

## 6. Gjenbruk og avhengigheter

Ingen andre functionality-kontrakter konsumeres; delte datatyper følger arkitekturen.

Preview er første konsument. Andre tidsstyrte funksjoner kan bruke adapteren når de har konkret krav; ikke opprett separate timere direkte inne i featurekode.

Sprint 004 consumer (2026-09-24 reconstruction): FUNC-031 owns a FoxScheduler for endpoint/child/release polling and a separate navigator scheduler; timers stop with their owners.
See [FUNC-031](Functionality-031--Generated-Document-Navigation.md) for the complete call path.

## 7. Verifikasjon

Relevante akseptanse-ID-er: AT-004, AT-018, AT-020, AT-023.

`PreviewTest` verifiserer 0/100/250→550 ms, cancellation og teardown med FakeScheduler. PresentationTest verifiserer faktisk FOX-timer/live preview.

Evidence: [Fase P4](../../../docs/evidence/P4.md). Samlet kravdekning og eventuelle gjenstående begrensninger kontrolleres i P7; Implemented er ikke automatisk Verified.

## 8. Status, risiko og endringskonsekvenser

Implemented i P4. Oppdater kontrakter, kallkart, konsumenter og tester i samme endring.
Rene porter og tydelig rolleeierskap er obligatorisk. Eventuelle senere avvik står i fasens bevisrapport.
