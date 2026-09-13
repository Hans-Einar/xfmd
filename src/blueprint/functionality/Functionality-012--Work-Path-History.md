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

Eie arbeidsroten, avgrensningspolicy og historikk som rene application-data.
Dette er uavhengig av dokumenthistorikk, filer i editoren og GUI-skanning.

## 2. Krav og akseptanse

UR-012 / AT-026 og UR-013 / AT-027; se
[kravspesifikasjonen](../../../xfmd_requirements.md).

## 3. Kontrakter og eierskap

WorkPathHistory eier kanonisk rot, home og en unik MRU-liste på maksimalt 32 stier.
activate validerer mappe/lesbarhet før mutasjon; broaderRoot foreslår home eller /.
contains sammenligner path-komponenter, og displayPath forkorter bare home-prefix.
WorkspacePanel persisterer entries via FOX-registry og bruker absolutte stier
ved historikkvalg selv om listen viser ~/… . Ingen FOX-avhengighet i tjenesten.

## 4. Atferd, tilstand og feil

Defaultrot er alltid home, også når en tidligere historikk lastes. Vellykket
activate flytter stien først i MRU; ugyldig sti kaster uten å endre tilstanden.
restore beholder manglende mapper slik at valg kan gi en forståelig feilmelding.
En rot utenfor home utvides også til home, så til /; / forblir /. Tjenesten
endrer aldri prosessens PWD, dokumentbuffer, synlighet eller dokumenthistorikk.

## 5. Plumbing

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `WorkspacePanel::setWorkPath` | `WorkPathHistory::activate` | `src/application/workspace/WorkPathHistory.cpp` | Path → canonical root, unique 32-entry MRU | Directory readability checked before commit; throws on failure | Implemented |
| 2 | `WorkspacePanel constructor` | `WorkPathHistory::restore` | `src/application/workspace/WorkPathHistory.cpp` | Registry paths → bounded MRU | Missing paths retained until explicitly activated | Implemented |
| 3 | `SidebarWidget broadenRoot callback` | `WorkPathHistory::broaderRoot` | `src/application/workspace/WorkPathHistory.cpp` | Current root → home or / | Pure proposal; setWorkPath validates separately | Implemented |
| 4 | `WorkspacePanel::remember / setWorkPath` | `WorkPathHistory::displayPath` | `src/application/workspace/WorkPathHistory.cpp` | Absolute path → ~/relative display label | Does not change stored or activated paths | Implemented |
| 5 | `DirectoryScanner::scan / SidebarWidget::onOpen` | `WorkPathHistory::contains` | `src/application/workspace/WorkPathHistory.cpp` | Root and resolved target → containment | Component comparison rejects sibling-prefix escape | Implemented |

## 6. Gjenbruk og avhengigheter

WorkspacePanel bruker rot, visningsetiketter og MRU ved alle brukerinnganger.
DirectoryScanner og SidebarWidget bruker contains for samme stigrense-policy.
Ingen avhengighet til interpreter, renderer eller dokumenthistorikk.

## 7. Verifikasjon

`WorkPathTest` for matching, historikk og scanner; `WorkPathGuiTest` for
native input, oppstart, filter og arbeidsrot. Eksisterende SidebarGuiTest og
WheelGuiTest består. Bevis: [P8-verifikasjon](../../../docs/evidence/P8.md).

## 8. Status, risiko og endringskonsekvenser

Implemented i P8. Store eller langsomme filsystemer kan bruke tid; GUI viser fremdrift
og kansellerer gammelt arbeid. Dette er treavgrensning, ikke en OS-sandbox.
