# Mermaid-dekning i XFMD

XFMD støtter **23 diagramfamilier gjennom avgrensede syntaksprofiler**.
Dette dokumentet beskriver støtten i `main`. [Eksempelgalleriet](mermaid_evicence.md)
inneholder 29 praktiske diagrammer som kan åpnes direkte i XFMD.

Alle familiene nedenfor har støtte gjennom hele kjeden: **tolkning, bevaring av
modellens betydning, layout, SVG-preview og vektor-PDF**. Støtten gjelder de
oppførte konstruksjonene, ikke hele Mermaid-språket. At rendererbiblioteket
aksepterer annen syntaks, betyr ikke at XFMD støtter den.

## Scenarier, tilstander og modeller

| Diagram / startlinje | Støttede konstruksjoner | Viktige begrensninger | Eksempel |
| --- | --- | --- | --- |
| Flowchart — `flowchart LR` / `graph TD` | LR/RL/TD/TB/BT; rektangel, avrundet rektangel, beslutning og sirkel; etiketter, grupper, kjeder, sykluser, self-loops og parallelle kanter; solide, stiplete og tykke linjer | Begrenset formutvalg; ingen vilkårlig styling eller interaktive direktiver | [Ansvar og forbindelser](docs/design/mermaid/service-map.mmd) |
| Sequence — `sequenceDiagram` | Participants/actors, alias, sync/reply/async-piler, aktivering, notater og nestede alt/else, opt, loop og par/and | Eksplisitte deltakere; balansert aktivitet ved grengrenser; ingen inline-aktivering, create/destroy, autonumber eller box | [APT-import](docs/design/mermaid/apt-import.mmd), [nestede forløp](docs/design/mermaid/sequence-nested.mmd) |
| State — `stateDiagram-v2` | Start/slutt, navngitte tilstander, etiketterte overganger, sammensatte tilstander, choice, fork/join og samtidige regioner | Ingen state-notater, lokal retning eller styling; sammensatte tilstander bruker enklere native ruting | [Aktualitet](docs/design/mermaid/measurement-state.mmd), [regioner](docs/design/mermaid/state-regions.mmd), [forgrening](docs/design/mermaid/state-choice.mmd) |
| Class — `classDiagram` | Navngitte typer, attributter, operasjoner, annotations, relasjonstekst, multipliciteter, arv, komposisjon, aggregasjon, avhengighet og realisering | Ingen namespace, generics, callbacks eller styling | [Metamodell](docs/design/mermaid/sdl-class.mmd) |
| Requirement — `requirementDiagram` | Krav-ID/tekst, kravtyper, risk/verifymethod, elementer med type/docref; contains, copies, derives, satisfies, verifies, refines og traces | Mermaid-vokabular; ingen automatisk kobling til kjørende tester eller SDL-typekontroll | [APT-krav og testbidrag](docs/design/mermaid/apt-requirements.mmd) |
| ER — `erDiagram` | Entiteter, attributter, PK/FK/UK, kommentarer, kardinaliteter og identifiserende/ikke-identifiserende relasjoner | Ingen alias, styling eller databaseutføring | [Proveniens](docs/design/mermaid/provenance-er.mmd) |

Hendelsesrekkefølge og fragmentomfang bevares i sekvensdiagrammer. Deltakere er
ikke automatisk tråder, og asynkrone piler bestemmer ikke scheduling. State-diagrammer
viser overgangene, men evaluerer ikke guards. Klasse- og ER-diagrammer beskriver
relasjoner uten å kreve en objektorientert implementasjon eller en bestemt database.
En tegnet `verifies`-relasjon er ikke bevis for bestått test. SDL/SDP-eksemplene
illustrerer kandidatbegreper; XFMD gjør dem ikke til vedtatte språkregler.

## Arkitektur

| Diagram / startlinje | Støttede konstruksjoner | Viktige begrensninger | Eksempel |
| --- | --- | --- | --- |
| C4 — `C4Context`, `C4Container`, `C4Component` | Person/System/Container/Component, Db/Queue/_Ext-varianter, grenser, relasjoner med retning, navn, teknologi og beskrivelse | Ingen Dynamic/Deployment, styles, callbacks, eksterne ressurser eller navngitte argumenter; enkel native ruting | [Kontekst](docs/design/mermaid/c4-context.mmd), [containere](docs/design/mermaid/c4-container.mmd), [komponenter](docs/design/mermaid/c4-component.mmd) |
| Architecture — `architecture-beta` | Grupper, services, junctions, L/R/T/B-tilkoblingssider og innebygde cloud/database/disk/internet/server-ikoner | Ingen nestede grupper, `{group}`-kanter, align eller eksterne ikonpakker | [Ressurser og tilkoblinger](docs/design/mermaid/architecture-resources.mmd) |
| Block — `block-beta` | Kolonner, rektangulære celler, spans, mellomrom og enkle forbindelser | 1–16 kolonner; én deklarasjon per linje; ingen nestede blocks eller spesialformer | [Horisontale lag](docs/design/mermaid/block-layers.mmd) |

C4 passer systemgrenser og ansvar. Architecture viser ressurser og grafiske
portretninger; Block viser eksplisitte lag og grid. Ingen av dem gir alene en
full semantisk modell av SDL-Containere, porter eller Channels.

## Encoding, planlegging og visualisering

| Diagram / startlinje | Støttede konstruksjoner | Viktige begrensninger | Eksempel |
| --- | --- | --- | --- |
| Packet — `packet` / `packet-beta` | Bit, start–slutt-bit, relative feltlengder og etiketter; proporsjonale feltbredder og 32-bit-rader | Maks 4096 bits / 128 felt; ingen overlapp eller init-direktiver | [Konkret wire-format](docs/design/mermaid/packet-encoding.mmd) |
| Timeline — `timeline` | Tittel, seksjoner, perioder og ordnede hendelser, også videreførte hendelseslinjer | Kategorisk tid, ikke proporsjonal kalender | [Beslutningsforløp](docs/design/mermaid/timeline-decisions.mmd) |
| Gantt — `gantt` | Tittel, seksjoner, task-ID, dato eller `after`, varighet og done/active/crit | YYYY-MM-DD, 1970–2100, 1–3650 hele dager, én tidligere avhengighet; ingen excludes eller milestones | [Dokumentasjonspilot](docs/design/mermaid/gantt-pilot.mmd) |
| Journey — `journey` | Tittel, seksjoner, ordnede oppgaver, score og aktører | Score 1–5; ingen styling | [Dokumentgjennomgang](docs/design/mermaid/journey-review.mmd) |
| Pie — `pie` / `pie showData` | Tittel, siterte kategorier og positive verdier | Maks 12 kategorier; ingen init eller styling | [Evidenskategorier](docs/design/mermaid/pie-evidence.mmd) |
| Mindmap — `mindmap` | Innrykket tre med plain/rectangle/rounded/circle-noder | To mellomrom per nivå, én rot, maks åtte nivåer; ingen icons/classes | [Gjennomgang](docs/design/mermaid/mindmap-review.mmd) |
| GitGraph — `gitGraph` | Commit med ID, branch, checkout/switch og merge; commit-foreldre bevares | LR-layout, eksplisitte commit-ID-er; ingen cherry-pick, order, tag eller type | [Forslag og review](docs/design/mermaid/gitgraph-proposal.mmd) |
| Sankey — `sankey` / `sankey-beta` | Source/target/verdi, vektede strømmer og etiketter knyttet til egne bånd | Positive verdier, asyklisk graf, enkel CSV uten siterte felt | [Fordeling av forekomster](docs/design/mermaid/sankey-provenance.mmd) |
| Quadrant — `quadrantChart` | Tittel, akseender, fire quadrant-navn og punkter | Koordinater 0–1; ingen punktstyling | [Prioriteringer](docs/design/mermaid/quadrant-priorities.mmd) |
| ZenUML — `zenuml` | Deltakere, alias og asynkrone meldinger med `A->B: tekst` | Ingen synkrone kall, creation, replies eller kontrollblokker | [Observasjoner](docs/design/mermaid/zenuml-observation.mmd) |
| Kanban — `kanban` | Kolonner og innrykkede kort med ID, tekst og kolonnetilhørighet | Ingen metadata, tickets, assignments eller styling | [Dokumentarbeid](docs/design/mermaid/kanban-review.mmd) |
| Radar — `radar-beta` | Tittel, akser/alias, kurver, min/max og graticule | 3–12 akser, komplette posisjonelle verdier; ingen named values | [Illustrerte vurderinger](docs/design/mermaid/radar-quality.mmd) |
| Treemap — `treemap-beta` | Innrykkede grupper og siterte blader med positive vekter | Maks åtte nivåer; foreldre har ingen egenverdi | [Illustrert innsats](docs/design/mermaid/treemap-effort.mmd) |
| XYChart — `xychart-beta` | Tittel, kategorisk X-akse, Y-navn/område, bar- og line-serier | Vertikal orientering, 1–12 kategorier og komplette endelige tallserier | [Sammenligning](docs/design/mermaid/xychart-evidence.mmd) |

Packet beskriver en faktisk encoding, ikke en abstrakt SDL Datagram-familie.
Planleggings- og visualiseringsdiagrammer supplerer designmodellen.

## Felles støtte og grenser

- Bruk et kodegjerde med første infotoken `mermaid`. Vanlige kodeblokker og `.txt`
  forblir tekst. Ukjent syntaks, ugyldige referanser og overskredne grenser gir
  lokal forklaring og bevart kildeblokk; resten av Markdown-dokumentet vises.
- Vanlig Unicode-tekst støttes. Profilene tillater ikke vilkårlig HTML, CSS,
  script, eksterne ressurser eller init-direktiver. Kjøring fungerer offline,
  uten Node, nettleser eller rutertjeneste.
- Lys/mørk modus og lesefarger virker i preview. Kategorifarger beholdes der de
  skiller dataserier eller grupper. Zoom og endret vindusbredde bevarer sideforhold.
- PDF bruker samme SVG med vektorgeometri og tekst. Store diagrammer skaleres ned
  til tilgjengelig sideplass og kan bli små på A4. Tekst inne i diagrammer kan
  foreløpig ikke merkes separat i preview; kilden kan kopieres fra editoren.
- Maks 64 KiB kilde per diagram, 64 diagrammer per dokument, 8 MiB per scene og
  64 MiB samlet scenesvar. Flowcharts har maks 128 noder, 512 kanter, 32 grupper
  og gruppedybde åtte. Sekvenser har maks 16 deltakere, 128 hendelser og åtte
  fragmentnivåer. Andre familiespesifikke grenser står i profilveiledningene.
- Layout har kooperativt tidsbudsjett; det er ikke en hard tidsgrense.
  Plassmangel eller rutefeil gir synlig fallback. Konkurrerende Libavoid-porter
  kan gi ulike gyldige ruter; byteidentisk SVG er ikke garantert.
- Flowcharts bruker Libavoid for ortogonal ruting, med Legacy som eksplisitt
  alternativ. Kryssinger er tillatt; valgfrie kryssingsbuer og etikettpekere er
  presentasjon. Sammensatte state-diagrammer og C4 har enklere native ruting.

## Detaljer og bruk

Profiler: [Flowchart](docs/design/mermaid-integration.md#2-profil-xfmd-flowchart-1),
[Sequence](docs/design/mermaid-sequence-authoring.md),
[State/Class/Requirement/ER](docs/design/mermaid-semantic-authoring.md),
[arkitektur](docs/design/mermaid-architecture-authoring.md),
[encoding og planlegging](docs/design/mermaid-planning-authoring.md),
[øvrige visualiseringer](docs/design/mermaid-broad-authoring.md).
Den låste bibliotekversjonen står i [avhengighetsmanifestet](cmake/mermaid-source.json).
Typer eller konstruksjoner som ikke er oppført her, regnes ikke som støttet.

Åpne hele galleriet fra build-mappen:

```sh
./xfmd ../mermaid_evicence.md
```

## BoxUI-utvidelsen

BoxUI er en separat prototypemodell, ikke en 24. standard Mermaid-familie.
XFMD kan vise BoxUI og native kontroller i Markdown; diagram-widgeten gjenbruker
de avgrensede flowchart-, sequenceDiagram- og stateDiagram-v2-profilene ovenfor.
Se [BoxUI-veiledningen](docs/design/boxui-authoring.md) og
[eksemplet](boxui_evidence.md). Øvrige Mermaid-profiler er uendret.
