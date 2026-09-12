---
id: FUNC-010
kind: Functionality
audience: User
role: Adapter
owner: application
status: Proposed
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

`XfmdWindow::buildUi`, `CommandRouter::dispatch(Command)`, `ViewModeController::setMode(ViewMode)`, `toggleSidebar()`, `SidebarWidget::onOpen(Path)`. ViewMode er Preview, Editor eller Split. CommandRouter holder bare routing/enabled-state; de konkrete koordinatorene eier handlingene.

## 4. Atferd, tilstand og feil

Preview er standard. Split betyr editor til venstre og preview til høyre, ikke over/under. Modusbytte bevarer buffer/undo; visning/resize ber pipeline om gyldig layout. Sidepanel viser mapper samt .md/.txt uten hensyn til ASCII case. Enkeltklikk velger; dobbeltklikk bruker vanlig requestOpen. F10 og menyer går samme vei. Dialog/CLI og toolbar bruker samme router/koordinatorer.

## 5. Plumbing

Alle symboler og kildefiler i tabellen er **planlagte**, ikke implementert kode.
Bibliotekskall verifiseres mot valgt dependency-versjon før implementering.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `Application::initialize` | `XfmdWindow::buildUi` | `src/application/ui/XfmdWindow.cpp` | FOX-parent + koordinatorreferanser | Ingen featurelogikk ved konstruksjon. | Planned |
| 2 | `FOX menu/key/toolbar event` | `CommandRouter::dispatch` | `src/application/commands/CommandRouter.cpp` | Command → eierens offentlige inngang | Disabled kommando gir NoOp. | Planned |
| 3 | `CommandRouter view command` | `ViewModeController::setMode / toggleSidebar` | `src/application/ui/ViewModeController.cpp` | Mode → synlighet/fokus | Behold tekst, dirty og undo. | Planned |
| 4 | `SidebarWidget::onOpen / CommandRouter::open` | `DocumentCoordinator::requestOpen` | `src/application/document/DocumentCoordinator.cpp` | Path → OpenResult | Single click åpner ikke; feil beholder økt. | Planned |
| 5 | `ViewModeController ved visning/resize` | `PreviewCoordinator::relayout` | `src/application/preview/PreviewCoordinator.cpp` | Ny bredde/generasjon | Ingen stale frame-interaksjon mens layout venter. | Planned |

## 6. Gjenbruk og avhengigheter

[FUNC-001](../functionality/Functionality-001--Document-Session.md), [FUNC-007](../functionality/Functionality-007--Preview-Pipeline.md)

UC-001/003/004 bruker denne functionality direkte; navigasjon/sync bruker kontrollene uten å eie dem. Nye kommandoer rutes til riktig tjeneste, aldri en ny stor switch med implementasjoner i vinduet.

## 7. Verifikasjon

AT-001, AT-006, AT-007, AT-012, AT-018, AT-023: filtreringsfixture, F10, alle view modes, splitter/fokus, uleselig mappe, doble events og CLI med mellomrom. Planlagt `tests/gui/WorkspaceTest.cpp`.

Bevis: ingen applikasjonstest kjørt; testfiler ovenfor er planlagte. Ved implementering
oppgis kommando, fixture, miljø, commit og faktisk utfall. Strukturkontroll alene
oppfyller ikke atferdskravene.

## 8. Status, risiko og endringskonsekvenser

Proposed, revisjon 0.1, 2026-09-12. Dette objektet grupperer UI-roller med samme endringsårsak. Splitt senere dersom kompleksitet eller forskjellige eiere begrunner det; ikke samle dokument-/navigasjonslogikk her.

Ved endret offentlig kontrakt: oppdater konsumentene i registeret, dette kallkartet,
berørte krav og kontrakttester i samme endring. Før status Ready skal relevante
P0-spørsmål være avgjort; før Verified skal kapittel 7 inneholde testbevis.
