---
id: FUNC-005
kind: Functionality
audience: System
role: Adapter
owner: application
status: Proposed
scope: FirstRelease
requirements: UR-002, UR-005, UR-008, SR-001, SR-008, SR-009, SR-010, SR-013
uses: FUNC-004
---

# Functionality-005: FOX-host for presentasjon

## 1. Hensikt og avgrensning

Koble en uavhengig renderer til FOX-vinduet. Eie FOX-ressurser, viewport, fontmåling, paint og inputoversettelse. Ingen Markdown-semantikk, fil-I/O eller historikk her.

## 2. Krav og akseptanse

Krav: UR-002, UR-005, UR-008, SR-001, SR-008, SR-009, SR-010, SR-013. Definisjoner og normativ akseptanse finnes i
[kravspesifikasjonen](../../../xfmd_requirements.md). Kapittel 7 konkretiserer beviset.

## 3. Kontrakter og eierskap

`FoxRenderHost::present(RenderFrame)`, `setViewport(ViewportCommand)`, `onPaint`, `onPointer`, `onViewportChanged`; `FoxTextMetrics::measure` implementerer ITextMetrics. Host mottar IRenderer-port for hitTest, ikke konkret rendererklasse. Frame skal matche aktiv dokument/revisjon/generasjon før det blir interaktivt.

## 4. Atferd, tilstand og feil

Paint tegner bare gyldig display list og clippes til viewport. Fontcache eies av GUI-tråden og bruker samme FontSpec som metrics. Pekekoordinater oversettes til dokumentkoordinater før hitTest. LinkActivated og ViewportChanged er typede hendelser; programmatisk scroll merkes slik at echo kan undertrykkes. Ved gammel preview vises status og interaksjon slås av.

## 5. Plumbing

Alle symboler og kildefiler i tabellen er **planlagte**, ikke implementert kode.
Bibliotekskall verifiseres mot valgt dependency-versjon før implementering.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `PreviewCoordinator::refresh / relayout` | `FoxRenderHost::present` | `src/application/adapters/FoxRenderHost.cpp` | Gyldig RenderFrame → aktivt frame | Avvis feil dokument/revisjon/generasjon. | Planned |
| 2 | `FOX paint event` | `FoxRenderHost::onPaint` | `src/application/adapters/FoxRenderHost.cpp` | Display list → FOX-tegning | Ingen parsing i paint; behold clipping. | Planned |
| 3 | `InlineLayout via ITextMetrics` | `FoxTextMetrics::measure` | `src/application/adapters/FoxTextMetrics.cpp` | FontSpec + tekst → mål | Fontfeil gir dokumentert fallback/feil. | Planned |
| 4 | `FOX pointer event / FoxRenderHost::onPointer` | `IRenderer::hitTest` | `src/contracts/IRenderer.h` | Dokumentpunkt → HitResult | LinkActivated sendes bare fra gyldig frame. | Planned |
| 5 | `ScrollCoordinator::applyTarget` | `FoxRenderHost::setViewport` | `src/application/adapters/FoxRenderHost.cpp` | Mål + opprinnelse/sekvens → viewport | Clamp og merk programmatisk ekko. | Planned |

## 6. Gjenbruk og avhengigheter

[FUNC-004](../functionality/Functionality-004--Render-Layout.md)

Brukes av presentasjon, preview, navigasjon og sync. Application.cpp kobler hendelser til koordinatorer; host kjenner ikke konkrete feature-arbeidsflyter.

## 7. Verifikasjon

AT-002, AT-005, AT-008, AT-011, AT-018, AT-019, AT-020, AT-023: FOX smoke, resize, glyphmål, koordinatkonvertering, lenketreff og teardown med pending event. Planlagt `tests/gui/FoxRenderHostTest.cpp` og manuell fontfixture.

Bevis: ingen applikasjonstest kjørt; testfiler ovenfor er planlagte. Ved implementering
oppgis kommando, fixture, miljø, commit og faktisk utfall. Strukturkontroll alene
oppfyller ikke atferdskravene.

## 8. Status, risiko og endringskonsekvenser

Proposed, revisjon 0.1, 2026-09-12. Installerte FOX 1.6.57-headere er undersøkt; konkret FXScrollArea-tegning skal prototypes i P0. Ingen fungerende host finnes ennå.

Ved endret offentlig kontrakt: oppdater konsumentene i registeret, dette kallkartet,
berørte krav og kontrakttester i samme endring. Før status Ready skal relevante
P0-spørsmål være avgjort; før Verified skal kapittel 7 inneholde testbevis.
