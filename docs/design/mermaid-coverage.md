# Mermaid-dekning — revisjon 6, P41

Dato: 2026-09-18. XFMD støtter **23 familier gjennom eksplisitte delprofiler**.
[Praktisk galleri](../../mermaid_evicence.md): 29 eksempler. Ingen full
JS-Mermaid-kompatibilitet påstås. Forslagene i SDL/SDP-eksemplene er fortsatt forslag.
Upstream-base `3726ccbffe0e8032361eb9668694b24f77858060`; endelig fork-pin `c2ccccbc27dcc087268983d84f010300e868d26e` står i
[pin-manifestet](../../cmake/mermaid-source.json). [Faktiske bevis](../evidence/P41.md).

I tabellen betyr «P/L/S» kildeinspisert parser/layout/SVG i upstream; det er
ikke ende-til-ende-kompatibilitet. Alle XFMD-rader nedenfor har **streng parser,
egen modell, layout, SVG-preview og vektor-PDF** for den dokumenterte profilen.
Ingen renderer parser Mermaid-kildetekst, og bibliotekstyper krysser ikke kontrakten.

| Familie | Upstream → pin | Bevarte konstruksjoner / XFMD-modell | Layout | Preview/PDF | Viktigste profilgrenser |
| --- | --- | --- | --- | --- | --- |
| Flowchart | P/L/S → forbedret ruting | Noder, former, kanter, grupper | Native plassering + Libavoid/Legacy | SVG/vektor | Flowchart 1, ingen vilkårlige styles/directives |
| Sequence | P/L/S → ordnet event-IR | Actors/alias, sync/reply/async, activation, notes, nested alt/opt/loop/par | Sequence events | SVG/vektor | Åtte nivåer, balansert aktivitet ved grengrense |
| State v2 | P/L/S → målte etiketter/ruting | State, transition, composite, region, choice, fork/join | Native + Libavoid flate grafer | SVG/vektor | Native composite-ruting; guardtekst evalueres ikke |
| Class | P/L/S → markørrettinger | Typer, medlemmer, annotations, relasjoner, multiplicitet | Native + Libavoid | SVG/vektor | Ingen generics eller styles; ikke SDL-typekontroll |
| Requirement | P/L/S → målt routing | Krav/element-identitet, attributter, Mermaid-relasjoner | Native + Libavoid | SVG/vektor | Verifies er ikke bestått test |
| ER | P/L/S → crowfoot-retting | Entitet, attributt/nøkkel, kardinalitet, identifying | Native + Libavoid | SVG/vektor | Ingen databaseskjema-generering |
| C4 | P/L/S → samme native layout | Context/container/component, grenser, ansvar, relasjoner | C4Data | SVG/vektor | Ingen Dynamic/Deployment, styles eller eksterne ressurser |
| Architecture | P/L/S → samme native layout | Grupper, services, junctions, L/R/T/B-porter | Architecture | SVG/vektor | Ingen nested groups, group-kanter eller eksterne icons |
| Block | P/L/S → grenseklippede piler | Grid, spans, spaces, rektangler, forbindelser | Block grid | SVG/vektor | Ingen nested block eller spesialformer |
| Packet | Generisk flowchart → typed bitlayout | Start/end-bit, label, relative antall | 32-bit-rader | SVG/vektor | 4096 bits/128 felt, ingen overlapp eller init |
| Timeline | P/L/S → native | Perioder, hendelser, seksjoner, tittel | Timeline | SVG/vektor | Kategorisk tid, ikke proporsjonal kalender |
| Gantt | P/L/S → tittelplass rettet | ID, start/duration/after, section/status | Gantt | SVG/vektor | YYYY-MM-DD, hele dager, én tidligere avhengighet |
| Journey | P/L/S → native | Ordnet oppgave, score, aktører, seksjon | Journey | SVG/vektor | Score 1–5, ingen styling |
| Pie | P/L/S → native | Kategori/positiv verdi, tittel, showData | Pie | SVG/vektor | Maks 12 kategorier |
| Mindmap | P/L/S → native | Foreldretre og fire nodeformer | Mindmap | SVG/vektor | Én rot, åtte nivåer, ingen icon/class |
| GitGraph | P/L/S → native | Branch, commit-foreldre, checkout, merge | GitGraph LR | SVG/vektor | Eksplisitte commit-ID-er; ingen cherry-pick/tag/type |
| Sankey | P/L/S → målt caption-avstand | Ordnet vektet strømnett | Sankey | SVG/vektor | Positiv DAG, enkel CSV uten quotes |
| Quadrant | P/L/S → native | Akseender, quadrant-navn, punktkoordinater | Quadrant | SVG/vektor | Punkter 0–1, ingen styling |
| ZenUML | Begrenset arrow-parser → egen profil | Deltakere/alias og asynkrone meldinger | Native sequence/OpenV | SVG/vektor | Ingen sync/creation/reply/kontrollblokker |
| Kanban | P/L/S → headerklaring | Kolonner, kort og tilhørighet | Kanban | SVG/vektor | Ingen metadata eller assignments |
| Radar | P/L/S → native | Akser/alias, komplette kurver, skala/graticule | Radar | SVG/vektor | 3–12 akser, posisjonelle verdier |
| Treemap | P/L/S → native | Foreldretre og positive bladvekter | Treemap | SVG/vektor | Sitert tekst, foreldre uten egenverdi |
| XYChart | P/L/S → native | Kategorier, Y-range, bar/line-serier | XYChart | SVG/vektor | Vertikal, 1–12 kategorier, komplette serier |

Profiler: [Flowchart](mermaid-integration.md), [Sequence](mermaid-sequence-authoring.md),
[State/Class/Requirement/ER](mermaid-semantic-authoring.md),
[arkitektur](mermaid-architecture-authoring.md), [planlegging](mermaid-planning-authoring.md),
[øvrige familier](mermaid-broad-authoring.md).

Grenser: 64 KiB kilde, 64 blokker/dokument, 8 MiB/scene og 64 MiB samlede scenesvar.
Kooperative frister er ikke hard preemption. Byteidentisk Libavoid-SVG er ikke
lovet ved konkurrerende porter; feil blir eksplisitt kildefallback. Composite
state og C4 bruker bibliotekets enklere ruting. Store diagrammer skaleres ned
på A4 og kan bli små; diagramtekst er ikke separat markerbar i preview.
Diagramfarger følger previewpaletten, mens kategorifarger beholdes der de bærer mening.

Nyere JS-Mermaid-familier som ikke finnes i pinnen (for eksempel use-case,
swimlanes og event modeling) er framtidig arbeid. Neste revisjoner kan utvide
hver profil med egne akseptanseeksempler; de skal ikke åpnes ved bare å endre headerlisten.

## Historikk: revisjon 1–5

### Revisjon 1, P36

Dato: 2026-09-18. Status: Sequence 1 implementert; [faktiske testbevis](../evidence/P36.md).
Upstream HEAD kontrollert med `git ls-remote`: `3726ccbffe0e8032361eb9668694b24f77858060`.
Undersøkt baseline-pin: `3eb91bc78d3efee6fa8e9b746a1e99606fb2f1e7`.
P36-pin: `1e3d2aabfe2a48e011ce067bd8fa000d9a44e924` (actor-symbol, målte
deltakerbredder, fristkontroll, målbasert fragmentplass og eksklusiv fragment-slutt for notater).
Upstreams README annonserer 23 typer; dette er ikke XFMD-kompatibilitet.
Kildekontroll: forkens `src/{parser,ir,render}.rs`, `src/layout/sequence.rs`,
XFMDs `profile.rs`, DiagramModel, DiagramWire, MermaidDiagramLayout,
DiagramPreparation, DiagramPainter og ExportPipeline.

## Støttematrise

P/L/S betyr at biblioteket har parser, typeinformasjon og layout/SVG-kode;
det er kildeinspeksjon, ikke bevis for alle Mermaid-konstruksjoner.
XFMD-kolonnene beskriver P36. P/L/S sier ikke at IR bevarer alle typer:
state-regioner blir syntetiske subgraphs; requirement-attributter blir tekst.
Disse trenger spesielt review før en fremtidig typed XFMD-adapter.

| Type / prioritet | Upstream og pin | XFMD parser/modell | Layout/preview/PDF | Viktigste gjenstående arbeid |
| --- | --- | --- | --- | --- |
| Flowchart | P/L/S | Flowchart 1 | Implementert, tidligere P32–35-bevis | Flere former/direktiver krever eksplisitt utvidelse |
| Sequence / 1 | P/L/S; parser slår sammen pilformer, frames bruker meldingsindekser | P36: separat Sequence 1 | P36: bibliotekets sekvenslayout/SVG | Async-piler, nested frames og full ordning av grensehendelser |
| State v2 / 2 | P/L/S, egne state-notater og pseudotilstander | Avvist / mangler typed state-modell | Ikke eksponert | Regioner, hierarchy, choice/fork/join må kontrakttestes |
| Class / 3 | P/L/S, medlemmer og dekorasjoner | Avvist / mangler | Ikke eksponert | Annotations, medlemsseksjoner, multiplicitet og relasjonstyper |
| Requirement / 4 | P/L/S; attributter pakkes i nodeetiketter | Avvist / mangler | Ikke eksponert | Identitet/tekst, elementer og eksakt Mermaid-relasjonstype |
| ER / 5 | P/L/S, attributter og crow-foot-dekorasjoner | Avvist / mangler | Ikke eksponert | Kardinalitet begge ender, identitet og proveniens |
| C4 / 6 | P/L/S; C4-varianter må testes hver for seg | Avvist / mangler | Ikke eksponert | Context/container/component, grenser og relasjonsetiketter |
| Architecture / 6 | P/L/S | Avvist / mangler | Ikke eksponert | Grupper, junctions, ikonpolicy og portretning |
| Block / 6 | P/L/S | Avvist / mangler | Ikke eksponert | Grid, spans, grupper og forbindelser |
| Packet / 7 | P/L/S | Avvist / mangler | Ikke eksponert | Bitposisjoner, bredder, faktisk encoding |
| Timeline, Gantt, Journey / 7 | P/L/S | Avvist / mangler | Ikke eksponert | Tid, avhengigheter, aktiviteter og skala |
| Pie, Mindmap, GitGraph, Sankey, Quadrant, ZenUML, Kanban, Radar, Treemap, XYChart / senere | P/L/S-dispatch finnes | Avvist / mangler | Ikke eksponert | Egen semantisk kontrakt og akseptanse per type |

### Ende-til-ende-status per grense (P36)

«Nei» betyr eksplisitt avvist før layout, ikke manglende kode i avhengigheten.
«Subset» gjelder bare konstruksjonene i forfatterveiledningen; bevis lenkes i P36.

| Type | XFMD-parser | Modellbevaring | Layout | Preview | PDF |
| --- | --- | --- | --- | --- | --- |
| Flowchart 1 | Subset | Noder/kanter/grupper | Libavoid eller Legacy | SVG | Vektor-SVG/Cairo |
| Sequence 1 | Subset | Ordnet typed interaksjon | Dedikert sequence-layout | SVG | Vektor-SVG/Cairo |
| State v2 | Nei | Nei | Nei | Nei | Nei |
| Class | Nei | Nei | Nei | Nei | Nei |
| Requirement | Nei | Nei | Nei | Nei | Nei |
| ER | Nei | Nei | Nei | Nei | Nei |
| C4 context/container/component | Nei | Nei | Nei | Nei | Nei |
| Architecture / Block | Nei | Nei | Nei | Nei | Nei |
| Packet | Nei | Nei | Nei | Nei | Nei |
| Timeline / Gantt / Journey | Nei | Nei | Nei | Nei | Nei |
| Øvrige annonserte typer | Nei | Nei | Nei | Nei | Nei |

Nyere Mermaid-dokumentasjon viser også typer uten en DiagramKind i denne pinnen,
blant annet use-case og swimlanes. De ligger i «øvrig framtidig dekning» og
krever først bibliotekstøtte. Versjonskontroll skal aldri anta at JS-Mermaid og
Rust-biblioteket har samme syntaks eller samme versjonsnummer.

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
aktivering skal være balansert og ligge utenfor fragmenter. Notat rett før
fragment krever en mellomliggende melding for entydig visuell scope. Samme 8 MiB scene, 16 blokker/dokument,
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

## Konkrete funn i den undersøkte bibliotekmodellen

- `parser.rs::parse_sequence_message` returnerer bare stil og aktivering, ikke
  piltype. `parse_sequence_diagram` lager alltid arrow_end=true. Derfor er
  `->` versus `->>` ikke en trygg kompatibilitetspåstand, og `-)` gjenkjennes ikke.
- SequenceFrame har meldingsintervaller, uten foreldre-ID; note har bare
  meldingsindeks. XFMDs ordnede hendelser er rikere og skal ikke erstattes av
  disse flatere bibliotekverdiene. Sequence 1 avgrenser mappingen eksplisitt.
- State-parseren lager syntetiske `__region_*`-subgraphs for concurrent regions;
  framtidig adapter må bevare regionscope og composite-identitet eksplisitt.
- Class-parseren har multiplicitet, stereotype og medlemslinjer, men XFMDs
  gamle fire former og utypede kantetiketter kunne ikke bevart relasjonene.
- ER bruker crow-foot-dekorasjoner og attributtlinjer; gamle DiagramEdge har
  bare arrowStart/arrowEnd og mister derfor kardinalitet.
- Requirement-parseren samler attributtlinjer i nodepresentasjon og bruker
  relasjonsnavnet som kantetikett. Bibliotekrendering alene dokumenterer ikke
  typebevaring av krav-ID, testelement eller relasjonsvokabular.

Alle nye typer må derfor ha egne profiler og mappingtester, også når layouten
internt gjenbruker bibliotekets flowchart-plassering. Deling av en algoritme er
lovlig; tap av typed mening i XFMD-kontrakten er ikke det.

## Revisjon 2 — P37 Sequence 2 (implementert)

P37 bevarer ordnede Message/Note/Activate/Start/Branch/End-hendelser helt frem
til forkens layout. Egne livslinjer/sekvensrader gjenbrukes; et eksplisitt
hendelsessteg setter vertikale avstander og nested rammer før SVG, uten
flowchart-etterbehandling. Åpne `-)`/`--)`-piler skiller async fra lukkede piler.
Maks nested dybde er 8; aktiveringer må være balansert i hver alternativgren.
Kildeordning av notater og fragmentgrenser beholdes. Par beskriver mulige
samtidige aktiviteter, ikke OS-tråder eller konkret scheduling.

Pin: `e36c7374a51a00f3803e97b5e0d117f1d57c9a43`. [P37-bevis](../evidence/P37.md)
og [forfatterprofil](mermaid-sequence-authoring.md) erstatter Sequence 1-grensene ovenfor.

## Revisjon 3 — P38 semantiske diagrammer

Fork-pin `ba4865555c32747ab1536a8d0e19312f9f04d0f6`. Egen modellversjon 3
bevarer domain records for følgende profiler. Matrisen over er historisk P36.

| Type | Parser/modell | Layout | Preview | PDF | Grenser |
| --- | --- | --- | --- | --- | --- |
| State 1 | Egen state/transition/region-kontrakt | Native plassering, Libavoid flate grafer | SVG | Vektor | Composite bruker native ruting; guardtekst evalueres ikke |
| Class 1 | Typer, medlemmer, annotations, relasjoner og multipliciteter | Native plassering og markører, Libavoid | SVG | Vektor | Ingen generics eller stylingdirektiver |
| Requirement 1 | Identitet, attributter og Mermaid-relasjoner | Native layout + Libavoid | SVG | Vektor | Verifies er ikke testresultat |
| ER 1 | Attributter, nøkler, kardinaliteter og identifying | Native layout + Libavoid | SVG | Vektor | Ingen databasegenerering |

Se [profilene](mermaid-semantic-authoring.md) og [P38-bevis](../evidence/P38.md).
Byteidentisk gjentatt SVG er en kjent libavoid-begrensning for konkurrerende
porter. 64 diagramblokker og samlet 64 MiB scenesvar per dokument tillates;
8 MiB per scene og eksisterende tids-/cachegrenser gjelder fortsatt.

## Revisjon 4 — P39 arkitekturprofiler

C4Context/Container/Component, architecture-beta og block-beta har strenge
parserprofiler, egne records og native layout/SVG i preview og vektor-PDF.
Fem nye fixtures er kontrollert med lys/mørk, resize og tekstuttrekk. C4s
syntetiske globale rot finnes bare i adapteren; den er ikke en SDL-container.
[Profilgrenser](mermaid-architecture-authoring.md), [bevis](../evidence/P39.md).
Block-pilspisser krever native grenseklipping; korreksjon følger neste fork-pin.

## Revisjon 5 — P40 planlegging og encoding

Pin `79d27a15c3d68b0412fc540c50913f27dfe17bdd` gir native PacketField/geometri,
og retter block-pilspisser. Packet, Timeline, Gantt og Journey har egne records,
strenge parserprofiler, native layout, SVG-preview og vektor-PDF.
[Profil](mermaid-planning-authoring.md), [bevis](../evidence/P40.md).
