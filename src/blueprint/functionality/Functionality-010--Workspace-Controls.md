---
id: FUNC-010
kind: Functionality
audience: User
role: Adapter
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-001, UR-006, UR-007, SR-002, SR-008, SR-013
uses: FUNC-001, FUNC-007
---

# Functionality-010: Arbeidsflate, kommandoer og sidepanel

## 1. Hensikt og avgrensning

Samle tynn UI-plumbing for vindu, splitter, view modes, sidepanel og kommandoinnganger. Dette er samlet workspace-ansvar, ikke en feature per knapp. Ingen parseregler eller filtransaksjoner i XfmdWindow.

## 2. Krav og akseptanse

Krav: UR-001, UR-006, UR-007, SR-002, SR-008, SR-013. Definisjoner og normativ akseptanse finnes i
[kravspesifikasjonen](../../../xfmd_requirements.md). Kapittel 7 konkretiserer beviset.

## 3. Kontrakter og eierskap

XfmdWindow::buildUi oppretter vindusstruktur. CommandRouter::dispatch/update ruter handlinger og enabled-state. ViewModeController::setMode/toggleSidebar eier synlighet; SidebarWidget::onOpen sender åpneforespørsel.

## 4. Atferd, tilstand og feil

Preview standard, Ctrl+1/2/3 bytter modus, F10 bytter sidebar. Bare dobbeltklikk åpner. Case-insensitive .md/.txt-filter. Modusbytte bevarer økt og undo; ingen filtransaksjon i vindusklassen.

## 5. Plumbing

Tabellen beskriver implementerte kall. Navngitte hendelser er injiserte callbacks, ikke en global event bus.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `XfmdWindow constructor` | `XfmdWindow::buildUi` | `src/application/ui/XfmdWindow.cpp` | FOX app → vindu | Parenting eier widgets | Implemented |
| 2 | `FOX command` | `CommandRouter::dispatch` | `src/application/commands/CommandRouter.cpp` | Command → Application::execute | Enabled kontrolleres | Implemented |
| 3 | `Application::execute` | `ViewModeController::setMode` | `src/application/ui/ViewModeController.cpp` | Mode → flater | Bevarer dokument | Implemented |
| 4 | `FOX double click` | `SidebarWidget::onOpen` | `src/application/ui/SidebarWidget.cpp` | Tree item → Path | Enkeltklikk åpner ikke | Implemented |
| 5 | `SidebarWidget open callback` | `Application::open` | `src/application/Application.cpp` | Path → requestOpen | Felles dirty-policy | Implemented |

## 6. Gjenbruk og avhengigheter

[FUNC-001](../functionality/Functionality-001--Document-Session.md), [FUNC-007](../functionality/Functionality-007--Preview-Pipeline.md)

UC-001/003/004 bruker denne functionality direkte; navigasjon/sync bruker kontrollene uten å eie dem. Nye kommandoer rutes til riktig tjeneste, aldri en ny stor switch med implementasjoner i vinduet.

## 7. Verifikasjon

Relevante akseptanse-ID-er: AT-001, AT-006, AT-007, AT-012, AT-018, AT-023.

`WorkspaceTest` kjører under egen Xvfb og kontrollerer editor/preview-modus, F10-funksjonen, filfilter, undo og dirty-close. Kommando-/visuell ende-til-ende QA utvides i P7.

Evidence: [Fase P2](../../../docs/evidence/P2.md). Samlet kravdekning og eventuelle gjenstående begrensninger kontrolleres i P7; Implemented er ikke automatisk Verified.

## 8. Status, risiko og endringskonsekvenser

Implemented i P2. Oppdater kontrakter, kallkart, konsumenter og tester i samme endring.
Rene porter og tydelig rolleeierskap er obligatorisk. Eventuelle senere avvik står i fasens bevisrapport.
