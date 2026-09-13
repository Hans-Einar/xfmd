---
id: FUNC-015
kind: Functionality
audience: System
role: Mechanism
owner: application
status: Ready
scope: Future
requirements: UR-011, UR-015, UR-016, SR-002, SR-015, SR-019
uses: FUNC-014, FUNC-006
---

# Functionality-015: Felles scrollinput og bevegelsespolicy

## 1. Hensikt og avgrensning

Eie normalisering, hastighet, akselerasjon, rest, mål og avslutning for wheel-scrolling i alle xfmd-flater. Trekk dette ansvaret ut av FUNC-005. Source-anchor-synkronisering forblir i FUNC-009.

## 2. Krav og akseptanse

UR-011, UR-015, UR-016, SR-002, SR-015, SR-019. Definisjoner: [krav](../../../xfmd_requirements.md).
Akseptanse: AT-012, AT-025, AT-029, AT-030, AT-035, AT-039.

## 3. Kontrakter og eierskap

Planlagte `ScrollInput{axis,delta,timestamp,source,origin}` og `ScrollProfile{speed,acceleration,k,v0,maxGain}` er application-verdier uten FOX. `ScrollDynamics::advance(input,range,now)` og `reset(reason)` er rene og får injisert monoton klokke. `FoxWheelScrollBar` normaliserer `FXEvent.code/120.0` til logiske wheel-enheter, håndterer FOX-target/varsler og har separat dynamikk per bar/akse. Unknown kilde er gyldig; FOXs core-eventvei kan ikke pålitelig identifisere fingerantall eller fingerløft.

Foreslått første profil: speed=1.5 (0.25–4), acceleration=false, k=0.5 (0–2), v0=8 wheel-enheter/s, maxGain=3 (1–5). Dette er tuningforslag; P10 sammenligner med kompatibilitetsprofil speed=1, acceleration=false.

For hvert normalisert inkrement q: `g = acceleration ? clamp(1 + k*max(0,v/v0-1),1,maxGain) : 1`; `dTarget = -q * baseUnit * speed * g`. baseUnit er FOXs ordinære line/wheelLines/page-baserte enhet i innholdspiksler. Bare nye inkrementer integreres; tidligere totaldistanse multipliseres aldri på nytt. Mottakerflaten ved Sync setter absolutt mål uten gain.

v estimeres fra summert absolutt input over et 80 ms tidsvindu, med faste 8 ms bokser. Koaleserte mengder fordeles ikke på oppdiktede fingerhendelser. 200 ms inaktivitet, akse/target-skifte, fortegnsskifte og nye preferences resetter hastighet/rest. Samme-timestamp/komprimerte hendelser summeres; monotont lokal tid brukes uten å anta at FOXs tidsstempel er fysisk gesture-tid.

## 4. Atferd, tilstand og feil

Alt=line og Ctrl=page beholder baseline og bypasser personlig gain/akselerasjon. Tastatur, scrollbar-drag, Sync og Restore bruker eksisterende presise posisjonskommandoer. Sign/natural-scroll respekteres fra input; ingen ny global invertering.

Først flyttes baseline-reglene uendret. Deretter eier adapteren én retargetbar timer, eksempelvis 8 ms ticks og opptil 80 ms settling; arvet onTimeWheel må da ikke kjøre parallelt. Animasjon følger et clamped mål, stopper ved eksakt heltallsmål og lager ingen ekstra kinetisk distanse etter input. Drag/key/nytt range/nytt dokument avbryter gammel wheel-jobb; ingen global inertiamotor. Flytende rest beholdes inne i området, overskytende press forkastes ved begge ender. Grenser, uendelig input og integer-konvertering sjekkes før FOX-kall.

## 5. Plumbing

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `FOX SEL_MOUSEWHEEL` | `FoxWheelScrollBar::onMouseWheel` | `src/application/adapters/FoxWheelScrollBar.cpp` | FXEvent → ScrollInput | en inputvei, konsumér én gang | Implemented |
| 2 | `FoxWheelScrollBar::onMouseWheel` | `ScrollDynamics::advance` | `src/application/scroll/ScrollDynamics.cpp` | input/profil/range → clamped mål | rest og hastighet per bar | Implemented |
| 3 | `FOX timeout` | `FoxWheelScrollBar::onMotionTick` | `src/application/adapters/FoxWheelScrollBar.cpp` | mål → position + SEL_CHANGED | slutt gir SEL_COMMAND; ingen dobbel timer | Planned |

## 6. Gjenbruk og avhengigheter

Alle scrollflater bruker samme policyinstans-type og samme profil: SidebarWidget, WorkPathList, EditorWidget, FoxRenderHost og Preferences-prøvefelt. FTR-006 eier brukerresultatet; FTR-004 bruker fortsatt FUNC-009 etter faktisk viewport-endring. Ingen feature-interne kall.

## 7. Verifikasjon

Deterministiske serier tester base uavhengig av gain, små ±1/120, batching innen samme tidsboks, reversering, idle, overflow og endepunkter. Ekte FOX/X11-tester skal teste selve tre-/tekst-/preview-flaten, ikke bare adaptermetoden. Mål physical touchpad separat; syntetisk fin-delta er ikke driverbevis.

AT-012, AT-025, AT-029, AT-030, AT-035, AT-039: planlagt verifikasjon; ingen implementasjonsbevis for utvidelsen.

## 8. Status, risiko og endringskonsekvenser

Revisjon 1.1, 2026-09-13. Alle nye kall i kapittel 5 er Planned.
P9 har avklart core-eventveien; se [P9](../../../docs/evidence/P9.md). P10-M1 trekker ut rest/clamp til ScrollDynamics; akselerasjon og egen timer følger M3. Første implementasjon bruker FOX-hook. XI2 er en eksplisitt senere adapter dersom målingen viser tapt nødvendig oppløsning; den må erstatte, ikke supplere, den samme core-eventstrømmen. Ingen endring av libinput, xfw eller system-FOX.
[Integrasjonsdesign](../../../softwareDesign.md) og [faseplan](../../../implementationPlan.md) gir kontekst.
