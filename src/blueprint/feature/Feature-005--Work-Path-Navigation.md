---
id: FTR-005
kind: Feature
audience: User
role: Workflow
owner: application
status: Ready
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
| 1 | `CLI / kontekstmeny / historikk` | `WorkspacePanel::setWorkPath` | `src/application/ui/WorkspacePanel.cpp` | Rot/filter → nytt tre eller historikk | Validering og kansellering før bytte | Planned |
| 2 | `WorkspacePanel` | `WorkPathHistory::activate` | `src/application/workspace/WorkPathHistory.cpp` | Rot/filter → nytt tre eller historikk | Validering og kansellering før bytte | Planned |
| 3 | `Root double click` | `WorkPathHistory::broaderRoot` | `src/application/workspace/WorkPathHistory.cpp` | Rot/filter → nytt tre eller historikk | Validering og kansellering før bytte | Planned |

## 6. Gjenbruk og avhengigheter

[FUNC-010](../functionality/Functionality-010--Workspace-Controls.md)

[FUNC-012](../functionality/Functionality-012--Work-Path-History.md)

[FUNC-013](../functionality/Functionality-013--Filtered-File-Tree.md)

FileNameFilter og DirectoryScanner brukes av samme tre ved oppstart, filter og rotbytte.
Ingen avhengighet til interpreter/renderer. Dokumentåpning bruker Application::open.

## 7. Verifikasjon

Planlagt: WorkPathTest for matching, historikk og scanner; WorkPathGuiTest for
native input, oppstart, filter og arbeidsrot. Eksisterende SidebarGuiTest og
WheelGuiTest skal fortsatt bestå. Ingen Verified-status før utførte tester.

## 8. Status, risiko og endringskonsekvenser

Ready i P8. Store eller langsomme filsystemer kan bruke tid; GUI viser fremdrift
og kansellerer gammelt arbeid. Dette er treavgrensning, ikke en OS-sandbox.
