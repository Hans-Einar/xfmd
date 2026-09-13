---
id: FTR-001
kind: Feature
audience: User
role: Workflow
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-001, UR-002, UR-009, SR-001, SR-003, SR-004, SR-005, SR-011, SR-012, SR-013, UR-017
uses: FUNC-001, FUNC-002, FUNC-003, FUNC-004, FUNC-005, FUNC-007, FUNC-010, FUNC-016, FUNC-017
---

# Feature-001: Lesbar Markdown-presentasjon

## 1. Hensikt og avgrensning

Brukeren åpner et lokalt dokument og får lesbar native Markdown med tydelig typografi. Dette er samlet brukerresultat på tvers av lag. Application eier orkestrering; interpreter eier semantikk og renderer eier presentasjon. Load/read/paint er gjenbrukt functionality, ikke underfeatures.

## 2. Krav og akseptanse

Krav: UR-001, UR-002, UR-009, SR-001, SR-003, SR-004, SR-005, SR-011, SR-012, SR-013. Definisjoner og normativ akseptanse finnes i
[kravspesifikasjonen](../../../xfmd_requirements.md). Kapittel 7 konkretiserer beviset.

## 3. Kontrakter og eierskap

Åpning bruker DocumentCoordinator og PreviewCoordinator. Ingen egen featureklasse. Pipeline konsumerer IInterpreter, IRenderer og ITextMetrics; application composition root registrerer cmark/MarkdownRenderer/FOX-host.

**Implementert utvidelse 1.1 (P11):** Lesing får Window wrap og A4. Samme dokumentmodell brukes i begge; fit-width skalerer papir uten ny wrapping.

## 4. Atferd, tilstand og feil

Lokalt dokument lastes, får semantisk modell og native layout. .txt går samme vei uten Markdown-tolkning. Parse-/layoutfeil gir tydelig status og ingen aktiv gammel frame. Live preview og worker-pipeline er integrert i P4; lenkeaktivering med historikk i P6.

## 5. Plumbing

Implemented-rader beskriver gjeldende plumbing; historiske fasebevis identifiserer tidligere baseline. Navngitte hendelser er injiserte callbacks, ikke en global event bus.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `NavigationCoordinator::openTarget` | `DocumentCoordinator::requestOpen` | `src/application/document/DocumentCoordinator.cpp` | Path → committed snapshot | Dirty/I/O før commit | Implemented |
| 2 | `DocumentCoordinator opened callback` | `PreviewCoordinator::refresh` | `src/application/preview/PreviewCoordinator.cpp` | Snapshot → pipeline | Åpning rendres | Implemented |
| 3 | `ParserWorker::run` | `IInterpreter::parse` | `src/contracts/IInterpreter.h` | Snapshot → model | Typet feil | Implemented |
| 4 | `PreviewCoordinator::relayout` | `IRenderer::layout` | `src/contracts/IRenderer.h` | Model/metrics → frame | Gyldig token/bredde | Implemented |
| 5 | `PreviewCoordinator present callback` | `FoxRenderHost::present` | `src/application/adapters/FoxRenderHost.cpp` | Frame → native visning | Ingen ressurslasting | Implemented |
| 6 | `View mode` | `PreviewCoordinator::setLayoutProfile` | `src/application/preview/PreviewCoordinator.cpp` | profil → FrameKey | bevar source anchor | Implemented |

## 6. Gjenbruk og avhengigheter

[FUNC-001](../functionality/Functionality-001--Document-Session.md), [FUNC-002](../functionality/Functionality-002--Local-File-Storage.md), [FUNC-003](../functionality/Functionality-003--Markdown-Interpretation.md), [FUNC-004](../functionality/Functionality-004--Render-Layout.md), [FUNC-005](../functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-007](../functionality/Functionality-007--Preview-Pipeline.md)

Deler hele pipeline med FTR-002; navigasjon bruker samme åpning; sync leser samme ankergeometri. Ingen alternativ renderer inne i live-preview-feature.

## 7. Verifikasjon

Relevante akseptanse-ID-er: AT-001, AT-002, AT-009, AT-011, AT-013, AT-014, AT-015, AT-021, AT-022, AT-023.

`InterpreterTest`, `RendererTest` og `PresentationTest` passerer. P3-skjermbilde bekrefter native typografi. Full ressursbenchmark og endelig AT-matrise kommer i P7.

Evidence: [Fase P3](../../../docs/evidence/P3.md). Samlet kravdekning og eventuelle gjenstående begrensninger kontrolleres i P7; Implemented er ikke automatisk Verified.

Utvidelsen krever AT-031. Dette er planlagt dekning, ikke nye testbevis.

## 8. Status, risiko og endringskonsekvenser

**Implemented 1.1:** [P11-bevis](../../../docs/evidence/P11.md) beskriver ny kode og kontroller. Historiske bevis nedenfor gjelder baseline, ikke automatisk de nye kravene.


Implemented i P3. Oppdater kontrakter, kallkart, konsumenter og tester i samme endring.
Rene porter og tydelig rolleeierskap er obligatorisk. Eventuelle senere avvik står i fasens bevisrapport.
