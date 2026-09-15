---
id: FUNC-023
kind: Functionality
audience: System
role: Service
owner: interpreter
status: Proposed
scope: Future
requirements: UR-039, UR-041, SR-021, SR-022, SR-023
uses: none
---

# Functionality-023: Mermaid-tolkning

## 1. Hensikt og avgrensning

Avgrenset interpreter-tjeneste som konverterer støttet Mermaid til XFMDs diagrammodell. Kjenner ingen font, layout, FOX, Cairo, filsystem eller renderer.

## 2. Krav og akseptanse

UR-039/041, SR-021/022/023; AT-059/061/062/063/064. Se [design og kontrakter](../../../docs/design/mermaid-integration.md) og [krav](../../../xfmd_requirements.md).

## 3. Kontrakter og eierskap

IDiagramInterpreter::parse(DiagramSource) → DiagramParseResult. DiagramModel inneholder ordnede noder, kanter, grupper, retning og profilversjon. Kilde ligger ved resultatet for diagnostikk, ikke som input til layout. C++ eier kopierte verdier; Rust-eide FFI-resultater frigjøres med tilhørende free-funksjon.

## 4. Atferd, tilstand og feil

Gjenkjenn bare eksplisitt mermaid-gjerde. Rust bruker parse_mermaid_strict, men XFMD-profilvalidering må i tillegg sikre at ingen konstruksjon er stille ignorert. Init/CSS/HTML/callbacks/ressurser er utenfor første profil. Feil dekorerer blokken med original kilde. Nøyaktige node-/edge-spans er ikke eksponert av undersøkt IR; blokkankre er Approximate.

## 5. Plumbing

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `ModelBuilder::appendNode` | `MermaidBlockBuilder::build` | `src/interpreter/mermaid/MermaidBlockBuilder.cpp` | fence info + literal + SourceRange → DiagramSource | bevar original fallback | Planned |
| 2 | `IDiagramInterpreter virtual dispatch` | `MermaidInterpreter::parse` | `src/interpreter/mermaid/MermaidInterpreter.cpp` | DiagramSource → DiagramParseResult | grense og profil sjekkes | Implemented |
| 3 | `MermaidInterpreter::parse` | `xfmd_mermaid_parse_v1` | `src/application/composition/mermaid/src/lib.rs` | lånt UTF-8 → eid ABI-resultat | panic/feil blir status | Implemented |
| 4 | `xfmd_mermaid_parse_v1` | `inspect` | `src/interpreter/mermaid/rust/src/profile.rs` | strict upstream-resultat → tillatt Flowchart 1 | ikke godta delvis tolket graf | Implemented |
| 5 | `inspect` | `map_graph` | `src/interpreter/mermaid/rust/src/model.rs` | upstream Graph → ren ABI-modell | avvis ukjent enum/overstørrelse | Implemented |


## 6. Gjenbruk og avhengigheter

Konsument: FUNC-003 via injisert port. Bibliotek og Rust-typer er private. Ingen dependency til FUNC-024. Et alternativ kan produsere samme modell uten å endre Application eller diagramlayout.

## 7. Verifikasjon

Planlagt MermaidInterpreterTest og Rust-profiltester: brukerfixtures, Unicode/CRLF/list-indent, gjentatte noder, parallelle kanter, diagnostikk, ukjente konstruksjoner og alloker/frigi/panic. AT-059/061/062/063/064.

## 8. Status, risiko og endringskonsekvenser

Proposed. P26 må avklare fullconsumption og span-kvalitet mot pin; mangler løses med liten dokumentert adapter/patch eller eksplisitt profilsnevring, aldri stille tap.

Planlagt akseptanse: AT-059, AT-061, AT-062, AT-063, AT-064.
