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

Levere filnavnmatches og treinnhold uten å blokkere FOX med rekursivt søk.
Root/historikk tilhører FUNC-012; dokumentåpning følger eksisterende tjenester.

## 2. Krav og akseptanse

UR-014 / AT-028; se [kravspesifikasjonen](../../../xfmd_requirements.md).

## 3. Kontrakter og eierskap

FileNameFilter eier typeflagg og navnemønster, uten FOX eller fil-I/O.
DirectoryScanner eier én worker, katalogjobber og maks 4096 ventende oppføringer.
GUI henter maks 512 per poll via take; worker kaller aldri FOX. SidebarWidget
eier FXTreeList-noder, path→node-register og hvilke mapper som er forespurt.
setRoot stopper/joiner gammel worker før gamle oppføringer slettes.

## 4. Atferd, tilstand og feil

Valgte typer kombineres med OR, deretter AND med navn. Ingen typer betyr alle;
uten wildcard matches delstreng, ellers hele navnet. ? teller UTF-8-tegn og
ASCII-bokstaver matches case-insensitivt. Tomt filter leser direkte barn ved
utvidelse; aktivt filter søker rekursivt og publiserer bare filer med treff.
GUI legger til forfedremapper og beholder roten ved null treff. Uleselige mapper
rapporteres; symlinkmapper traverseres ikke og filsymlinker utenfor rot utelates.
Køens backpressure vekkes ved kansellering. Refresh starter ny skanning.

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

| 8 | `DirectoryScanner::start / destructor` | `DirectoryScanner::stop` | `src/application/workspace/DirectoryScanner.cpp` | cancellation predicate under mutex → notify → join | prevents lost wakeup between predicate check and wait | Implemented |

## 6. Gjenbruk og avhengigheter

[FUNC-012](Functionality-012--Work-Path-History.md) eier stigrense-policy.

FileNameFilter og DirectoryScanner brukes av samme tre ved oppstart, filter og rotbytte.
Ingen avhengighet til interpreter/renderer. Dokumentåpning bruker Application::open.

## 7. Verifikasjon

`WorkPathTest` for matching, historikk og scanner; `WorkPathGuiTest` for
native input, oppstart, filter og arbeidsrot. Eksisterende SidebarGuiTest og
WheelGuiTest består. Bevis: [P8-verifikasjon](../../../docs/evidence/P8.md).

## 8. Status, risiko og endringskonsekvenser

Implemented i P8. Store eller langsomme filsystemer kan bruke tid; GUI viser fremdrift
og kansellerer gammelt arbeid. Dette er treavgrensning, ikke en OS-sandbox.

P27 regresjon avdekket lost wakeup ved stop/join. Predikatendring skjer nå
under samme mutex som condition-variable-ventingen. WorkPathTest utfører
100 raske start/stopp i tillegg til backpressure-scenariet.
