---
id: FTR-005
kind: Feature
audience: User
role: Workflow
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-012, UR-013, UR-014
uses: FUNC-010, FUNC-012, FUNC-013
---

# Feature-005: Arbeidsområder med historikk

## 1. Hensikt og avgrensning

Arbeidsroten gir et fokusert filområde med gjenbrukbar filtrering og historikk.
Dokumentnavigasjon, parsing og lagring beholder eksisterende eiere.

## 2. Krav og akseptanse

UR-012, UR-013, UR-014; se [kravspesifikasjonen](../../../xfmd_requirements.md).
AT-026, AT-027, AT-028 dekker oppstart, rotvalg, persistens og kjedet filtrering.

## 3. Kontrakter og eierskap

WorkPathHistory eier kanonisk rot og unik MRU-liste med maksimalt 32 stier.
WorkspacePanel eier FOX-filterkontroller og historikkliste, og persisterer via FOX-registry.
SidebarWidget eier FXTreeList-noder og en DirectoryScanner. FileNameFilter er ren
navnematching. Scanner-worker eier kun filsystemdata; GUI poller nye treff.

## 4. Atferd, tilstand og feil

Standardrot er home, mappeargument setter rot, filargument bruker foreldre-mappen.
Roten vises utvidet. Dobbeltklikk rot gir home og deretter /; høyreklikk mappe
og historikkvalg går gjennom samme validering. Feil beholder gammel rot.
Filter er (valgte endelser OR) AND navnemønster. Ingen knapper betyr alle endelser.
Med filter vises kun treffenes forfedre, uten filter lastes barn ved utvidelse.
Søk kanselleres ved rot-/filterbytte. Uleselige mapper rapporteres; symlinkmapper
følges ikke og filsymlinker utenfor rot utelates. Dokumenttilstand endres ikke.

## 5. Plumbing

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `main CLI path argument` | `Application::startPath` | `src/application/Application.cpp` | Directory → work path; file → open and parent work path | Invalid path returns failure; ordinary open keeps root | Implemented |
| 2 | `SidebarWidget root/context callback or history click` | `WorkspacePanel::requestWorkPath` | `src/application/ui/WorkspacePanel.cpp` | Copied absolute path → deferred activation | No tree-node pointer survives native event dispatch | Implemented |
| 3 | `WorkspacePanel::onActivate / Application::startPath` | `WorkspacePanel::setWorkPath` | `src/application/ui/WorkspacePanel.cpp` | Validated root → tree and MRU refresh | Invalid target preserves root/document, reports error | Implemented |
| 4 | `WorkspacePanel::setWorkPath` | `WorkPathHistory::activate` | `src/application/workspace/WorkPathHistory.cpp` | Path → canonical root and history | Readability before state commit | Implemented |
| 5 | `WorkspacePanel::setWorkPath` | `SidebarWidget::setRoot` | `src/application/ui/SidebarWidget.cpp` | Root and display label → expanded root plus worker | Previous worker and entries discarded | Implemented |
| 6 | `WorkspacePanel::setWorkPath` | `WorkspacePanel::remember` | `src/application/ui/WorkspacePanel.cpp` | MRU → list and FOX registry | Display uses ~/…; persistence uses absolute paths | Implemented |

## 6. Gjenbruk og avhengigheter

[FUNC-010](../functionality/Functionality-010--Workspace-Controls.md)

[FUNC-012](../functionality/Functionality-012--Work-Path-History.md)

[FUNC-013](../functionality/Functionality-013--Filtered-File-Tree.md)

FileNameFilter og DirectoryScanner brukes av samme tre ved oppstart, filter og rotbytte.
Ingen avhengighet til interpreter/renderer. Dokumentåpning bruker Application::open.

## 7. Verifikasjon

`WorkPathTest` for matching, historikk og scanner; `WorkPathGuiTest` for
native input, oppstart, filter og arbeidsrot. Eksisterende SidebarGuiTest og
WheelGuiTest består. Bevis: [P8-verifikasjon](../../../docs/evidence/P8.md).

## 8. Status, risiko og endringskonsekvenser

Implemented i P8. Store eller langsomme filsystemer kan bruke tid; GUI viser fremdrift
og kansellerer gammelt arbeid. Dette er treavgrensning, ikke en OS-sandbox.
