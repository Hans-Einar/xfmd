---
id: FUNC-013
kind: Functionality
audience: User
role: Service
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-014
uses: FUNC-012
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
| 1 | `WorkspacePanel::onApplyFilter` | `SidebarWidget::setFilter` | `src/application/ui/SidebarWidget.cpp` | Toggle states and pattern → rebuild current root | Old scan stopped; document unaffected | Implemented |
| 2 | `SidebarWidget::setRoot` | `DirectoryScanner::start` | `src/application/workspace/DirectoryScanner.cpp` | Root and filter → one worker with root job | Stop/join old worker and discard old pending entries | Implemented |
| 3 | `SidebarWidget::expandTree` | `DirectoryScanner::request` | `src/application/workspace/DirectoryScanner.cpp` | Unfiltered directory → queued direct-child job | Once per node generation; no request outside root | Implemented |
| 4 | `DirectoryScanner::scan` | `FileNameFilter::matches` | `src/application/workspace/FileNameFilter.cpp` | Basename → type OR followed by name AND | No parsing or FOX dependency | Implemented |
| 5 | `FOX poll timer` | `SidebarWidget::onPoll` | `src/application/ui/SidebarWidget.cpp` | Entries → path nodes and matching ancestors | GUI thread only; status includes unreadable count | Implemented |
| 6 | `SidebarWidget::onPoll` | `DirectoryScanner::take` | `src/application/workspace/DirectoryScanner.cpp` | Up to 512 entries, busy state and errors | Mutex exchange releases bounded producer backpressure | Implemented |
| 7 | `DirectoryScanner::scan` | `DirectoryScanner::publish` | `src/application/workspace/DirectoryScanner.cpp` | Entry → pending queue | Wait at 4096 entries; cancellation wakes producer | Implemented |

## 6. Gjenbruk og avhengigheter

[FUNC-012](Functionality-012--Work-Path-History.md) eier stigrense-policy.

FileNameFilter og DirectoryScanner brukes av samme tre ved oppstart, filter og rotbytte.
Ingen avhengighet til interpreter/renderer. Dokumentåpning bruker Application::open.

## 7. Verifikasjon

`WorkPathTest` for matching, historikk og scanner; `WorkPathGuiTest` for
native input, oppstart, filter og arbeidsrot. Eksisterende SidebarGuiTest og
WheelGuiTest skal fortsatt bestå. Bevis føres i P8 etter utførte tester.

## 8. Status, risiko og endringskonsekvenser

Implemented i P8. Store eller langsomme filsystemer kan bruke tid; GUI viser fremdrift
og kansellerer gammelt arbeid. Dette er treavgrensning, ikke en OS-sandbox.
