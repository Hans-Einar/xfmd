---
id: FUNC-009
kind: Functionality
audience: System
role: Mechanism
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-005, UR-008, SR-002, SR-008, SR-009, SR-013, UR-017, SR-019
uses: FUNC-005
---

# Functionality-009: Kildeankre og viewport-koordinering

## 1. Hensikt og avgrensning

Oversett kildeankre til viewport og motsatt med eksplisitt mappingkvalitet. Tjenesten brukes både av sync og historikk. AnchorMapper er ren algoritme; ScrollCoordinator kobler den til FOX-adapterne.

## 2. Krav og akseptanse

Krav: UR-005, UR-008, SR-002, SR-008, SR-009, SR-013. Definisjoner og normativ akseptanse finnes i
[kravspesifikasjonen](../../../xfmd_requirements.md). Kapittel 7 konkretiserer beviset.

## 3. Kontrakter og eierskap

AnchorMapper::map og anchorAt er rene funksjoner over RenderFrame. ScrollCoordinator eier aktivt token, pending restore, siste anker, split-mode og ekko-/sekvensguard. setEditor/setPreview injiseres som adaptercallbacks; captureAnchor/restoreAnchor deles med historikk.

**Implementert utvidelse 1.1 (P11):** AnchorMapper gir VisualLocation med sideindeks og points; host mapper visningskoordinater gjennom ViewTransform før lookup. Klikk/scroll i page gap velger nærmeste dokumentkant med deterministisk tie-break mot neste side. Sync og Restore er eksplisitte origins, uten wheel-gain.

## 4. Atferd, tilstand og feil

Byteområder og dokumentgeometri brukes begge veier. Smaleste kildeområde foretrekkes ved source→viewport; nærmeste linjeregion ved motsatt retning. Hidden syntax bruker blokkområde, transformert tekst er Approximate. Kodelinjer får egne fysisk forankrede ranges, også ved gjentatt tekst. Gammelt token/frame avvises; resize restaurerer anker etter ny layout. Programmatisk echo og gammel sekvens ignoreres.

## 5. Plumbing

Implemented-rader beskriver gjeldende plumbing; historiske fasebevis identifiserer tidligere baseline. Navngitte hendelser er injiserte callbacks, ikke en global event bus.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `EditorWidget / FoxRenderHost viewport callback` | `ScrollCoordinator::onViewportChanged` | `src/application/scroll/ScrollCoordinator.cpp` | Origin + posisjon + token → guard | Stale/echo/sekvens avvises | Implemented |
| 2 | `ScrollCoordinator::onViewportChanged` | `AnchorMapper::anchorAt` | `src/application/scroll/AnchorMapper.cpp` | Preview-y → kildeanker | Unavailable ved tom mapping | Implemented |
| 3 | `ScrollCoordinator::onViewportChanged / restoreAnchor` | `AnchorMapper::map` | `src/application/scroll/AnchorMapper.cpp` | Kildeanker → dokument-y | Clamp/nærmeste relevante område | Implemented |
| 4 | `ScrollCoordinator setEditor callback` | `EditorWidget::setSourceAnchor` | `src/application/ui/EditorWidget.cpp` | Raw byte → projected FOX-posisjon | Undertrykker programmatisk callback | Implemented |
| 5 | `ScrollCoordinator setPreview callback` | `FoxRenderHost::setViewport` | `src/application/adapters/FoxRenderHost.cpp` | Y → FOX-scrollposisjon | Undertrykker programmatisk callback | Implemented |
| 6 | `PreviewCoordinator present callback` | `ScrollCoordinator::setFrame` | `src/application/scroll/ScrollCoordinator.cpp` | FrameReady → restore | Kun forventet token | Implemented |
| 7 | `ScrollCoordinator restore` | `AnchorMapper::map` | `src/application/scroll/AnchorMapper.cpp` | SourceAnchor + PageLayout → VisualLocation | Approximate eller Unavailable | Implemented |

## 6. Gjenbruk og avhengigheter

[FUNC-005](../functionality/Functionality-005--FOX-Presentation-Host.md)

FTR-004 bruker begge retninger; FUNC-008 bruker capture/restore. Renderer eier geometri, interpreter eier source ranges; application kombinerer uten å reparse. Ikke lag separat prosentformel i historikk eller UI.

## 7. Verifikasjon

Relevante akseptanse-ID-er: AT-005, AT-008, AT-012, AT-018, AT-019, AT-023.

`ScrollTest` verifiserer gjentatte kodelinjer, mapping begge veier, tom fil, stale token og forsinket/merket echo. `ScrollingTest` bruker ekte FOX-scroll og resize.

Evidence: [Fase P5](../../../docs/evidence/P5.md). Samlet kravdekning og eventuelle gjenstående begrensninger kontrolleres i P7; Implemented er ikke automatisk Verified.

Utvidelsen krever AT-031, AT-039. Dette er planlagt dekning, ikke nye testbevis.

## 8. Status, risiko og endringskonsekvenser

**Implemented 1.1:** [P11-bevis](../../../docs/evidence/P11.md) beskriver ny kode og kontroller. Historiske bevis nedenfor gjelder baseline, ikke automatisk de nye kravene.


Implemented i P5. Oppdater kontrakter, kallkart, konsumenter og tester i samme endring.
Rene porter og tydelig rolleeierskap er obligatorisk. Eventuelle senere avvik står i fasens bevisrapport.
