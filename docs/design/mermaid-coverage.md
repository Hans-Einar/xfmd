# Mermaid-dekning — revisjon 1, P36

Dato: 2026-09-18. Status: implementasjonsgrunnlag; testbevis publiseres separat.
Upstream HEAD kontrollert med `git ls-remote`: `3726ccbffe0e8032361eb9668694b24f77858060`.
XFMDs fork-pin: `3eb91bc78d3efee6fa8e9b746a1e99606fb2f1e7`.
Upstreams README annonserer 23 typer; dette er ikke XFMD-kompatibilitet.
Kildekontroll: forkens `src/{parser,ir,render}.rs`, `src/layout/sequence.rs`,
XFMDs `profile.rs`, DiagramModel, DiagramWire, MermaidDiagramLayout,
DiagramPreparation, DiagramPainter og ExportPipeline.

## Støttematrise

P/L/S betyr at biblioteket har parser, typeinformasjon og layout/SVG-kode;
det er kildeinspeksjon, ikke bevis for alle Mermaid-konstruksjoner.
XFMD-kolonnene beskriver baseline før P36; Sequence 1 blir første tillegg.

| Type / prioritet | Upstream og pin | XFMD parser/modell | Layout/preview/PDF | Viktigste gjenstående arbeid |
| --- | --- | --- | --- | --- |
| Flowchart | P/L/S | Flowchart 1 | Implementert, tidligere P32–35-bevis | Flere former/direktiver krever eksplisitt utvidelse |
| Sequence / 1 | P/L/S; parser slår sammen pilformer, frames bruker meldingsindekser | P36: separat Sequence 1 | P36: bibliotekets sekvenslayout/SVG | Async-piler, nested frames og full ordning av grensehendelser |
| State v2 / 2 | P/L/S, egne state-notater og pseudotilstander | Avvist / mangler typed state-modell | Ikke eksponert | Regioner, hierarchy, choice/fork/join må kontrakttestes |
| Class / 3 | P/L/S, medlemmer og dekorasjoner | Avvist / mangler | Ikke eksponert | Annotations, medlemsseksjoner, multiplicitet og relasjonstyper |
| Requirement / 4 | P/L/S, egne requirement-verdier | Avvist / mangler | Ikke eksponert | Identitet/tekst, elementer og eksakt Mermaid-relasjonstype |
| ER / 5 | P/L/S, attributter og crow-foot-dekorasjoner | Avvist / mangler | Ikke eksponert | Kardinalitet begge ender, identitet og proveniens |
| C4 / 6 | P/L/S; C4-varianter må testes hver for seg | Avvist / mangler | Ikke eksponert | Context/container/component, grenser og relasjonsetiketter |
| Architecture / 6 | P/L/S | Avvist / mangler | Ikke eksponert | Grupper, junctions, ikonpolicy og portretning |
| Block / 6 | P/L/S | Avvist / mangler | Ikke eksponert | Grid, spans, grupper og forbindelser |
| Packet / 7 | P/L/S | Avvist / mangler | Ikke eksponert | Bitposisjoner, bredder, faktisk encoding |
| Timeline, Gantt, Journey / 7 | P/L/S | Avvist / mangler | Ikke eksponert | Tid, avhengigheter, aktiviteter og skala |
| Pie, Mindmap, GitGraph, Sankey, Quadrant, ZenUML, Kanban, Radar, Treemap, XYChart / senere | P/L/S-dispatch finnes | Avvist / mangler | Ikke eksponert | Egen semantisk kontrakt og akseptanse per type |

Ingen av de ueksponerte typene blir en flowchart som nødløsning.
Nyere Mermaid-syntaks er ikke automatisk støttet av denne pinnen.

## Sekvenskontrakt og første avgrensning

En egen SequenceModel inneholder deltakere (ID, alias, actor) og ordnede,
taggete hendelser. Meldinger, notater, aktiveringer og fragmentgrenser er
ikke flowchart-noder eller -kanter. Wire har eksplisitt modellversjon og type.
Renderer bygger bibliotekets sekvensverdier fra denne kontrakten uten parsing.
SVG er presentasjonsformatet; samme scene går gjennom librsvg/Cairo til preview
og PDF. Libavoid, kryssingshopp og flowchart-etikettpekere brukes ikke her.

Sequence 1 avgrenses til eksplisitte deltakere/actors, alias, `->>`/`-->>`,
eksplisitt activate/deactivate, notater utenfor fragmenter og flate
alt/else, opt, loop og par/and. Nesting, andre piler, implicit participants,
inline +/- aktivering, box, autonumber, create/destroy og direktiver avvises
med forklaring. Det er bevisst: pinnen mister pilsemantikk, og nestede frames
har ingen eksplisitt foreldreidentitet i layout; notater på fragmentgrenser
kan få feil scope. Ingen delvis eller stille redusert rendering tillates.

Grenser: 64 KiB kilde, 16 deltakere, 128 hendelser, minst én melding;
aktivering skal være balansert. Samme 8 MiB scene, 16 blokker/dokument,
worker/cache og deadline gjelder. Sekvenslayoutens tidskontroll er kooperativ,
ikke hard preemption; bounded input begrenser arbeid mellom checkpoints.

## Videre inkrementer og semantikk

1. Sequence 2: utvid forkens parser/IR/renderer for eksakte åpne async-piler,
   og hendelsesbasert scope/layout for nesting og notater på grenser. Bevis
   nested alt/opt/loop/par før profilen åpnes. Ikke skriv en ny layoutmotor i XFMD.
2. State: måling missing/current/stale, sen observasjon og ny source-session.
   Guardtekst evalueres ikke. Bevar hierarchy/regioner i egen modell.
3. Class: Unit, Container, Functionality, Function, Dataset, Datagram, Command,
   Value; skille eierskap, bruk og realization uten å kreve OO-implementasjon.
4. Requirement: APT-krav, designansvar og testbevis. `verifies` er en tegnet
   relasjon, ikke grønn test. Mermaid-vokabular mappes eksplisitt til kandidat-SDL.
5. ER: artifact, Dataset-instans, accepted revision og Datagram occurrence;
   ingen antatt 1:1 Dataset–Datagram.
6. Første arkitekturpilot anbefales **C4 context/container**, fordi ansvar og
   systemgrenser passer behovet best. Component vurderes separat; C4 er fortsatt
   eksperimentell Mermaid-syntaks. Block passer ordnede horisontale lag bedre;
   architecture-beta er primært services/ressurser og erstatter ikke logiske
   Units/Channels. Ingen av disse lover typed ports eller SDL-validering.
7. Packet beskriver konkret encoding. Tidslinje, Gantt og journey supplerer
   planlegging; de blir ikke den normative systemmodellen.

Hver fase skal ha typed modell, strengt profilerte konstruksjoner, positive og
negative fixtures, blandet Markdown, lys/mørk/zoom/resize og lesbar PDF før
støttestatus heves. Bevar flowchart-regresjoner og offline feilgrenser.

Checkpoint #1 i SDP (2026-09-18) er eksempelgrunnlag, ikke vedtatt språk.
APT-import oppretter et validert utkast og er ikke aktivering. Deltakere er
ansvarsroller, ikke implisitte tråder; async-piler fastsetter ikke scheduling.

## Kilder

- [Mermaid sequence syntax](https://mermaid.js.org/syntax/sequenceDiagram.html)
- [Mermaid C4](https://mermaid.js.org/syntax/c4.html)
- [Mermaid architecture](https://mermaid.js.org/syntax/architecture.html)
- [Mermaid block](https://mermaid.js.org/syntax/block.html)
- [Låst upstream](https://github.com/1jehuang/mermaid-rs-renderer/tree/3726ccbffe0e8032361eb9668694b24f77858060)
