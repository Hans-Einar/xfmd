---
id: FUNC-003
kind: Functionality
audience: System
role: Service
owner: interpreter
status: Implemented
scope: FirstRelease
requirements: UR-039, UR-036, UR-037, UR-005, UR-002, SR-001, SR-003, SR-004, SR-005, SR-009, SR-011, SR-013
uses: FUNC-023
---

# Functionality-003: Markdown-tolkning og semantisk modell

## 1. Hensikt og avgrensning

Konverter kildetekst til en stabil, eid semantisk modell. cmark er første adapter, ikke del av den offentlige kontrakten. Interpreter kjenner ikke fonter, FOX eller navigasjonshistorikk.

## 2. Krav og akseptanse

Krav: UR-005, UR-002, SR-001, SR-003, SR-004, SR-005, SR-009, SR-011, SR-013. Definisjoner og normativ akseptanse finnes i
[kravspesifikasjonen](../../../xfmd_requirements.md). Kapittel 7 konkretiserer beviset.

## 3. Kontrakter og eierskap

CmarkInterpreter implementerer IInterpreter::parse. ModelBuilder oversetter AST til eide SemanticBlock/InlineRun-verdier; SourceMapBuilder oversetter kildekolonner til raw UTF-8-offsets. Bare adapteren inkluderer cmark. Resultatet er shared_ptr<const SemanticDocument>; feil er Error.

## 4. Atferd, tilstand og feil

cmark-gfm 0.29.0.gfm.13 med CMARK_OPT_DEFAULT og bare table-utvidelsen. .txt er ren tekst. HTML vises inert, bilder som alt-plassholder. Blokker beholder egne ranges; transformerte entiteter/escapes merkes Approximate. BOM tas ut før parse og legges til offsets. Tabs merkes konservativt Approximate. Inndata begrenses; overdyp nesting avvises.

InlineRun.linkId er kildeposisjon + 1 for lenkenoden. Stilfragmenter i samme
lenke deler ID; tilstøtende lenker har hver sin ID, også ved lik URL. Interpreter
leverer semantisk identitet, ikke synlige markører.

SemanticTable eier alignments og rader med kildeområder og InlineRun-celler.
Ingen biblioteknoder lekker ut. Utvidelsesregister initieres én gang før parser-
instanser opprettes; preview og PDF har hver sin parser. Maks 64 kolonner og
50 000 celler per dokument.

## 5. Plumbing

Tabellen beskriver implementerte kall. Navngitte hendelser er injiserte callbacks, ikke en global event bus.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `ParserWorker::run via IInterpreter` | `CmarkInterpreter::parse` | `src/interpreter/CmarkInterpreter.cpp` | Snapshot/options → immutable model | Error ved ressursfeil | Implemented |
| 2 | `CmarkInterpreter::parse` | `ModelBuilder::appendNode` | `src/interpreter/ModelBuilder.cpp` | AST traversal → blocks/runs | Ingen cmark-typer ut av laget | Implemented |
| 3 | `ModelBuilder::beginBlock / appendNode` | `SourceMapBuilder::record` | `src/interpreter/SourceMapBuilder.cpp` | Node → byteområde/kvalitet | Ukjent range arver blokk approximate | Implemented |
| 4 | `CmarkInterpreter::parse` | `ModelBuilder::finish` | `src/interpreter/ModelBuilder.cpp` | Builder → eid modell | Ingen lånte inputpekere | Implemented |

| 5 | `ModelBuilder::appendNode` | `ModelBuilder::appendTable` | `src/interpreter/TableModelBuilder.cpp` | GFM-noder → eide tabellrader/celler | 64 kolonner / 50 000 celler | Implemented |


P23: MathSyntax maskerer formeldelimitere med uendret bytelengde og eget koordinatgrunnlag for SourceMapBuilder. ModelBuilder produserer EmbeddedContent for bilde- og formelnoder; ingen ressurs-I/O.

| 60 | `CmarkInterpreter::parse` | `MathSyntax::MathSyntax` | `src/interpreter/MathSyntax.cpp` | Raw kilde + AST → kodebeskyttet matematikkprofil | Feil gir plassholder | Implemented |

| 99 | `ModelBuilder::appendNode` | `MermaidBlockBuilder::build` | `src/interpreter/mermaid/MermaidBlockBuilder.cpp` | Implementert diagramutvidelse | Blokkfeil og stale-data følger Mermaid-designet | Implemented |

## 6. Gjenbruk og avhengigheter

Ingen andre functionality-kontrakter konsumeres; delte datatyper følger arkitekturen.

Preview er første konsument; headless inspeksjon/eksport kan senere bruke samme port. Renderer mottar bare ferdig modell. Ingen deling av cmark callbacks med application.

## 7. Verifikasjon

Relevante akseptanse-ID-er: AT-005, AT-002, AT-011, AT-013, AT-014, AT-015, AT-019, AT-021, AT-023.

`InterpreterTest` bekrefter Unicode/BOM/ranges, gjentatt tekst, nesting, entiteter, HTML/bilder, .txt, tom fil og ressursgrense.

Evidence: [Fase P3](../../../docs/evidence/P3.md). Samlet kravdekning og eventuelle gjenstående begrensninger kontrolleres i P7; Implemented er ikke automatisk Verified.

Ny regresjonskontroll: [Native lenker og markører](../../../docs/evidence/document-links.md).

P14: TableTest, TablePreviewTest og utvidet PdfFidelityTest dekker tabellutvidelsen;
se [P14](../../../docs/evidence/P14.md).

## 8. Status, risiko og endringskonsekvenser

Implemented i P3. Oppdater kontrakter, kallkart, konsumenter og tester i samme endring.
Rene porter og tydelig rolleeierskap er obligatorisk. Eventuelle senere avvik står i fasens bevisrapport.

P23 akseptanse: AT-056, AT-057. Tester: RichPreviewTest og PreviewSelectionGuiTest.

P25 (Proposed): MermaidBlockBuilder kobles til injisert IDiagramInterpreter. Vanlige kodegjerder og math/latex beholder sin eksisterende vei. Krav: UR-039; AT-059.
Se [design](../../../docs/design/mermaid-integration.md). Eksisterende Implemented-rader
og eldre bevis gjelder baseline; ny plumbing er ikke implementert eller testet.
