---
id: FUNC-030
kind: Functionality
audience: System
role: Adapter
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-043, SR-026
uses: none
---

# Functionality-030: BoxUI-Host

## 1. Hensikt og avgrensning

Eier publisert frame → native input og semantisk kontrollkart. Ingen HTML, nettverkskjøring eller automatisk scenariostart.

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
Preview-host videresender tastetrykk gjennom FOX sin focus chain før egne
snarveier. Tegn, Backspace, Home/End, Ctrl+A/C og Escape tilhører fokusert felt.

## 5. Plumbing

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | host present | `FoxBoxUiOverlay::reconcile` | `src/application/adapters/FoxBoxUiOverlay.cpp` | kontrollkart → native felt/knapper | inkompatibelt utkast forkastes med beskjed | Implemented |
| 2 | zoom/scroll/theme | `FoxBoxUiOverlay::position` | `src/application/adapters/FoxBoxUiOverlay.cpp` | publisert geometri → klippet viewport | ingen ny layoutberegning | Implemented |
| 3 | Enter/knapp | `FoxBoxUiOverlay::submit` | `src/application/adapters/FoxBoxUiOverlay.cpp` | draft eller none → BoxUiIntent | bare aktuell frame kan sende | Implemented |
| 4 | Escape | `FoxBoxUiOverlay::restore` | `src/application/adapters/FoxBoxUiOverlay.cpp` | akseptert verdi → felt | forkaster utkast | Implemented |

| 5 | X11 → fokusert preview | `FoxRenderHost::onKeyPress` | `src/application/adapters/FoxPreviewInput.cpp` | FOX focus chain → native tekstredigering | preview-snarveier brukes bare når barnet ikke håndterer tasten | Implemented |

## 6. Gjenbruk og avhengigheter

Brukes av FTR-011. Vanlig Markdown/Mermaid, tekstmål og PDF beholder eksisterende vei.
Ingen direkte kall til andre features sine interne implementasjoner.

## 7. Verifikasjon

BoxUiCoreTest, BoxUiSessionTest, BoxUiGuiTest og BoxUiPdfTest gir avgrenset
bevis. Kjøringer og kjente begrensninger registreres i sprint 003 fase 048.

## 8. Status, risiko og endringskonsekvenser

Implemented. Implementasjon autorisert 2026-09-18. IME/desktop-tilgjengelighet må vurderes
separat fra grunnleggende Unicode, clipboard og tastaturnavigasjon.
