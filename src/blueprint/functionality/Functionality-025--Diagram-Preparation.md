---
id: FUNC-025
kind: Functionality
audience: System
role: Workflow
owner: application
status: Ready
scope: FirstRelease
requirements: UR-039, UR-040, UR-041, SR-021, SR-022, SR-023
uses: FUNC-016, FUNC-024
---

# Functionality-025: Diagramforberedelse

## 1. Hensikt og avgrensning

Application-arbeidsflyt for worker-forberedelse, cache og publisering av diagramscener. Egen rolle fordi layoutressurser trenger font-/request-nøkler og scheduling; utvider ikke MathTypesetter/ImageDecoder med grafalgoritmer.

## 2. Krav og akseptanse

UR-039–041, SR-021–023; AT-059–064. Se [design og kontrakter](../../../docs/design/mermaid-integration.md) og [krav](../../../xfmd_requirements.md).

## 3. Kontrakter og eierskap

DiagramPreparation::prepare(model, metrics, cancellation) produserer en ny immutable presentasjonsmodell. Injisert IDiagramLayout brukes; DiagramCache eier begrenset LRU. Composition root kobler portene og lenker én Rust staticlib, sammensatt av separate parser-/layout-crates.

## 4. Atferd, tilstand og feil

Gjenbruk ParserWorker med én aktiv/én ventende jobb og ExportPipeline med egne snapshots. Worker-ticket skiller også refresh av samme DocumentToken. Scene har fontidentitet; DiagramPlacement avviser uforenlig fontsett. Eksisterende FrameKey sjekker token, font og layoutgenerasjon ved publisering. Bredde/A4 påvirker bare placement og trenger ikke ny Rust-jobb. Cache deles ikke usynlig mellom tråder: separat cache per preview-/export-worker. Rust-kall er synkront med trådlokal kooperativ tidsgrense på to sekunder; ticket-/stoppkontroll kjøres mellom blokker og før/etter layout. Svar fra utdatert token/generasjon publiseres aldri. P26 må måle verste aksepterte graf før grensen godtas.

## 5. Plumbing

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `DiagramServices::preview` | `DiagramPreparation::DiagramPreparation` | `src/application/diagrams/DiagramPreparation.h` | injected layout → prepare-objekt per worker | ingen global singleton | Implemented |
| 2 | `DiagramServices::preview / DiagramServices::prepare` | `DiagramPreparation::prepare` | `src/application/diagrams/DiagramPreparation.cpp` | immutable modell + request key → dekorert modell | gjenbruk også EmbeddedVisuals i samme prepare-kjede | Implemented |
| 3 | `DiagramPreparation::prepare` | `DiagramCache::find` | `src/application/diagrams/DiagramCache.cpp` | innhold + profil + font/backend → treff/miss | begrenset LRU; ingen diskcache | Implemented |
| 4 | `DiagramPreparation::prepare` | `IDiagramLayout::layout` | `src/contracts/diagram/IDiagramLayout.h` | ren modell → scene | blokkfeil blir fallback | Implemented |
| 5 | `DiagramPreparation::prepare` | `DiagramCache::insert` | `src/application/diagrams/DiagramCache.cpp` | vellykket scene → worker-lokal cache | ikke cache transient feil | Implemented |
| 6 | `DisplayListPainter::paint` | `DiagramPainter::paint` | `src/application/adapters/DiagramPainter.cpp` | SVG + semantisk palett → librsvg/Cairo | ingen separate diagrametikett-runs | Implemented |


## 6. Gjenbruk og avhengigheter

FUNC-007 og FUNC-018 er konsumenter; gjenbruk FUNC-016 for shaping/tegning. Diagramlayout eies av [FUNC-024](Functionality-024--Diagram-Layout.md). Parserkobling skjer i composition root, ikke ved å kalle featureinterner. Eksisterende bilde-/matematikkforberedelse beholdes i samme injiserte kjede.

## 7. Verifikasjon

DiagramPreparationTest dekker feil, fontidentitet og cachegrense. DiagramWorkerTest verifiserer kansellering av gammel prepare-jobb med samme token. MermaidGuiTest og MermaidPdfTest dekker native presentasjon og eksport. Se [P29](../../../docs/evidence/P29.md).

## 8. Status, risiko og endringskonsekvenser

P31/P32: [Gjeldende SVG-/rutebeslutning](../../../docs/design/mermaid-svg-routing.md) erstatter tidligere native etiketttegning. Historiske tester nedenfor gjelder P25–P30; ny atferd er Ready frem til nytt testbevis.

Implementert. P26 avdekket dyr kantruting og la til kooperative checkpoints. Ingen hard preemption eller global cache. Hver preview-worker eier egne font-/layout-/cacheobjekter; PDF-jobben har tilsvarende kortlivet prepare-kjede.

Akseptanse: AT-059, AT-060, AT-061, AT-062, AT-063, AT-064.
