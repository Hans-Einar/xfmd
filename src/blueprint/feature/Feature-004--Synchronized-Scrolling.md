---
id: FTR-004
kind: Feature
audience: User
role: Workflow
owner: application
status: Proposed
scope: FirstRelease
requirements: UR-008, SR-002, SR-008, SR-009, SR-012, SR-013
uses: FUNC-004, FUNC-005, FUNC-009, FUNC-010
---

# Feature-004: Synkronisert scrolling mellom kilde og visning

## 1. Hensikt og avgrensning

Hold samme kildeavsnitt synlig i editor og rendret dokument når brukeren scroller i enten flate. Dette er en feature med selvstendig akseptanse; split view er forutsetning levert av workspace-functionality.

## 2. Krav og akseptanse

Krav: UR-008, SR-002, SR-008, SR-009, SR-012, SR-013. Definisjoner og normativ akseptanse finnes i
[kravspesifikasjonen](../../../xfmd_requirements.md). Kapittel 7 konkretiserer beviset.

## 3. Kontrakter og eierskap

Inngang: ViewportChanged med dokument/revisjon, origin og sekvens. Utgang: motsatt viewport oppdatert gjennom SourceAnchor, eller uttrykkelig Unavailable når mapping ikke kan brukes. Ingen egen sync-tilstand i begge widgets; ScrollCoordinator eier retning og guard.

## 4. Atferd, tilstand og feil

Synkronisering er aktiv bare når begge flater er synlige og frame stemmer. Konverter synlig posisjon til kildeanker og derfra til motsatt flate. Programmatisk ekko gir ikke ny runde. Nye fonter, bredde eller revisjon ugyldiggjør layout; hold anker til ny FrameReady. Mappingkvalitet må være synlig i diagnostikk/testbevis; prosentfallback skal ikke utgis for presisjon.

## 5. Plumbing

Alle symboler og kildefiler i tabellen er **planlagte**, ikke implementert kode.
Bibliotekskall verifiseres mot valgt dependency-versjon før implementering.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `EditorWidget / FoxRenderHost viewport event` | `ScrollCoordinator::onViewportChanged` | `src/application/scroll/ScrollCoordinator.cpp` | Event + frame-token | Ignore stale/skjult/echo. | Planned |
| 2 | `ScrollCoordinator::onViewportChanged` | `AnchorMapper::anchorAt` | `src/application/scroll/AnchorMapper.cpp` | Kildeviewport → SourceAnchor | FUNC-009 eier konvertering. | Planned |
| 3 | `ScrollCoordinator::onViewportChanged` | `AnchorMapper::map` | `src/application/scroll/AnchorMapper.cpp` | SourceAnchor + frame → mål | Exact/Approximate/Unavailable. | Planned |
| 4 | `ScrollCoordinator::applyTarget` | `EditorWidget::setSourceAnchor` | `src/application/ui/EditorWidget.cpp` | Byteanker + programmatisk origin | Clamp og undertrykk ekko. | Planned |
| 5 | `ScrollCoordinator::applyTarget` | `FoxRenderHost::setViewport` | `src/application/adapters/FoxRenderHost.cpp` | Geometri + programmatisk origin | Alternativ retning; clamp og undertrykk ekko. | Planned |
| 5 | `FrameReady etter resize` | `ScrollCoordinator::restoreAnchor` | `src/application/scroll/ScrollCoordinator.cpp` | Bevart anker + ny generasjon | Gammel mapping brukes aldri. | Planned |

## 6. Gjenbruk og avhengigheter

[FUNC-004](../functionality/Functionality-004--Render-Layout.md), [FUNC-005](../functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-009](../functionality/Functionality-009--Source-Anchor-Mapping.md), [FUNC-010](../functionality/Functionality-010--Workspace-Controls.md)

Samme mappingtjeneste som FTR-003. Interpreter-ranges og renderer-geometri gjenbrukes; ingen ekstra Markdown-parser eller separat prosentberegning. Framtidig IPC kan bruke ankerkontrakten, men trenger egen versjonert transport.

## 7. Verifikasjon

AT-008, AT-012, AT-018, AT-019, AT-022, AT-023: begge retninger, lange kodeblokker, nesting, Unicode, tom fil, resize, gammel frame og forsinket echo. Planlagt `tests/acceptance/SynchronizedScrollingTest.cpp`; mål samme avsnitt synlig, ikke bare at scrollbar beveger seg.

Bevis: ingen applikasjonstest kjørt; testfiler ovenfor er planlagte. Ved implementering
oppgis kommando, fixture, miljø, commit og faktisk utfall. Strukturkontroll alene
oppfyller ikke atferdskravene.

## 8. Status, risiko og endringskonsekvenser

Proposed, revisjon 0.1, 2026-09-12. Presis interpreter-mapping og fontlayout er P0-gates. IPC er Future og ikke skjult del av denne featureleveransen.

Ved endret offentlig kontrakt: oppdater konsumentene i registeret, dette kallkartet,
berørte krav og kontrakttester i samme endring. Før status Ready skal relevante
P0-spørsmål være avgjort; før Verified skal kapittel 7 inneholde testbevis.
