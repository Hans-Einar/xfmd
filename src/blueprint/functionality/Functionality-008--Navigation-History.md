---
id: FUNC-008
kind: Functionality
audience: User
role: Workflow
owner: application
status: Implemented
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

NavigationCoordinator::openTarget/followLink/goBack/goForward er offentlige innganger. HistoryStore eier in-memory liste/cursor (maks 100), LinkResolver eier lokal sti-/URI-policy. Pending request bevarer forrige anker og eventuell historikkindeks. DocumentCoordinator opened-callback er eneste commitVisit-inngang.

Relative lenker bruker det åpne dokumentets absolutte foreldremappe som base.
Prosessens arbeidsmappe brukes ikke. Absolute stier beholdes før canonicalisering.

## 4. Atferd, tilstand og feil

CLI/dialog/sidebar går via Application::open til samme navigation→document-vei. resolveUnsaved og lasting må lykkes før history.commit. Ny visit etter back trunkerer frem-gren; samme fil dupliseres ikke. Back/forward leser fil på nytt og gjenoppretter anker når frame er klart. Percent-encoding dekodes én gang; schemes/network/fragment/query/control-byte avvises. SaveAs oppdaterer aktuell poststi.

## 5. Plumbing

Tabellen beskriver implementerte kall. Navngitte hendelser er injiserte callbacks, ikke en global event bus.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `Application::open` | `NavigationCoordinator::openTarget` | `src/application/navigation/NavigationCoordinator.cpp` | Path → pending request | Reentrant request avvises | Implemented |
| 2 | `FoxRenderHost link callback` | `NavigationCoordinator::followLink` | `src/application/navigation/NavigationCoordinator.cpp` | LinkTarget → lokal navigasjon | Feil gir status/dialog | Implemented |
| 3 | `NavigationCoordinator::followLink` | `LinkResolver::resolve` | `src/application/navigation/LinkResolver.cpp` | Basepath/target → canonical local path | Ingen shell/network | Implemented |
| 4 | `NavigationCoordinator::goBack / goForward` | `HistoryStore::propose` | `src/application/navigation/HistoryStore.cpp` | Retning → indeks | NoOp ved endepunkt | Implemented |
| 5 | `NavigationCoordinator::openTarget` | `DocumentCoordinator::requestOpen` | `src/application/document/DocumentCoordinator.cpp` | Path → success/failure | Failed/cancel muterer ikke history | Implemented |
| 6 | `DocumentCoordinator opened callback` | `NavigationCoordinator::commitVisit` | `src/application/navigation/NavigationCoordinator.cpp` | Committed dokument → history | Én callback per commit | Implemented |
| 7 | `NavigationCoordinator::commitVisit` | `HistoryStore::commit` | `src/application/navigation/HistoryStore.cpp` | Path/forrige anker/target → cursor | Ny visit trunkerer frem-gren | Implemented |
| 8 | `NavigationCoordinator::commitVisit` | `ScrollCoordinator::restoreAnchor` | `src/application/scroll/ScrollCoordinator.cpp` | Historikkanker → pending restore | Venter på gyldig FrameReady | Implemented |

## 6. Gjenbruk og avhengigheter

[FUNC-001](../functionality/Functionality-001--Document-Session.md), [FUNC-009](../functionality/Functionality-009--Source-Anchor-Mapping.md)

CLI, dialog, sidebar og lenker deler dokumentbytte/commit-policy. Source-anchor-functionality deles med scrollsync. HistoryStore er ikke en service som widgets muterer direkte.

## 7. Verifikasjon

Relevante akseptanse-ID-er: AT-001, AT-005, AT-009, AT-012, AT-015, AT-018, AT-023.

`NavigationTest` tester A/B/C/back/D, avbrudd, brutt lenke, 100-grense, encoding/schemes og ingen dobbel commit. NavigationGuiTest klikker et ekte renderer-hit og kontrollerer back/forward i FOX.

Evidence: [Fase P6](../../../docs/evidence/P6.md). Samlet kravdekning og eventuelle gjenstående begrensninger kontrolleres i P7; Implemented er ikke automatisk Verified.

Ny regresjonskontroll: [Native lenker og markører](../../../docs/evidence/document-links.md).

## 8. Status, risiko og endringskonsekvenser

Implemented i P6. Oppdater kontrakter, kallkart, konsumenter og tester i samme endring.
Rene porter og tydelig rolleeierskap er obligatorisk. Eventuelle senere avvik står i fasens bevisrapport.
