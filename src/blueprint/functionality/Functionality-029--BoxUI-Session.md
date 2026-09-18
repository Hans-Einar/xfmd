---
id: FUNC-029
kind: Functionality
audience: System
role: Service
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-043, SR-026
uses: none
---

# Functionality-029: BoxUI-Session

## 1. Hensikt og avgrensning

Eier validert intent → lokal syntetisk deltaker og command-ledger. Ingen HTML, nettverkskjøring eller automatisk scenariostart.

## 2. Krav og akseptanse

UR-043, SR-026; AT-068 / AT-069. Felles kontrakt og akseptanse er beskrevet i
[BoxUI-design](../../../docs/design/boxui-integration.md).

## 3. Kontrakter og eierskap

BX-HOST/0.1-draft1. Offentlige kontrakter bruker C++-verdier uten FOX/Rust/JSON.
JSON-kodek holdes privat ved ABI. Interpreter tolker diagram-barn; application
forbereder dem via eksisterende IDiagramLayout før BoxUI-layout.

## 4. Atferd, tilstand og feil

Kilde-/bindings-/kontekstendring ugyldiggjør input straks. Ugyldig blokk gir lokal
feil med kilde, aldri delvis modell. Tidsbudsjett og avbrudd er kooperative.
Utkast er ikke aksepterte verdier; PDF bruker bare frosset akseptert snapshot.

## 5. Plumbing

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | dokumentendring | `BoxUiSession::expect` | `src/application/boxui/BoxUiSession.cpp` | DocumentToken → ugyldiggjort epoch | stanser prototype | Implemented |
| 2 | View-meny | `BoxUiSession::toggle` | `src/application/boxui/BoxUiSession.cpp` | modell → lokale deltakere | eksplisitt start/restart | Implemented |
| 3 | preview/PDF | `BoxUiSession::freeze` | `src/application/boxui/BoxUiSession.cpp` | source → kopiert snapshot | ingen utkast eller scenarioeffekt | Implemented |
| 4 | native kontroll | `BoxUiSession::dispatch` | `src/application/boxui/BoxUiSession.cpp` | intent → validert ledger-resultat | stale/type/kontekst/full ledger avvises | Implemented |
| 5 | BoxUiSession | `SyntheticActivity::execute` | `src/application/boxui/SyntheticActivity.cpp` | kommando → lokal domenetilstand | resume-konflikt bevarer aktivitet | Implemented |

## 6. Gjenbruk og avhengigheter

Brukes av FTR-011. Vanlig Markdown/Mermaid, tekstmål og PDF beholder eksisterende vei.
Ingen direkte kall til andre features sine interne implementasjoner.

## 7. Verifikasjon

BoxUiCoreTest, BoxUiSessionTest, BoxUiGuiTest og BoxUiPdfTest gir avgrenset
bevis. Kjøringer og kjente begrensninger registreres i sprint 003 fase 048.

## 8. Status, risiko og endringskonsekvenser

Implemented. Implementasjon autorisert 2026-09-18. IME/desktop-tilgjengelighet må vurderes
separat fra grunnleggende Unicode, clipboard og tastaturnavigasjon.
