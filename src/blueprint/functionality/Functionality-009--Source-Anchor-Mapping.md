---
id: FUNC-009
kind: Functionality
audience: System
role: Mechanism
owner: application
status: Proposed
scope: FirstRelease
requirements: UR-005, UR-008, SR-002, SR-008, SR-009, SR-013
uses: FUNC-005
---

# Functionality-009: Kildeankre og viewport-koordinering

## 1. Hensikt og avgrensning

Oversett kildeankre til viewport og motsatt med eksplisitt mappingkvalitet. Tjenesten brukes både av sync og historikk. AnchorMapper er ren algoritme; ScrollCoordinator kobler den til FOX-adapterne.

## 2. Krav og akseptanse

Krav: UR-005, UR-008, SR-002, SR-008, SR-009, SR-013. Definisjoner og normativ akseptanse finnes i
[kravspesifikasjonen](../../../xfmd_requirements.md). Kapittel 7 konkretiserer beviset.

## 3. Kontrakter og eierskap

`AnchorMapper::map(SourceAnchor, RenderFrame) -> MappingResult`, `anchorAt(ViewportState, RenderFrame) -> SourceAnchor`; `ScrollCoordinator::onViewportChanged(ViewportEvent)`, `captureAnchor()`, `restoreAnchor(SourceAnchor, FrameToken)`, `applyTarget(ViewportCommand)`. Events har Origin/Sequence; positionsenheter er navngitte, ikke generiske int-linjer.

## 4. Atferd, tilstand og feil

Editorens tekstposisjon konverteres eksplisitt til snapshot-byteoffset. Rendergeometri gir nærmeste semantiske kildeanker; hidden syntax bruker relevant blokk. Eksakt mapping foretrekkes, tilnærming merkes, Unavailable deaktiverer sync. Layout-/revisjonsmismatch avvises. Guard undertrykker programmatisk ekko også når FOX leverer hendelsen senere. Restore etter resize/nav venter på korrekt FrameReady.

## 5. Plumbing

Alle symboler og kildefiler i tabellen er **planlagte**, ikke implementert kode.
Bibliotekskall verifiseres mot valgt dependency-versjon før implementering.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `EditorWidget / FoxRenderHost viewport event` | `ScrollCoordinator::onViewportChanged` | `src/application/scroll/ScrollCoordinator.cpp` | Origin/Sequence + posisjon | Ignore echo/skjult panel/stale frame. | Planned |
| 2 | `ScrollCoordinator::onViewportChanged / captureAnchor` | `AnchorMapper::anchorAt` | `src/application/scroll/AnchorMapper.cpp` | Viewport + modell/frame → SourceAnchor | Ukjent mapping returnerer kvalitet, ikke falsk presisjon. | Planned |
| 3 | `ScrollCoordinator::onViewportChanged / restoreAnchor` | `AnchorMapper::map` | `src/application/scroll/AnchorMapper.cpp` | Anker + gyldig frame → målgeometri | Clamp tomme/endrede dokumenter. | Planned |
| 4 | `ScrollCoordinator::applyTarget` | `EditorWidget::setSourceAnchor` | `src/application/ui/EditorWidget.cpp` | Byteanker + origin → FOX-tekstposisjon | Adapter konverterer, guard hindrer tilbakekobling. | Planned |
| 5 | `ScrollCoordinator::applyTarget` | `FoxRenderHost::setViewport` | `src/application/adapters/FoxRenderHost.cpp` | Dokumentgeometri + origin → viewport | Programmatisk hendelse merkes. | Planned |

## 6. Gjenbruk og avhengigheter

[FUNC-005](../functionality/Functionality-005--FOX-Presentation-Host.md)

FTR-004 bruker begge retninger; FUNC-008 bruker capture/restore. Renderer eier geometri, interpreter eier source ranges; application kombinerer uten å reparse. Ikke lag separat prosentformel i historikk eller UI.

## 7. Verifikasjon

AT-005, AT-008, AT-012, AT-018, AT-019, AT-023: Unicode, gjentatt tekst, whitespace/skjult syntaks, tom fil, resize, gammel frame, endret historikkfil og forsinket echo. Planlagt `tests/application/AnchorMapperTest.cpp` og `ScrollCoordinatorTest.cpp`.

Bevis: ingen applikasjonstest kjørt; testfiler ovenfor er planlagte. Ved implementering
oppgis kommando, fixture, miljø, commit og faktisk utfall. Strukturkontroll alene
oppfyller ikke atferdskravene.

## 8. Status, risiko og endringskonsekvenser

Proposed, revisjon 0.1, 2026-09-12. P0-mapping er blokkerende for presis sync. FOX getTopLine/setTopLine skal behandles som tekstposisjons-API, ikke uverifiserte linjenumre.

Ved endret offentlig kontrakt: oppdater konsumentene i registeret, dette kallkartet,
berørte krav og kontrakttester i samme endring. Før status Ready skal relevante
P0-spørsmål være avgjort; før Verified skal kapittel 7 inneholde testbevis.
