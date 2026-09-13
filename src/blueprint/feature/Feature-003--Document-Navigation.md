---
id: FTR-003
kind: Feature
audience: User
role: Workflow
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-001, UR-005, UR-009, SR-002, SR-005, SR-008, SR-012, SR-013, SR-019
uses: FUNC-001, FUNC-002, FUNC-005, FUNC-008, FUNC-009, FUNC-010
---

# Feature-003: Lokale dokumentreiser med historikk

## 1. Hensikt og avgrensning

Brukeren følger lokale dokumentlenker og kommer tilbake/frem til riktig fil og leseposisjon. Lenker alene er functionality; lenker med transaksjonell historikk gir den samlede featureverdien.

## 2. Krav og akseptanse

Krav: UR-001, UR-005, UR-009, SR-002, SR-005, SR-008, SR-012, SR-013. Definisjoner og normativ akseptanse finnes i
[kravspesifikasjonen](../../../xfmd_requirements.md). Kapittel 7 konkretiserer beviset.

## 3. Kontrakter og eierskap

Feature orkestreres av NavigationCoordinator. LocalFileStore, DocumentCoordinator, HistoryStore, LinkResolver og ScrollCoordinator gjenbrukes. Lenketreff utføres av renderer-porten, mens application bestemmer hva som åpnes.

**Implementert utvidelse 1.1 (P11):** Historikk beholder source-byteankre på tvers av sideprofil, zoom og fullscreen; ingen lagring av skjermpiksler som dokumentposisjon.

## 4. Atferd, tilstand og feil

Native knappetrykk/-slipp går gjennom FOX-dispatch; host er enabled. Relative
stier løses fra det åpne dokumentets mappe, ikke prosessens arbeidsmappe.

Et aktivt lenketreff gir lokal filnavigasjon. Historikk committes etter vellykket åpning og lagrer kildeanker. Back/forward venter på korrekt frame før restore. Brutt lenke og dirty-cancel bevarer både buffer og cursor. Eksterne schemes/fragmenter er eksplisitt unsupported, ikke sideeffekter. Alle dokumentinnganger bruker samme policy.

## 5. Plumbing

Implemented-rader beskriver baseline 0712c29; Planned-rader beskriver utvidelsen. Navngitte hendelser er injiserte callbacks, ikke en global event bus.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `FoxRenderHost::onPointer` | `IRenderer::hitTest` | `src/contracts/IRenderer.h` | Frame/punkt → link | Kun gyldig frame | Implemented |
| 2 | `Host linkActivated callback` | `NavigationCoordinator::followLink` | `src/application/navigation/NavigationCoordinator.cpp` | Target → request | Lokal sti-policy | Implemented |
| 3 | `NavigationCoordinator::openTarget` | `DocumentCoordinator::requestOpen` | `src/application/document/DocumentCoordinator.cpp` | Path → dokumentcommit | Dirty/lesefeil før commit | Implemented |
| 4 | `DocumentCoordinator opened callback` | `NavigationCoordinator::commitVisit` | `src/application/navigation/NavigationCoordinator.cpp` | Vellykket dokument → historikk | Aldri commit på failed/cancel | Implemented |
| 5 | `NavigationCoordinator::commitVisit` | `ScrollCoordinator::restoreAnchor` | `src/application/scroll/ScrollCoordinator.cpp` | Anker → pending restore | Frame-token må stemme | Implemented |
| 6 | `History restore` | `ScrollCoordinator::restoreAnchor` | `src/application/scroll/ScrollCoordinator.cpp` | SourceAnchor → VisualLocation | vent på riktig FrameKey | Implemented |

## 6. Gjenbruk og avhengigheter

[FUNC-001](../functionality/Functionality-001--Document-Session.md), [FUNC-002](../functionality/Functionality-002--Local-File-Storage.md), [FUNC-005](../functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-008](../functionality/Functionality-008--Navigation-History.md), [FUNC-009](../functionality/Functionality-009--Source-Anchor-Mapping.md), [FUNC-010](../functionality/Functionality-010--Workspace-Controls.md)

Historikk og scrollsync deler kildeanker; sidebar/CLI bruker samme dokumentbytte. Ingen parser, filstore eller widgetkopi inne i feature.

## 7. Verifikasjon

Relevante akseptanse-ID-er: AT-001, AT-005, AT-009, AT-012, AT-015, AT-018, AT-022, AT-023.

`NavigationTest` og `NavigationGuiTest` passerer, med dirty-cancel og tilbake/frem etter renderer-hit. Source-anchor/resize er også verifisert i P5.

Evidence: [Fase P6](../../../docs/evidence/P6.md). Samlet kravdekning og eventuelle gjenstående begrensninger kontrolleres i P7; Implemented er ikke automatisk Verified.

Ny regresjonskontroll: [Native lenker og markører](../../../docs/evidence/document-links.md).

Utvidelsen krever AT-039. Dette er planlagt dekning, ikke nye testbevis.

## 8. Status, risiko og endringskonsekvenser

**Implemented 1.1:** [P11-bevis](../../../docs/evidence/P11.md) beskriver ny kode og kontroller. Historiske bevis nedenfor gjelder baseline, ikke automatisk de nye kravene.


Implemented i P6. Oppdater kontrakter, kallkart, konsumenter og tester i samme endring.
Rene porter og tydelig rolleeierskap er obligatorisk. Eventuelle senere avvik står i fasens bevisrapport.
