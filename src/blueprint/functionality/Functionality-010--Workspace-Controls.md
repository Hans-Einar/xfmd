---
id: FUNC-010
kind: Functionality
audience: User
role: Adapter
owner: application
status: Proposed
scope: FirstRelease
requirements: UR-011, UR-001, UR-006, UR-007, SR-002, SR-008, SR-013, UR-015, UR-017, UR-018, UR-019, UR-020, SR-019
uses: FUNC-001, FUNC-005, FUNC-007, FUNC-012, FUNC-013, FUNC-014, FUNC-015, FUNC-018, FUNC-019
---

# Functionality-010: Arbeidsflate, kommandoer og sidepanel

## 1. Hensikt og avgrensning

Samle tynn UI-plumbing for vindu, splitter, view modes, sidepanel og kommandoinnganger. Dette er samlet workspace-ansvar, ikke en feature per knapp. Ingen parseregler eller filtransaksjoner i XfmdWindow.

## 2. Krav og akseptanse

Krav: UR-011, UR-001, UR-006, UR-007, SR-002, SR-008, SR-013. Definisjoner og normativ akseptanse finnes i
[kravspesifikasjonen](../../../xfmd_requirements.md). Kapittel 7 konkretiserer beviset.

## 3. Kontrakter og eierskap

XfmdWindow::buildUi oppretter vindusstruktur. CommandRouter::dispatch/update ruter handlinger og enabled-state. ViewModeController::setMode/toggleSidebar eier synlighet for hele WorkspacePanel; SidebarWidget::onOpen sender åpneforespørsel.

**Planlagt utvidelse 1.1:** CommandRouter delegerer Preferences, eksport og visningsprofil til egne tjenester. FoxWindowMode eier fullscreen/restore, XfmdWindow bygger kun widgets. Sidebar og WorkPathList får samme scrollprofil. Menyvalgene endrer ikke dirty/undo.

## 4. Atferd, tilstand og feil

Preview standard, Ctrl+1/2/3 bytter modus, F10 bytter sidebar. Dobbeltklikk på en faktisk fil åpner dokument. Dobbeltklikk rot utvider arbeidsroten eksplisitt, uten dokumentbytte. Mapper, også mapper med
.md-suffiks, forblir tre-navigasjon. SidebarWidget bruker ID_TREE_EVENT fra
FXTreeList::ID_LAST slik at treets SEL_COMMAND ikke treffer FOXs ID_HIDE.
Treklikk og dokumentbytte bevarer sidepanelets synlighet; bare eksplisitt
F10/Sidebar-handling endrer den. WorkspacePanel komponerer arbeidsrot, filter og historikk; FTR-005 beskriver arbeidsflyten. Modusbytte bevarer økt og undo; ingen filtransaksjon i vindusklassen.

FoxWheelScrollBar er delt FOX-adapter for begge scrollakser. Den bevarer
fraksjoner mellom små wheel-events og bruker FOXs eksisterende animasjon og
changed/command-varsler. Konstruktørene bytter barene før create(); widgets eier dem.

## 5. Plumbing

Implemented-rader beskriver baseline 0712c29; Planned-rader beskriver utvidelsen. Navngitte hendelser er injiserte callbacks, ikke en global event bus.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `XfmdWindow constructor` | `XfmdWindow::buildUi` | `src/application/ui/XfmdWindow.cpp` | FOX app → vindu | Parenting eier widgets | Implemented |
| 2 | `FOX command` | `CommandRouter::dispatch` | `src/application/commands/CommandRouter.cpp` | Command → Application::execute | Enabled kontrolleres | Implemented |
| 3 | `Application::execute` | `ViewModeController::setMode` | `src/application/ui/ViewModeController.cpp` | Mode → flater | Bevarer dokument | Implemented |
| 4 | `FOX SEL_DOUBLECLICKED / ID_TREE_EVENT` | `SidebarWidget::onOpen` | `src/application/ui/SidebarWidget.cpp` | Faktisk fil → Path | Mappe/enkeltklikk åpner ikke og skjuler ikke panelet | Implemented |
| 5 | `SidebarWidget open callback` | `Application::open` | `src/application/Application.cpp` | Path → requestOpen | Felles dirty-policy | Implemented |
| 6 | `SidebarWidget constructor` | `FoxWheelScrollBar::replace` | `src/application/adapters/FoxWheelScrollBar.cpp` | Standard bar → presis wheel-adapter | Parent eier ny bar; før create | Implemented |
| 7 | `FOX wheel dispatch` | `FoxWheelScrollBar::onMouseWheel` | `src/application/adapters/FoxWheelScrollBar.cpp` | Delta/rest → target og FOX-timer | Clamp, behold delpiksel-rest, standard varsler | Implemented |
| 8 | `XfmdWindow::buildUi` | `WorkspacePanel::WorkspacePanel` | `src/application/ui/WorkspacePanel.cpp` | Filter, tre og historikk → ett panel | Parent eier widgets | Implemented |
| 9 | `WorkPathList constructor` | `FoxWheelScrollBar::replace` | `src/application/adapters/FoxWheelScrollBar.cpp` | Historikkliste → samme presise scrollbarer | Parent eier adapterne; før create | Implemented |
| 10 | `Edit Preferences` | `PreferencesService::begin` | `src/application/preferences/PreferencesService.cpp` | aktiv profil → dialog | Cancel forkaster | Planned |
| 11 | `View Full Screen` | `FoxWindowMode::requestFullscreen` | `src/application/adapters/FoxWindowMode.cpp` | bool → WM-request | bekreft faktisk WM-state | Planned |
| 12 | `File Export PDF` | `ExportCoordinator::start` | `src/application/export/ExportCoordinator.cpp` | snapshot → jobb | ingen markSaved | Planned |

## 6. Gjenbruk og avhengigheter

[FUNC-001](../functionality/Functionality-001--Document-Session.md), [FUNC-005](../functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-007](../functionality/Functionality-007--Preview-Pipeline.md)

[FUNC-012](Functionality-012--Work-Path-History.md) og [FUNC-013](Functionality-013--Filtered-File-Tree.md) eier arbeidsstier og filtret tre.

UC-001/003/004 bruker denne functionality direkte; navigasjon/sync bruker kontrollene uten å eie dem. Nye kommandoer rutes til riktig tjeneste, aldri en ny stor switch med implementasjoner i vinduet.

## 7. Verifikasjon

Relevante akseptanse-ID-er: AT-025, AT-001, AT-006, AT-007, AT-012, AT-018, AT-023.

`WorkspaceTest` kjører under egen Xvfb og kontrollerer editor/preview-modus, F10-funksjonen, filfilter, undo og dirty-close. Kommando-/visuell ende-til-ende QA utvides i P7.

Evidence: [Fase P2](../../../docs/evidence/P2.md). Samlet kravdekning og eventuelle gjenstående begrensninger kontrolleres i P7; Implemented er ikke automatisk Verified.

Regresjon: [Sidepanel og rotklikk](../../../docs/evidence/sidebar-tree.md).

Regresjonsbevis: [Gesture og scrollgrenser](../../../docs/evidence/wheel-scrolling.md).

Utvidelsen krever AT-029, AT-031, AT-032, AT-033, AT-034, AT-039. Dette er planlagt dekning, ikke nye testbevis.

## 8. Status, risiko og endringskonsekvenser

**Proposed 1.1:** Historisk Implemented/evidence nedenfor gjelder baseline. Nye kontrakter er beskrevet i [designrevisjonen](../../../softwareDesign.md); gamle bevis verifiserer ikke disse.


Implemented i P2. Oppdater kontrakter, kallkart, konsumenter og tester i samme endring.
Rene porter og tydelig rolleeierskap er obligatorisk. Eventuelle senere avvik står i fasens bevisrapport.
