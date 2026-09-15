---
id: FUNC-011
kind: Functionality
audience: User
role: Service
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-032, UR-033, UR-011, UR-003, UR-004, UR-009, SR-002, SR-006, SR-008, SR-013, UR-016, SR-019
uses: FUNC-001, FUNC-005, FUNC-007, FUNC-015
---

# Functionality-011: Tekstredigering, undo og søk

## 1. Hensikt og avgrensning

Tilby vanlig tekstredigering med én konsistent vei til dokumentrevisjoner. FOX-editoren er projeksjon, ikke en konkurrerende source of truth. Avgrenset enkel editor, ikke erstatning for xfw som avansert tekstverktøy.

## 2. Krav og akseptanse

Krav: UR-011, UR-003, UR-004, UR-009, SR-002, SR-006, SR-008, SR-013. Definisjoner og normativ akseptanse finnes i
[kravspesifikasjonen](../../../xfmd_requirements.md). Kapittel 7 konkretiserer beviset.

## 3. Kontrakter og eierskap

EditController::applyEdit/applyProjectedText/undo/redo/find eier undo-operasjoner. TextProjection konverterer LF-visning til rå kildeoffsets og opprinnelig newline-policy. EditorWidget eier bare FXText-projeksjonen.

**Implementert utvidelse 1.1 (P11):** EditorWidget beholder tekst, undo og kildeprojeksjon. FoxWheelScrollBar konsumerer felles FUNC-015-profil, uten egen akselerasjonsformel. Source-anchor restore og søketreff flytter direkte med korrekt origin.

## 4. Atferd, tilstand og feil

Én undo-stack med 32 MiB historikkbudsjett. Programmatisk projeksjon gir ikke ny edit. BOM og urørte blandede linjesluttsekvenser bevares; nye linjer bruker filens første linjesluttformat. UTF-8-diff utvides til tegnsgrense. Søk endrer ikke dokument. Åpning resetter undo, view mode gjør det ikke.

FoxWheelScrollBar er delt FOX-adapter for begge scrollakser. Den bevarer
fraksjoner mellom små wheel-events og bruker FOXs eksisterende animasjon og
changed/command-varsler. Konstruktørene bytter barene før create(); widgets eier dem.

TextProjection reserverer offset-/tekstkapasitet fra kjent raw-lengde før
konvertering. Dette unngår kapasitetdobling og store midlertidige topper ved
ny projeksjon; byte-/BOM-/CRLF-kontrakten er uendret. Se P14-målingen.

## 5. Plumbing

Implemented-rader beskriver gjeldende plumbing; historiske fasebevis identifiserer tidligere baseline. Navngitte hendelser er injiserte callbacks, ikke en global event bus.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `FXText edit event` | `EditorWidget::onChanged` | `src/application/ui/EditorWidget.cpp` | FXString → edited callback | Projection ignoreres | Implemented |
| 2 | `Application edited callback` | `EditController::applyProjectedText` | `src/application/document/EditController.cpp` | LF text → Edit | Feil gjenoppretter projeksjon | Implemented |
| 3 | `EditController::applyProjectedText` | `TextProjection::difference` | `src/application/document/TextProjection.cpp` | Ny tekst → raw edit | UTF-8/newline mapping | Implemented |
| 4 | `EditController::apply` | `DocumentSession::applyEdit` | `src/application/document/DocumentSession.cpp` | Edit → ny Revision | Input valideres | Implemented |
| 5 | `EditController changed callback` | `Application::updateUi` | `src/application/Application.cpp` | Snapshot → editor/title | contentChanged varsler preview | Implemented |
| 6 | `Application::execute` | `EditController::find` | `src/application/document/EditController.cpp` | Query → projected offset | Ingen dirty-endring | Implemented |
| 7 | `EditorWidget constructor` | `FoxWheelScrollBar::replace` | `src/application/adapters/FoxWheelScrollBar.cpp` | Standard bar → presis wheel-adapter | Parent eier ny bar; før create | Implemented |
| 8 | `FOX wheel dispatch` | `FoxWheelScrollBar::onMouseWheel` | `src/application/adapters/FoxWheelScrollBar.cpp` | Delta/rest → target og FOX-timer | Clamp, behold delpiksel-rest, standard varsler | Implemented |
| 9 | `Editor wheel` | `ScrollDynamics::advance` | `src/application/scroll/ScrollDynamics.cpp` | input → mål | ingen tekstendring | Implemented |
| 30 | `Application::applyAppearance / changeReadingColors` | `EditorWidget::setReadingColors` | `src/application/ui/EditorPresentation.cpp` | Delt palett → editor/cursor | Ingen tekstmutasjon | Implemented |
| 31 | `EditorWidget::layout` | `EditorWidget::applyViewProfile` | `src/application/ui/EditorPresentation.cpp` | Papir/zoom/bredde → font og wrap | Bevarer markering og kildeanker | Implemented |

## 6. Gjenbruk og avhengigheter

[FUNC-001](../functionality/Functionality-001--Document-Session.md), [FUNC-005](../functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-007](../functionality/Functionality-007--Preview-Pipeline.md)

Live preview konsumerer edits; dokumentøkt/lagring brukes uendret. Nye editorhandlinger skal beskrives som Edit-operasjoner, ikke mutere FXText og Session separat.

## 7. Verifikasjon

Relevante akseptanse-ID-er: AT-025, AT-003, AT-004, AT-009, AT-012, AT-016, AT-018, AT-023.

`DocumentTest` og `WorkspaceTest` bekrefter Unicode-edit, mixed EOL/BOM, dirty-baseline, ekte editor-events, undo/redo og søk.

Evidence: [Fase P2](../../../docs/evidence/P2.md). Samlet kravdekning og eventuelle gjenstående begrensninger kontrolleres i P7; Implemented er ikke automatisk Verified.

Regresjonsbevis: [Gesture og scrollgrenser](../../../docs/evidence/wheel-scrolling.md).

Utvidelsen krever AT-030, AT-039. Dette er planlagt dekning, ikke nye testbevis.

## 8. Status, risiko og endringskonsekvenser

**Implemented 1.1:** [P11-bevis](../../../docs/evidence/P11.md) beskriver ny kode og kontroller. Historiske bevis nedenfor gjelder baseline, ikke automatisk de nye kravene.


Implemented i P2. Oppdater kontrakter, kallkart, konsumenter og tester i samme endring.
Rene porter og tydelig rolleeierskap er obligatorisk. Eventuelle senere avvik står i fasens bevisrapport.

P22: UR-032/033, AT-052, AT-053. EditorPresentation.cpp eier visning separat fra edit-operasjoner.

P22: [testbevis og visuell kontroll](../../../docs/evidence/P22.md).
