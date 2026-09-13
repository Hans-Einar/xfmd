---
id: FUNC-005
kind: Functionality
audience: System
role: Adapter
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-011, UR-002, UR-005, UR-008, SR-001, SR-008, SR-009, SR-010, SR-013
uses: FUNC-004
---

# Functionality-005: FOX-host for presentasjon

## 1. Hensikt og avgrensning

Koble en uavhengig renderer til FOX-vinduet. Eie FOX-ressurser, viewport, fontmåling, paint og inputoversettelse. Ingen Markdown-semantikk, fil-I/O eller historikk her.

## 2. Krav og akseptanse

Krav: UR-011, UR-002, UR-005, UR-008, SR-001, SR-008, SR-009, SR-010, SR-013. Definisjoner og normativ akseptanse finnes i
[kravspesifikasjonen](../../../xfmd_requirements.md). Kapittel 7 konkretiserer beviset.

## 3. Kontrakter og eierskap

FoxRenderHost er FXScrollArea-adapter. present/expect/invalidate styrer frame/interaktivitet. FoxTextMetrics::segments velger font per tegnsegment med samme ressurser for measure og paint. Host kjenner bare IRenderer for hitTest.

## 4. Atferd, tilstand og feil

Bare riktig dokumenttoken og viewportbredde aksepteres. Nytt dokument fjerner gammel frame. Pending endring gjør preview ikke-interaktiv. Paint clippes og søker synlige runs; CJK/andre glyphs kan bruke installert fallback-font. Ingen Markdown-regler i host.

FoxRenderHost må være enabled for native muse- og tastaturhendelser. Host
tegner Globe-primitiven med buer/linje uten emoji-fontavhengighet; renderer
bestemmer lenkemarkørens type og plassering.

FoxWheelScrollBar er delt FOX-adapter for begge scrollakser. Den bevarer
fraksjoner mellom små wheel-events og bruker FOXs eksisterende animasjon og
changed/command-varsler. Konstruktørene bytter barene før create(); widgets eier dem.

## 5. Plumbing

Tabellen beskriver implementerte kall. Navngitte hendelser er injiserte callbacks, ikke en global event bus.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `PreviewCoordinator present callback` | `FoxRenderHost::present` | `src/application/adapters/FoxRenderHost.cpp` | Frame → aktiv visning | Feil token/bredde avvises | Implemented |
| 2 | `FOX paint` | `FoxRenderHost::onPaint` | `src/application/adapters/FoxRenderHost.cpp` | Display list → native drawing | Clip og ingen parserkall | Implemented |
| 3 | `InlineLayout::layout` | `FoxTextMetrics::measure` | `src/application/adapters/FoxTextMetrics.cpp` | Text/font → mål | Fallback per segment | Implemented |
| 4 | `FoxRenderHost::onPaint` | `FoxTextMetrics::segments` | `src/application/adapters/FoxTextMetrics.cpp` | Text/font → FOX-fontsegmenter | Samme mål som ved layout | Implemented |
| 5 | `FOX pointer` | `FoxRenderHost::onPointer` | `src/application/adapters/FoxRenderHost.cpp` | Punkt → IRenderer::hitTest | Kun aktiv frame sender link callback | Implemented |
| 6 | `ScrollCoordinator setPreview callback` | `FoxRenderHost::setViewport` | `src/application/adapters/FoxRenderHost.cpp` | Y → clamped viewport | Programmatisk echo undertrykkes | Implemented |
| 7 | `FoxRenderHost constructor` | `FoxWheelScrollBar::replace` | `src/application/adapters/FoxWheelScrollBar.cpp` | Standard bar → presis wheel-adapter | Parent eier ny bar; før create | Implemented |
| 8 | `FOX wheel dispatch` | `FoxWheelScrollBar::onMouseWheel` | `src/application/adapters/FoxWheelScrollBar.cpp` | Delta/rest → target og FOX-timer | Clamp, behold delpiksel-rest, standard varsler | Implemented |

## 6. Gjenbruk og avhengigheter

[FUNC-004](../functionality/Functionality-004--Render-Layout.md)

Brukes av presentasjon, preview, navigasjon og sync. Application.cpp kobler hendelser til koordinatorer; host kjenner ikke konkrete feature-arbeidsflyter.

## 7. Verifikasjon

Relevante akseptanse-ID-er: AT-025, AT-002, AT-005, AT-008, AT-011, AT-018, AT-019, AT-020, AT-023.

`PresentationTest` kontrollerer ekte fontmål, frame-token, visningsbredde og stale-avvisning. Skjermbildet i P3 er visuelt kontrollert med CJK-fallback.

Evidence: [Fase P3](../../../docs/evidence/P3.md). Samlet kravdekning og eventuelle gjenstående begrensninger kontrolleres i P7; Implemented er ikke automatisk Verified.

Ny regresjonskontroll: [Native lenker og markører](../../../docs/evidence/document-links.md).

Regresjonsbevis: [Gesture og scrollgrenser](../../../docs/evidence/wheel-scrolling.md).

## 8. Status, risiko og endringskonsekvenser

Implemented i P3. Oppdater kontrakter, kallkart, konsumenter og tester i samme endring.
Rene porter og tydelig rolleeierskap er obligatorisk. Eventuelle senere avvik står i fasens bevisrapport.
