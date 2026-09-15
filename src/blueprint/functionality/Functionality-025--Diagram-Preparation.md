---
id: FUNC-025
kind: Functionality
audience: System
role: Workflow
owner: application
status: Proposed
scope: Future
requirements: UR-039, UR-040, UR-041, SR-021, SR-022, SR-023
uses: FUNC-016, FUNC-024
---

# Functionality-025: Diagramforberedelse

## 1. Hensikt og avgrensning

Application-arbeidsflyt for worker-forberedelse, cache og publisering av diagramscener. Egen rolle fordi layoutressurser trenger font-/request-nøkler og scheduling; utvider ikke MathTypesetter/ImageDecoder med grafalgoritmer.

## 2. Krav og akseptanse

UR-039–041, SR-021–023; AT-059–064. Se [design og kontrakter](../../../docs/design/mermaid-integration.md) og [krav](../../../xfmd_requirements.md).

## 3. Kontrakter og eierskap

DiagramPreparation::prepare(model, request, metrics, cancellation) produserer en ny immutable presentasjonsmodell. Injisert IDiagramLayout brukes; DiagramCache eier begrenset LRU. Composition root kobler portene og lenker én Rust staticlib, sammensatt av separate parser-/layout-crates.

## 4. Atferd, tilstand og feil

Gjenbruk ParserWorker med én aktiv/én ventende jobb og ExportPipeline med egne snapshots. Request-identitet utvides med font/profile-generasjon; ikke bare DocumentToken. Cache deles ikke usynlig mellom tråder: separat cache per preview-/export-worker. Rust-kall er synkront og foreløpig ikke avbrytbart midt i algoritmen. Svar fra utdatert token/generasjon publiseres aldri. P26 må måle verste aksepterte graf før grensen godtas.

## 5. Plumbing

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `Application::initialize` | `DiagramPreparation::DiagramPreparation` | `src/application/diagrams/DiagramPreparation.cpp` | injected layout → prepare-objekt per worker | ingen global singleton | Planned |
| 2 | `ParserWorker::run / ExportPipeline::run` | `DiagramPreparation::prepare` | `src/application/diagrams/DiagramPreparation.cpp` | immutable modell + request key → dekorert modell | gjenbruk også EmbeddedVisuals i samme prepare-kjede | Planned |
| 3 | `DiagramPreparation::prepare` | `DiagramCache::find` | `src/application/diagrams/DiagramCache.cpp` | innhold + profil + font/backend → treff/miss | begrenset LRU; ingen diskcache | Planned |
| 4 | `DiagramPreparation::prepare` | `IDiagramLayout::layout` | `src/contracts/diagram/IDiagramLayout.h` | ren modell → scene | blokkfeil blir fallback | Planned |
| 5 | `DiagramPreparation::prepare` | `DiagramCache::insert` | `src/application/diagrams/DiagramCache.cpp` | vellykket scene → worker-lokal cache | ikke cache transient feil | Planned |
| 6 | `DisplayListPainter::paint` | `DiagramPainter::paint` | `src/application/adapters/DiagramPainter.cpp` | frame paths + semantisk palett → Cairo | bevar tekst som normale DrawRuns | Planned |


## 6. Gjenbruk og avhengigheter

FUNC-007 og FUNC-018 er konsumenter; gjenbruk FUNC-016 for shaping/tegning. Diagramlayout eies av [FUNC-024](Functionality-024--Diagram-Layout.md). Parserkobling skjer i composition root, ikke ved å kalle featureinterner. Eksisterende bilde-/matematikkforberedelse beholdes i samme injiserte kjede.

## 7. Verifikasjon

Planlagt DiagramPreparationTest, MermaidGuiTest og MermaidPdfTest: stale/font/resize-race, rask editing, cachegrense, kansellering, palettrepaint, glyph-markering og PDF-vektor/tekst. AT-059–064. Ingen runtime-test er utført i P25.

## 8. Status, risiko og endringskonsekvenser

Proposed. SR-022 lover kø-/inputgrenser og ingen GUI-kall, ikke hard preemption av Rust. Dersom P26 viser uakseptabel blokkering, må algoritmen få cancellation-seam eller worker-isolasjon før levering.

Planlagt akseptanse: AT-059, AT-060, AT-061, AT-062, AT-063, AT-064.
