---
id: FUNC-005
kind: Functionality
audience: System
role: Adapter
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-030, UR-031, UR-011, UR-002, UR-005, UR-008, SR-001, SR-008, SR-009, SR-010, SR-013, UR-017, SR-016, SR-019
uses: FUNC-004, FUNC-015, FUNC-016
---

# Functionality-005: FOX-host for presentasjon

## 1. Hensikt og avgrensning

Koble en uavhengig renderer til FOX-vinduet. Eie FOX-ressurser, viewport, fontmåling, paint og inputoversettelse. Ingen Markdown-semantikk, fil-I/O eller historikk her.

## 2. Krav og akseptanse

Krav: UR-011, UR-002, UR-005, UR-008, SR-001, SR-008, SR-009, SR-010, SR-013. Definisjoner og normativ akseptanse finnes i
[kravspesifikasjonen](../../../xfmd_requirements.md). Kapittel 7 konkretiserer beviset.

## 3. Kontrakter og eierskap

FoxRenderHost er FXScrollArea-adapter. present/expect/invalidate styrer frame/interaktivitet. SharedTextMetrics former tekst; DisplayListPainter tegner samme glyphdata gjennom FontCatalog. Host kjenner bare IRenderer for hitTest.

**Implementert utvidelse 1.1 (P11):** Behold FOX-host, input-enable og viewportvarsler. Wheelpolicy flyttes til FUNC-015, fontmåling/glyphtegning til FUNC-016. Host eier ViewTransform for points/zoom/DPI og page gaps. present må erstatte frame.width==viewport_w med profilbasert validering.

## 4. Atferd, tilstand og feil

Bare riktig dokumenttoken og viewportbredde aksepteres. Nytt dokument fjerner gammel frame. Pending endring gjør preview ikke-interaktiv. Paint clippes og søker synlige runs; CJK/andre glyphs kan bruke installert fallback-font. Ingen Markdown-regler i host.

FoxRenderHost må være enabled for native muse- og tastaturhendelser. Host
tegner Unicode-markøren ↗ gjennom den vanlige Pango/Cairo-tekstveien; renderer
bestemmer lenkemarkørens type og plassering.

FoxWheelScrollBar er delt FOX-adapter for begge scrollakser. Den bevarer
fraksjoner mellom små wheel-events og eier en retargetbar timer og
changed/command-varsler. Konstruktørene bytter barene før create(); widgets eier dem.

Preview eier balanserte press/release og frigjør egen grab før enhver callback.
Bare venstreklikk på samme mål uten drag/chord aktiverer lenken; andre knapper
ignoreres i dokumentflaten, men beholder fokus. Stale frame kansellerer klikket.

## 5. Plumbing

Implemented-rader beskriver gjeldende plumbing; historiske fasebevis identifiserer tidligere baseline. Navngitte hendelser er injiserte callbacks, ikke en global event bus.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `PreviewCoordinator present callback` | `FoxRenderHost::present` | `src/application/adapters/FoxRenderHost.cpp` | Frame → aktiv visning | Feil token/bredde avvises | Implemented |
| 2 | `FOX paint` | `FoxRenderHost::onPaint` | `src/application/adapters/FoxRenderHost.cpp` | Display list → native drawing | Clip og ingen parserkall | Implemented |
| 3 | `InlineLayout::layout` | `SharedTextMetrics::measure` | `src/application/adapters/SharedTextMetrics.cpp` | Text/font → mål | Fallback per segment | Implemented |
| 4 | `DisplayListPainter::text` | `FontCatalog::font` | `src/application/adapters/FontCatalog.cpp` | Fontidentitet → native font | Endret font avvises | Implemented |
| 5 | `FOX pointer` | `FoxRenderHost::onPointer` | `src/application/adapters/FoxRenderHost.cpp` | Punkt → IRenderer::hitTest | Kun aktiv frame sender link callback | Implemented |
| 6 | `ScrollCoordinator setPreview callback` | `FoxRenderHost::setViewport` | `src/application/adapters/FoxRenderHost.cpp` | Y → clamped viewport | Programmatisk echo undertrykkes | Implemented |
| 7 | `FoxRenderHost constructor` | `FoxWheelScrollBar::replace` | `src/application/adapters/FoxWheelScrollBar.cpp` | Standard bar → presis wheel-adapter | Parent eier ny bar; før create | Implemented |
| 8 | `FOX wheel dispatch` | `FoxWheelScrollBar::onMouseWheel` | `src/application/adapters/FoxWheelScrollBar.cpp` | Delta/rest → target og FOX-timer | Clamp, behold delpiksel-rest, standard varsler | Implemented |
| 9 | `FoxRenderHost::present` | `ViewTransform::configure` | `src/application/adapters/ViewTransform.cpp` | PageLayout + viewport → transform | zoom er ikke reflow | Implemented |
| 10 | `FoxRenderHost paint` | `DisplayListPainter::paint` | `src/application/adapters/DisplayListPainter.cpp` | frame + target → pixels | felles glyphgrunnlag | Implemented |

| 13 | `FoxRenderHost::onPaint` | `FoxCairoCanvas::present` | `src/application/adapters/FoxCairoCanvas.cpp` | Cairo viewportbuffer → FOX-pixmap | native ressurslevetid; ingen Cairo Xlib-device | Implemented |

| 14 | `FOX press` | `FoxRenderHost::onButtonPress` | `src/application/adapters/FoxRenderHost.cpp` | Knapp/frame/lenke → klikktilstand og grab | Chord kansellerer aktivering | Implemented |
| 15 | `FOX grab loss` | `FoxRenderHost::onUngrabbed` | `src/application/adapters/FoxRenderHost.cpp` | Tap av grab → nullstill klikk | Ingen lenkecallback | Implemented |
| 20 | `Application::changeReadingColors / applyAppearance` | `FoxRenderHost::setReadingColors` | `src/application/adapters/FoxRenderHost.cpp` | Lesefarger → repaint/profil | Ingen dokumentmutasjon | Implemented |

## 6. Gjenbruk og avhengigheter

[FUNC-004](../functionality/Functionality-004--Render-Layout.md)

Brukes av presentasjon, preview, navigasjon og sync. Application.cpp kobler hendelser til koordinatorer; host kjenner ikke konkrete feature-arbeidsflyter.

## 7. Verifikasjon

Relevante akseptanse-ID-er: AT-025, AT-002, AT-005, AT-008, AT-011, AT-018, AT-019, AT-020, AT-023.

`PresentationTest` kontrollerer ekte fontmål, frame-token, visningsbredde og stale-avvisning. Skjermbildet i P3 er visuelt kontrollert med CJK-fallback.

Evidence: [Fase P3](../../../docs/evidence/P3.md). Samlet kravdekning og eventuelle gjenstående begrensninger kontrolleres i P7; Implemented er ikke automatisk Verified.

Ny regresjonskontroll: [Native lenker og markører](../../../docs/evidence/document-links.md).

Regresjonsbevis: [Gesture og scrollgrenser](../../../docs/evidence/wheel-scrolling.md).

Utvidelsen krever AT-031, AT-036, AT-039. Dette er planlagt dekning, ikke nye testbevis.

## 8. Status, risiko og endringskonsekvenser

**Implemented 1.1:** [P11-bevis](../../../docs/evidence/P11.md) beskriver ny kode og kontroller. Historiske bevis nedenfor gjelder baseline, ikke automatisk de nye kravene.


Implemented i P3. Oppdater kontrakter, kallkart, konsumenter og tester i samme endring.
Rene porter og tydelig rolleeierskap er obligatorisk. Eventuelle senere avvik står i fasens bevisrapport.

P21 utvider samme eier med UR-030/031, AT-050, AT-051. ReadingColors er rene
lesepreferanser; PreviewColorControls bruker UiRow/UiContext. Bare FOX-host gir
DisplayListPainter en skjermpalett; PDF beholder standardfargene. DecorationRole
bevarer semantisk rolle gjennom PageComposer, uten FOX-typer i renderer/kontrakter.
Profiler lagres additivt i ReadingLight/ReadingDark via eksisterende preferences-service.
Live endring er repaint; commit ved release, med rollback ved skrivefeil.

P21: [AT-050/051, regresjoner og skjermbilder](../../../docs/evidence/P21.md).
