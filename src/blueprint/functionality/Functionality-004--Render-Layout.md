---
id: FUNC-004
kind: Functionality
audience: System
role: Service
owner: renderer
status: Proposed
scope: FirstRelease
requirements: UR-002, UR-008, SR-001, SR-003, SR-005, SR-008, SR-009, SR-011, SR-013
uses: none
---

# Functionality-004: Layout og visuell dokumentmodell

## 1. Hensikt og avgrensning

Beregn presentasjon fra semantisk modell uten FOX-avhengighet. Eie fontvalg som semantiske FontSpec-verdier, block/inline-layout, hit-regioner og mapping. Ingen Markdown-parsing, filåpning eller dokumenttilstand.

## 2. Krav og akseptanse

Krav: UR-002, UR-008, SR-001, SR-003, SR-005, SR-008, SR-009, SR-011, SR-013. Definisjoner og normativ akseptanse finnes i
[kravspesifikasjonen](../../../xfmd_requirements.md). Kapittel 7 konkretiserer beviset.

## 3. Kontrakter og eierskap

`IRenderer::layout(SemanticDocument, LayoutRequest, ITextMetrics&) -> LayoutResult`, `IRenderer::hitTest(RenderFrame, Point) -> HitResult`. MarkdownRenderer implementerer porten. RenderFrame eier display list, link regions og ankergeometri. ITextMetrics returnerer dimensjoner/baseline i avtalte layoutenheter; samme adapter brukes ved paint.

## 4. Atferd, tilstand og feil

Layout er deterministisk for samme modell, bredde og fontmål. Overskrifter, kode, lister og sitat får egne semantiske stiler. Brede kodeblokker får dokumentert horisontal overflow, vanlig tekst brytes. Ukjent node gir trygg tekstfallback. Ingen nettverk, skript eller utføring av lenker. Resize lager ny generasjon; frame fra gammel bredde kan ikke brukes til hit-testing eller sync.

## 5. Plumbing

Alle symboler og kildefiler i tabellen er **planlagte**, ikke implementert kode.
Bibliotekskall verifiseres mot valgt dependency-versjon før implementering.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `PreviewCoordinator::refresh / relayout` | `IRenderer::layout / MarkdownRenderer::layout` | `src/renderer/MarkdownRenderer.cpp` | Modell + request → RenderFrame | Feil returneres uten delvis frame. | Planned |
| 2 | `MarkdownRenderer::layout` | `BlockLayout::layout` | `src/renderer/BlockLayout.cpp` | Blokker + bredde → geometri | Ressursgrense stanser patologisk input. | Planned |
| 3 | `BlockLayout::layout` | `InlineLayout::layout` | `src/renderer/InlineLayout.cpp` | Runs + stil → linjer | Bevar source ranges gjennom wrapping. | Planned |
| 4 | `InlineLayout::layout` | `ITextMetrics::measure` | `src/contracts/ITextMetrics.h` | TextView + FontSpec → mål | Ingen FOX-type i kontrakten. | Planned |
| 5 | `FoxRenderHost::onPointer` | `IRenderer::hitTest / HitTester::hitTest` | `src/renderer/HitTester.cpp` | Gyldig frame + dokumentpunkt → link/none | Treff utfører ikke navigasjon. | Planned |

## 6. Gjenbruk og avhengigheter

Ingen andre functionality-kontrakter konsumeres; delte datatyper følger arkitekturen.

Preview bruker layout; FOX-host bruker hitTest; mappingtjenesten leser RenderFrame. Renderer skal ikke vokse til applikasjonskontroller. Nye primitiver krever kontraktreview.

## 7. Verifikasjon

AT-002, AT-008, AT-011, AT-013, AT-015, AT-018, AT-019, AT-021, AT-023: deterministiske fontmål, varierende bredder, nested blocks, treff langs lenkegrenser og visuell QA med ekte FOX. Planlagt `tests/renderer/RendererContractTest.cpp`.

Bevis: ingen applikasjonstest kjørt; testfiler ovenfor er planlagte. Ved implementering
oppgis kommando, fixture, miljø, commit og faktisk utfall. Strukturkontroll alene
oppfyller ikke atferdskravene.

## 8. Status, risiko og endringskonsekvenser

Proposed, revisjon 0.1, 2026-09-12. P0 avklarer tekstmåling, fontfallback, clipping og praktisk rich-text-host. Fake metrics alene beviser ikke visuell korrekthet.

Ved endret offentlig kontrakt: oppdater konsumentene i registeret, dette kallkartet,
berørte krav og kontrakttester i samme endring. Før status Ready skal relevante
P0-spørsmål være avgjort; før Verified skal kapittel 7 inneholde testbevis.
