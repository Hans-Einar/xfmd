---
id: FTR-006
kind: Feature
audience: User
role: Workflow
owner: application
status: Proposed
scope: Future
requirements: UR-011, UR-015, UR-016, SR-002, SR-015, SR-019
uses: FUNC-005, FUNC-009, FUNC-010, FUNC-011, FUNC-014, FUNC-015
---

# Feature-006: Justerbar og presis scrolling

## 1. Hensikt og avgrensning

Gi brukeren én konsistent og justerbar scrollopplevelse i XFMD. Preferences + prøvefelt + bevegelse i alle flater har samlet akseptanse. Synkronisering er fortsatt FTR-004; denne featuren fungerer også i en enkelt flate.

## 2. Krav og akseptanse

UR-011, UR-015, UR-016, SR-002, SR-015, SR-019. Definisjoner: [krav](../../../xfmd_requirements.md).
Akseptanse: AT-012, AT-025, AT-029, AT-030, AT-035, AT-039.

## 3. Kontrakter og eierskap

Feature eier ingen ekstra scrollklasse. FUNC-014 eier profil/draft; FUNC-015 beregner bevegelse; widgetadaptere leverer viewportvarsler til FUNC-009. Bare brukerens wheel-input får gain. Programmatisk Sync/Restore og annen scrollbarbetjening beholder direkte posisjonssemantikk.

## 4. Atferd, tilstand og feil

Edit → Preferences → prøv hastighet/akselerasjon → OK oppdaterer levende flater. Cancel bevarer aktiv profil. Samme kode brukes i preview, editor, sidetre og historikk. Uidentifisert enhet gir fungerende wheel-fallback. Ingen raw-libinput-leser, global gesture-grab eller endringer i andre apper.

## 5. Plumbing

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `Edit Preferences` | `PreferencesService::begin` | `src/application/preferences/PreferencesService.cpp` | profil → dialog-draft | prøvefelt isolert | Planned |
| 2 | `FOX wheel i hvilken som helst xfmd-flate` | `ScrollDynamics::advance` | `src/application/scroll/ScrollDynamics.cpp` | normalisert input/profil → mål | clamp og reset | Planned |
| 3 | `Faktisk editor/preview viewport` | `ScrollCoordinator::onViewportChanged` | `src/application/scroll/ScrollCoordinator.cpp` | origin/token/anker → sync | ingen ny gain på mottaker | Planned |

## 6. Gjenbruk og avhengigheter

Gjenbruk FUNC-014/015, eksisterende hosts og FUNC-009. ScrollDynamics trekkes ut fra FUNC-005 og blir en selvstendig mekanisme med fire reelle konsumenter. Ingen feature kaller FTR-004s interne implementasjon.

## 7. Verifikasjon

Akseptanse kombinerer profil-roundtrip, tidsstyrte kurver, native dispatch og kildeanker-sync i begge retninger. Sammenlign speed=1/no-acceleration med baseline før personlig tuning.

AT-012, AT-025, AT-029, AT-030, AT-035, AT-039: planlagt verifikasjon; ingen implementasjonsbevis for utvidelsen.

## 8. Status, risiko og endringskonsekvenser

Revisjon 1.1, 2026-09-13. Alle nye kall i kapittel 5 er Planned.
P9 inputgate og P10 implementasjon. Fysisk touchpad-testing må supplere syntetiske smådeltaer; ingen garanti om rå gestureoppløsning i FOX 1.6.
[Integrasjonsdesign](../../../softwareDesign.md) og [faseplan](../../../implementationPlan.md) gir kontekst.
