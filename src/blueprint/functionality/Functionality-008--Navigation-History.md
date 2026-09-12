---
id: FUNC-008
kind: Functionality
audience: User
role: Workflow
owner: application
status: Proposed
scope: FirstRelease
requirements: UR-001, UR-005, UR-009, SR-002, SR-005, SR-008, SR-013
uses: FUNC-001, FUNC-009
---

# Functionality-008: Dokumentnavigasjon og historikk

## 1. Hensikt og avgrensning

Gi dokumentbytte med lokale lenker og back/forward én historikkpolicy. LinkResolver og HistoryStore er små interne roller; offentlige innganger eies av NavigationCoordinator. Ikke legg fillasting i renderer.

## 2. Krav og akseptanse

Krav: UR-001, UR-005, UR-009, SR-002, SR-005, SR-008, SR-013. Definisjoner og normativ akseptanse finnes i
[kravspesifikasjonen](../../../xfmd_requirements.md). Kapittel 7 konkretiserer beviset.

## 3. Kontrakter og eierskap

`NavigationCoordinator::followLink(LinkTarget)`, `goBack()`, `goForward()`, `openTarget(NavigationRequest)`, `commitVisit(OpenResult)`; `LinkResolver::resolve` returnerer lokal sti eller UnsupportedTarget; `HistoryStore::propose`, `commit`, `recordCurrentAnchor`. Post har normalisert målsti og kildeanker, ikke kopi av filinnhold.

## 4. Atferd, tilstand og feil

Før bytte registreres gjeldende anker i en pending transaksjon. Resolve/dirty/lasting må lykkes før cursor eller grenen endres. DocumentOpened-observeren er eneste commit-inngang også for CLI/dialog/sidebar. Historikkforespørsel har token som skiller Back/Forward fra NewVisit og hindrer dobbel commit. Tilbake/frem gjenoppretter anker etter FrameReady med riktig dokument/revisjon; endret fil clampler anker. Maksimalt 100 poster er foreslått.

## 5. Plumbing

Alle symboler og kildefiler i tabellen er **planlagte**, ikke implementert kode.
Bibliotekskall verifiseres mot valgt dependency-versjon før implementering.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `FoxRenderHost LinkActivated` | `NavigationCoordinator::followLink` | `src/application/navigation/NavigationCoordinator.cpp` | LinkTarget + aktiv frame-token | Stale frame ignoreres. | Planned |
| 2 | `NavigationCoordinator::followLink` | `LinkResolver::resolve` | `src/application/navigation/LinkResolver.cpp` | Basepath + mål → lokal Path | Scheme/fragment/ugyldig escape avvises uten åpning. | Planned |
| 3 | `CommandRouter::back / forward` | `HistoryStore::propose` | `src/application/navigation/HistoryStore.cpp` | Retning → pending mål/token | Ved endepunkt returneres NoOp. | Planned |
| 4 | `NavigationCoordinator::openTarget` | `ScrollCoordinator::captureAnchor` | `src/application/scroll/ScrollCoordinator.cpp` | Aktiv viewport → pending anker | Endrer ikke historikk før dokumentcommit. | Planned |
| 5 | `NavigationCoordinator::openTarget` | `DocumentCoordinator::requestOpen` | `src/application/document/DocumentCoordinator.cpp` | NavigationRequest → OpenResult | Failed/Cancelled beholder cursor og gren. | Planned |
| 6 | `DocumentOpened event` | `NavigationCoordinator::commitVisit` | `src/application/navigation/NavigationCoordinator.cpp` | Vellykket OpenResult + token | Commit én gang; NewVisit trunkerer frem-gren. | Planned |
| 7 | `FrameReady event` | `ScrollCoordinator::restoreAnchor` | `src/application/scroll/ScrollCoordinator.cpp` | Pending historikkanker + gyldig frame | Clamp ved endret fil; feil frame avvises. | Planned |

## 6. Gjenbruk og avhengigheter

[FUNC-001](../functionality/Functionality-001--Document-Session.md), [FUNC-009](../functionality/Functionality-009--Source-Anchor-Mapping.md)

CLI, dialog, sidebar og lenker deler dokumentbytte/commit-policy. Source-anchor-functionality deles med scrollsync. HistoryStore er ikke en service som widgets muterer direkte.

## 7. Verifikasjon

AT-001, AT-005, AT-009, AT-012, AT-015, AT-018, AT-023: A/B/C-back-D, feilet last, dirty-cancel, ny initial fil, grensene, repeated same target, endret/slettet besøkt fil og én commit. Planlagt `tests/application/NavigationCoordinatorTest.cpp`.

Bevis: ingen applikasjonstest kjørt; testfiler ovenfor er planlagte. Ved implementering
oppgis kommando, fixture, miljø, commit og faktisk utfall. Strukturkontroll alene
oppfyller ikke atferdskravene.

## 8. Status, risiko og endringskonsekvenser

Proposed, revisjon 0.1, 2026-09-12. Fragmenter/ekstern nettleseråpning er utsatt. URI-percent decoding og normalisering må testes uten shell; vellykket filåpning kan fortsatt gi tydelig previewfeil.

Ved endret offentlig kontrakt: oppdater konsumentene i registeret, dette kallkartet,
berørte krav og kontrakttester i samme endring. Før status Ready skal relevante
P0-spørsmål være avgjort; før Verified skal kapittel 7 inneholde testbevis.
