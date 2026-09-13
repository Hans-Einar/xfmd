---
id: FUNC-013
kind: Functionality
audience: User
role: Service
owner: application
status: Ready
scope: FirstRelease
requirements: UR-014
uses: none
---

# Functionality-013: Filnavnfilter og treinnhold

## 1. Hensikt og avgrensning

Arbeidsroten gir et fokusert filområde med gjenbrukbar filtrering og historikk.
Dokumentnavigasjon, parsing og lagring beholder eksisterende eiere.

## 2. Krav og akseptanse

UR-014; se [kravspesifikasjonen](../../../xfmd_requirements.md).
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
| 1 | `WorkspacePanel filter event` | `SidebarWidget::setFilter` | `src/application/ui/SidebarWidget.cpp` | Rot/filter → nytt tre eller historikk | Validering og kansellering før bytte | Planned |
| 2 | `SidebarWidget` | `DirectoryScanner::start` | `src/application/workspace/DirectoryScanner.cpp` | Rot/filter → nytt tre eller historikk | Validering og kansellering før bytte | Planned |
| 3 | `DirectoryScanner worker` | `FileNameFilter::matches` | `src/application/workspace/FileNameFilter.cpp` | Rot/filter → nytt tre eller historikk | Validering og kansellering før bytte | Planned |
| 4 | `FOX poll timer` | `SidebarWidget::onPoll` | `src/application/ui/SidebarWidget.cpp` | Rot/filter → nytt tre eller historikk | Validering og kansellering før bytte | Planned |

## 6. Gjenbruk og avhengigheter

FileNameFilter og DirectoryScanner brukes av samme tre ved oppstart, filter og rotbytte.
Ingen avhengighet til interpreter/renderer. Dokumentåpning bruker Application::open.

## 7. Verifikasjon

Planlagt: WorkPathTest for matching, historikk og scanner; WorkPathGuiTest for
native input, oppstart, filter og arbeidsrot. Eksisterende SidebarGuiTest og
WheelGuiTest skal fortsatt bestå. Ingen Verified-status før utførte tester.

## 8. Status, risiko og endringskonsekvenser

Ready i P8. Store eller langsomme filsystemer kan bruke tid; GUI viser fremdrift
og kansellerer gammelt arbeid. Dette er treavgrensning, ikke en OS-sandbox.
