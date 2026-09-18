---
id: FUNC-023
kind: Functionality
audience: System
role: Service
owner: interpreter
status: Implemented
scope: FirstRelease
requirements: UR-039, UR-041, SR-021, SR-022, SR-023
uses: none
---

# Functionality-023: Mermaid-tolkning

## 1. Hensikt og avgrensning

Avgrenset interpreter-tjeneste som konverterer støttet Mermaid til XFMDs diagrammodell. Kjenner ingen font, layout, FOX, Cairo, filsystem eller renderer.

## 2. Krav og akseptanse

UR-039/041, SR-021/022/023; AT-059/061/062/063/064. Se [design og kontrakter](../../../docs/design/mermaid-integration.md) og [krav](../../../xfmd_requirements.md).

## 3. Kontrakter og eierskap

IDiagramInterpreter::parse(DiagramSource) → DiagramParseResult. DiagramModel inneholder ordnede noder, kanter, grupper og retning. Flowchart 1 er fast adapter-/cacheprofil. SemanticBlock beholder kilde og spans for diagnostikk; layout mottar ingen kilde. C++ eier kopierte verdier; Rust-eide FFI-resultater frigjøres med tilhørende free-funksjon.

## 4. Atferd, tilstand og feil

Gjenkjenn bare eksplisitt mermaid-gjerde. Rust bruker parse_mermaid_strict, men XFMD-profilvalidering må i tillegg sikre at ingen konstruksjon er stille ignorert. Init/CSS/HTML/callbacks/ressurser er utenfor første profil. Feil dekorerer blokken med original kilde. Nøyaktige node-/edge-spans er ikke eksponert av undersøkt IR; blokkankre er Approximate.

## 5. Plumbing

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `ModelBuilder::appendNode` | `MermaidBlockBuilder::build` | `src/interpreter/mermaid/MermaidBlockBuilder.cpp` | fence info + literal + SourceRange → DiagramSource | bevar original fallback | Implemented |
| 2 | `IDiagramInterpreter virtual dispatch` | `MermaidInterpreter::parse` | `src/interpreter/mermaid/MermaidInterpreter.cpp` | DiagramSource → DiagramParseResult | grense og profil sjekkes | Implemented |
| 3 | `MermaidInterpreter::parse` | `xfmd_mermaid_parse_v1` | `src/application/composition/mermaid/src/lib.rs` | lånt UTF-8 → eid ABI-resultat | panic/feil blir status | Implemented |
| 4 | `xfmd_mermaid_parse_v1` | `parse` | `src/interpreter/mermaid/rust/src/lib.rs` | UTF-8 → profil, upstream og modell | Result/feil returneres til ABI | Implemented |
| 5 | `parse` | `inspect` | `src/interpreter/mermaid/rust/src/profile.rs` | kilde → tillatt Flowchart 1 | ikke godta uavklart syntaks | Implemented |
| 6 | `parse` | `map_graph` | `src/interpreter/mermaid/rust/src/model.rs` | upstream Graph → ren modell | kontroller enums og bevarte noder/kanter | Implemented |


| P36 | `parse` | `parse` | `src/interpreter/mermaid/rust/src/sequence.rs` | Sequence 1 → ordnede hendelser + kontroll mot bibliotekparser | ukjent syntaks/scope gir feil | Implemented |
| P36 | `Sequence::read` | `validate` | `src/contracts/diagram/rust/src/sequence.rs` | typed wire → grenser, referanser og balanserte fragmenter | ugyldige verdier avvises | Implemented |

| P38 | `parse` | `parse` | `src/interpreter/mermaid/rust/src/semantic/mod.rs` | standard Mermaid → typed State/Class/Requirement/ER-rekorder | full profilkonsum; ukjent syntaks avvises | Implemented |
| P38 | `Diagram::read` | `validate` | `src/contracts/diagram/rust/src/semantic.rs` | model wire 3 → skjema, identiteter, scope og enumerasjoner | lokal feil før layout | Implemented |

## 6. Gjenbruk og avhengigheter

Konsument: FUNC-003 via injisert port. Bibliotek og Rust-typer er private. Ingen dependency til FUNC-024. Et alternativ kan produsere samme modell uten å endre Application eller diagramlayout.

## 7. Verifikasjon

Rust-profil-/FFI-tester, DiagramLayoutTest og DiagramPreparationTest dekker brukerfixtures, retninger/former/grupper, avvist syntaks, grenser, panic og eierskap. Native/PDF-dekning og avgrensninger står i [P29](../../../docs/evidence/P29.md).

## 8. Status, risiko og endringskonsekvenser

Implementert i P26/P27. Hele kildeprofilen kontrolleres før upstream, og node-/kant-/gruppetap avvises. Kildemapping er Approximate for den komplette blokken.

Akseptanse: AT-059, AT-061, AT-062, AT-063, AT-064.

P36: [Typed dekning og Sequence 1](../../../docs/design/mermaid-coverage.md)
utvider samme porter. Sequence 1 er Implemented; verifikasjon dokumenteres separat.

P36: [Faktiske kontroller og grenser](../../../docs/evidence/P36.md).

P37 Implemented: Sequence 2 bruker samme `parse`/`graph`-innganger, utvidede
ordnede hendelser, async-piltype og forkens `sequence_events::apply`.
Se [revisjon 2](../../../docs/design/mermaid-coverage.md).

P38 Implemented: `semantic::parse` i `src/interpreter/mermaid/rust/src/semantic/`
produserer egne state/class/requirement/ER-rekorder. `Diagram::validate` i
`src/contracts/diagram/rust/src/semantic.rs` kontrollerer skjema og referanser.
[Kontraktbeslutning](../../../docs/design/mermaid-semantic-model.md).

P39 Implemented: C4/Architecture/Block utvider SemanticDiagram med egne elementer,
grenser, portretninger og grid. Parserprofiler og native mapping ligger i
`semantic/{c4,architecture,block}.rs`; felles SVG/PDF-port gjenbrukes.
Se [profil og kontrakt](../../../docs/design/mermaid-architecture-authoring.md).

P40 Implemented: `semantic/planning.rs` parser og native adapter bevarer bitfelt,
perioder, tidsavhengighet og score. `semantic_planning.rs` i contracts validerer
typed records før layout. Native packet-geometri implementeres i forken.
