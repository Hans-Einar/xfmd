---
id: FUNC-006
kind: Functionality
audience: System
role: Adapter
owner: application
status: Proposed
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

`FoxScheduler::restart(TimerKey, Duration, Callback)`, `cancel(TimerKey)`, `cancelAll(OwnerId)`, `onTimeout`. En nøkkel har høyst én aktiv timer. Callback kjøres på GUI-tråden og bærer dokument/revisjon som eieren kan kontrollere. Tester bruker en deterministisk klokkeadapter.

## 4. Atferd, tilstand og feil

Restart erstatter gammel timer. Ny edit setter fristen til siste edit +300 ms. Document switch eller teardown kansellerer. Timerlevering kan være senere dersom event loop er opptatt; debounce garanterer ikke maksimal renderlatens. Callback kontrollerer eierens levetid selv om cancel skjer nær dispatch.

## 5. Plumbing

Alle symboler og kildefiler i tabellen er **planlagte**, ikke implementert kode.
Bibliotekskall verifiseres mot valgt dependency-versjon før implementering.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `PreviewCoordinator::schedule` | `FoxScheduler::restart` | `src/application/adapters/FoxScheduler.cpp` | Key + 300 ms + token | Erstatt gammel timer uten duplikat. | Planned |
| 2 | `FoxScheduler::restart` | `FXApp::removeTimeout / addTimeout` | `src/application/adapters/FoxScheduler.cpp` | FOX target/selector → timer | API-verifisering mot valgt FOX-versjon i P0. | Planned |
| 3 | `FOX timeout event` | `FoxScheduler::onTimeout` | `src/application/adapters/FoxScheduler.cpp` | Token → Callback | Utgått eier/token ignoreres. | Planned |
| 4 | `PreviewCoordinator ved dokumentbytte/destruksjon` | `FoxScheduler::cancelAll` | `src/application/adapters/FoxScheduler.cpp` | OwnerId → ingen pending timer | Koble fra før mottaker frigjøres. | Planned |

## 6. Gjenbruk og avhengigheter

Ingen andre functionality-kontrakter konsumeres; delte datatyper følger arkitekturen.

Preview er første konsument. Andre tidsstyrte funksjoner kan bruke adapteren når de har konkret krav; ikke opprett separate timere direkte inne i featurekode.

## 7. Verifikasjon

AT-004, AT-018, AT-020, AT-023: edits ved 0/100/250 ms gir én refresh tidligst 550 ms; cancel, ny nøkkel og teardown. Planlagt `tests/application/FoxSchedulerTest.cpp` med fake clock samt ekte FOX smoke.

Bevis: ingen applikasjonstest kjørt; testfiler ovenfor er planlagte. Ved implementering
oppgis kommando, fixture, miljø, commit og faktisk utfall. Strukturkontroll alene
oppfyller ikke atferdskravene.

## 8. Status, risiko og endringskonsekvenser

Proposed, revisjon 0.1, 2026-09-12. Ingen klokketøy er implementert. Eierskap/registrering i FOX må bevises før Ready for integrasjonsdelen.

Ved endret offentlig kontrakt: oppdater konsumentene i registeret, dette kallkartet,
berørte krav og kontrakttester i samme endring. Før status Ready skal relevante
P0-spørsmål være avgjort; før Verified skal kapittel 7 inneholde testbevis.
