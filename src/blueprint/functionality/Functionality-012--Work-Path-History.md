---
id: FUNC-012
kind: Functionality
audience: User
role: Service
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-012, UR-013
uses: none
---

# Functionality-012: Arbeidsrot og historikk

## 1. Hensikt og avgrensning

Arbeidsroten gir et fokusert filområde med gjenbrukbar filtrering og historikk.
Dokumentnavigasjon, parsing og lagring beholder eksisterende eiere.

## 2. Krav og akseptanse

UR-012, UR-013; se [kravspesifikasjonen](../../../xfmd_requirements.md).
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
| 1 | `WorkspacePanel::setWorkPath` | `WorkPathHistory::activate` | `src/application/workspace/WorkPathHistory.cpp` | Path → canonical root, unique 32-entry MRU | Directory readability checked before commit; throws on failure | Implemented |
| 2 | `WorkspacePanel constructor` | `WorkPathHistory::restore` | `src/application/workspace/WorkPathHistory.cpp` | Registry paths → bounded MRU | Missing paths retained until explicitly activated | Implemented |
| 3 | `SidebarWidget broadenRoot callback` | `WorkPathHistory::broaderRoot` | `src/application/workspace/WorkPathHistory.cpp` | Current root → home or / | Pure proposal; setWorkPath validates separately | Implemented |
| 4 | `WorkspacePanel::remember / setWorkPath` | `WorkPathHistory::displayPath` | `src/application/workspace/WorkPathHistory.cpp` | Absolute path → ~/relative display label | Does not change stored or activated paths | Implemented |
| 5 | `DirectoryScanner::scan / SidebarWidget::onOpen` | `WorkPathHistory::contains` | `src/application/workspace/WorkPathHistory.cpp` | Root and resolved target → containment | Component comparison rejects sibling-prefix escape | Implemented |

## 6. Gjenbruk og avhengigheter

FileNameFilter og DirectoryScanner brukes av samme tre ved oppstart, filter og rotbytte.
Ingen avhengighet til interpreter/renderer. Dokumentåpning bruker Application::open.

## 7. Verifikasjon

`WorkPathTest` for matching, historikk og scanner; `WorkPathGuiTest` for
native input, oppstart, filter og arbeidsrot. Eksisterende SidebarGuiTest og
WheelGuiTest skal fortsatt bestå. Bevis føres i P8 etter utførte tester.

## 8. Status, risiko og endringskonsekvenser

Implemented i P8. Store eller langsomme filsystemer kan bruke tid; GUI viser fremdrift
og kansellerer gammelt arbeid. Dette er treavgrensning, ikke en OS-sandbox.
