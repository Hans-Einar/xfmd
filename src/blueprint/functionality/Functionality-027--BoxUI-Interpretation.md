---
id: FUNC-027
kind: Functionality
audience: System
role: Adapter
owner: interpreter
status: Ready
scope: FirstRelease
requirements: UR-043, SR-026
uses: none
---

# Functionality-027: BoxUI-Interpretation

## 1. Hensikt og avgrensning

Eier fence → typed BoxUiModel og typed diagram-barn. Ingen HTML, nettverkskjøring eller automatisk scenariostart.

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
| 1 | BoxUI-kjeden | `BoxUiBlockBuilder::build` | `src/interpreter/boxui/BoxUiBlockBuilder.cpp` | fence → typed BoxUiModel og typed diagram-barn | avviser ugyldige/gamle data | Planned |

## 6. Gjenbruk og avhengigheter

Brukes av FTR-011. Vanlig Markdown/Mermaid, tekstmål og PDF beholder eksisterende vei.
Ingen direkte kall til andre features sine interne implementasjoner.

## 7. Verifikasjon

Fase 045–048 registrerer parser-, layout-, livstids-, GUI- og PDF-bevis.
Ingen runtime-verifikasjon hevdes før testene er kjørt.

## 8. Status, risiko og endringskonsekvenser

Ready. Implementasjon autorisert 2026-09-18. IME/desktop-tilgjengelighet må vurderes
separat fra grunnleggende Unicode, clipboard og tastaturnavigasjon.
