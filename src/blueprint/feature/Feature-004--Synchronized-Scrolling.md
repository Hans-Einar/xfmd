---
id: FTR-004
kind: Feature
audience: User
role: Workflow
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-008, SR-002, SR-008, SR-009, SR-012, SR-013, UR-016, SR-019
uses: FUNC-004, FUNC-005, FUNC-009, FUNC-010, FUNC-015
---

# Feature-004: Synkronisert scrolling mellom kilde og visning

## 1. Hensikt og avgrensning

Hold samme kildeavsnitt synlig i editor og rendret dokument når brukeren scroller i enten flate. Dette er en feature med selvstendig akseptanse; split view er forutsetning levert av workspace-functionality.

## 2. Krav og akseptanse

Krav: UR-008, SR-002, SR-008, SR-009, SR-012, SR-013. Definisjoner og normativ akseptanse finnes i
[kravspesifikasjonen](../../../xfmd_requirements.md). Kapittel 7 konkretiserer beviset.

## 3. Kontrakter og eierskap

Feature koordineres av ScrollCoordinator og bruker samme ankerkontrakt som navigasjon. Interpreter/renderer leverer source ranges og linje-/blokkgeometri. Ingen ekstra parser og ingen total-prosent-algoritme.

**Implementert utvidelse 1.1 (P11):** Bevegelsespolicy kjøres bare på inputflaten. Synkroniserte absolutte posisjoner skal aldri akselereres på nytt. Sidemapping erstatter antakelsen om ett kontinuerlig y-plan.

## 4. Atferd, tilstand og feil

Begge flater synkroniseres i split-modus. Scrolling i enkeltvisning oppdaterer leseposisjonen uten å flytte skjult flate. Resize og ny revisjon ugyldiggjør mapping; FrameReady gjenoppretter anker. Blank/ukjent mapping er deaktivert. Kodelinjer med identisk tekst beholder hver sin kildeposisjon.

## 5. Plumbing

Implemented-rader beskriver gjeldende plumbing; historiske fasebevis identifiserer tidligere baseline. Navngitte hendelser er injiserte callbacks, ikke en global event bus.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `Ekte FOX viewport-event` | `ScrollCoordinator::onViewportChanged` | `src/application/scroll/ScrollCoordinator.cpp` | Origin/token/posisjon → anker | Guard stopper loops | Implemented |
| 2 | `ScrollCoordinator::onViewportChanged` | `AnchorMapper::anchorAt` | `src/application/scroll/AnchorMapper.cpp` | Preview-y → source | Linjegeometri, ikke prosent | Implemented |
| 3 | `ScrollCoordinator::onViewportChanged` | `AnchorMapper::map` | `src/application/scroll/AnchorMapper.cpp` | Source → y | Hidden syntax har blokkfallback | Implemented |
| 4 | `ScrollCoordinator setEditor callback` | `EditorWidget::setSourceAnchor` | `src/application/ui/EditorWidget.cpp` | Source → editor viewport | Clamped tekstposisjon | Implemented |
| 5 | `ScrollCoordinator setPreview callback` | `FoxRenderHost::setViewport` | `src/application/adapters/FoxRenderHost.cpp` | Y → preview viewport | Clamped geometri | Implemented |
| 6 | `PreviewCoordinator present callback` | `ScrollCoordinator::setFrame` | `src/application/scroll/ScrollCoordinator.cpp` | Ny generasjon → restore | Stale frame brukes aldri | Implemented |
| 7 | `Sync receiver` | `ScrollCoordinator::onViewportChanged` | `src/application/scroll/ScrollCoordinator.cpp` | merket origin + FrameKey → guard | ingen dobbel gain | Implemented |

## 6. Gjenbruk og avhengigheter

[FUNC-004](../functionality/Functionality-004--Render-Layout.md), [FUNC-005](../functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-009](../functionality/Functionality-009--Source-Anchor-Mapping.md), [FUNC-010](../functionality/Functionality-010--Workspace-Controls.md)

Samme mappingtjeneste som FTR-003. Interpreter-ranges og renderer-geometri gjenbrukes; ingen ekstra Markdown-parser eller separat prosentberegning. Framtidig IPC kan bruke ankerkontrakten, men trenger egen versjonert transport.

## 7. Verifikasjon

Relevante akseptanse-ID-er: AT-008, AT-012, AT-018, AT-019, AT-022, AT-023.

`ScrollTest` og `ScrollingTest` bekrefter begge retninger, samme avsnitt, gjentatt tekst, stale avvisning og resize-restore. GUI-testen bruker et dokument med 80 seksjoner.

Evidence: [Fase P5](../../../docs/evidence/P5.md). Samlet kravdekning og eventuelle gjenstående begrensninger kontrolleres i P7; Implemented er ikke automatisk Verified.

Utvidelsen krever AT-030, AT-039. Dette er planlagt dekning, ikke nye testbevis.

## 8. Status, risiko og endringskonsekvenser

**Implemented 1.1:** [P11-bevis](../../../docs/evidence/P11.md) beskriver ny kode og kontroller. Historiske bevis nedenfor gjelder baseline, ikke automatisk de nye kravene.


Implemented i P5. Oppdater kontrakter, kallkart, konsumenter og tester i samme endring.
Rene porter og tydelig rolleeierskap er obligatorisk. Eventuelle senere avvik står i fasens bevisrapport.
