# Mermaid i XFMD — designrevisjon 1.0

Status: **Proposed**, 2026-09-16. Baseline: `eebe342` (P24).
Designfasen P25 endrer dokumentasjon; den innfører ingen Rust-avhengighet eller
Mermaid-støtte i programmet. [Krav](../../xfmd_requirements.md),
[feature](../../src/blueprint/feature/Feature-010--Mermaid-Diagrams.md) og
[faseplan](../../implementationPlan.md) er normative sammen med kontraktene her.

## 1. Beslutning og undersøkt bibliotek

Valgt kandidat er `mermaid-rs-renderer`, MIT, uten Node/Chromium. Kildegjennomgangen
brukte commit `3726ccbffe0e8032361eb9668694b24f77858060` fra upstream HEAD.
Dette er en **evalueringspin**, ikke en ferdig godkjent produksjonsavhengighet.
Taggen v0.3.1 peker på `2f993bd79a55235eb59a34d807852276ba25bea7`; Cargo-versjon
alene identifiserer derfor ikke undersøkt kode. Cargo.toml bruker edition 2024,
har CLI/PNG som default features og et valgfritt Scene-API.

Kontrollerte kilder ved pin:

- [Cargo.toml](https://github.com/1jehuang/mermaid-rs-renderer/blob/3726ccbffe0e8032361eb9668694b24f77858060/Cargo.toml): bygg, features og lisens.
- [lib.rs](https://github.com/1jehuang/mermaid-rs-renderer/blob/3726ccbffe0e8032361eb9668694b24f77858060/src/lib.rs): `parse_mermaid_strict` og `compute_layout`; strict-forløpet har preflight, men er ikke bevis for full syntaksdekning.
- [ir.rs](https://github.com/1jehuang/mermaid-rs-renderer/blob/3726ccbffe0e8032361eb9668694b24f77858060/src/ir.rs): egne Graph/Node/Edge-typer uten generelle kildespans.
- [layout/types.rs](https://github.com/1jehuang/mermaid-rs-renderer/blob/3726ccbffe0e8032361eb9668694b24f77858060/src/layout/types.rs): nodebokser, edge points, etikettbokser og ankere.
- [layout/text.rs](https://github.com/1jehuang/mermaid-rs-renderer/blob/3726ccbffe0e8032361eb9668694b24f77858060/src/layout/text.rs): intern fontmåling som ikke tar vår ITextMetrics-port.
- [scene.rs](https://github.com/1jehuang/mermaid-rs-renderer/blob/3726ccbffe0e8032361eb9668694b24f77858060/src/scene.rs): `render_scene` tar kildetekst, kjører hele SVG-forløpet og gjør tekst til konturer.

**Valg:** gjenbruk parser og graf-layout, lag XFMDs native diagramtegning fra den
rene layoutmodellen. Ikke bruk render_scene som port mellom interpreter og
renderer. Den ville koblet delene via kildetekst og mistet tekstsemantikken.
Vi bygger form-/pilprimitive og integrasjon, men ikke en ny graf-layoutalgoritme.
Upstream-kompatibilitet vurderes med fixtures; ytelsestall i README regnes ikke
som XFMD-målinger. Ingen runtime-probe er utført i denne designfasen.

## 2. Første støttede profil: XFMD Flowchart 1

Første leveranse skal dekke begge brukerdiagrammene i
[fixtures](mermaid/README.md), og:

- Eksplisitte kodegjerder med første info-token `mermaid`, også tilde-gjerder og
  gjerder i lister/sitater. Rå `flowchart`-tekst og `.txt` behandles bokstavelig.
- `flowchart`/`graph`, LR/RL/TD/TB/BT, ordnede noder og Unicode-etiketter.
- Rektangel, avrundet rektangel, rombe og sirkel; eksplisitte og implisitte noder.
- Retningsløse/enveis/toveis kanter, hel/stiplet/tykk linje, kantetiketter,
  kjeder, parallelle kanter, self-loop og sykliske grafer.
- Subgraphs med etikett, nesting og lokal direction; presedens prøves mot upstream.

Andre former, diagramtyper, HTML-/Markdown-etiketter, ikoner, bilder, init/config,
CSS/classDef/style/linkStyle og click/callback/link-konstruksjoner er utenfor
profilen. De gir forklarende kildefallback for **hele blokken**, ikke en delvis
tegning som ser korrekt ut. Kommentarer `%%` tillates, aktive direktiver gjør ikke.
Policyvalidering må følge token-/quote-kontekst; ord som «click» inne i en vanlig
etikett skal ikke avvises. Første profil bruker XFMDs lesepalett fremfor kildefarger.

Parserens annonserte diagramtyper slås ikke automatisk på. Sekvens-, klasse- og
ER-diagrammer vurderes senere med egne modellvarianter og testmatriser under
samme feature. Profilendring krever versjonert kontrakt og oppdatert blueprint.

## 3. Horisontal lagdeling og filansvar

Alle filene i denne tabellen er **Planned**. Headers ligger ved sine C++-filer.

| Eier / planlagt kilde | Ansvar |
| --- | --- |
| `src/contracts/diagram/DiagramModel.h` | Parserfri graf, profiler, kildediagnostikk og stabile lokale ID-er. |
| `src/contracts/diagram/DiagramScene.h` | Rene stier, semantiske farger og tekst-/treffdata i points. |
| `src/contracts/diagram/IDiagramInterpreter.h`, `IDiagramLayout.h` | Uavhengige porter; ingen opaque bibliotekshandle mellom dem. |
| `src/contracts/diagram/DiagramAbi.h`, `rust/` | C ABI og tilsvarende repr(C)-verdier; ingen algoritmer eller upstream-typer. |
| `src/interpreter/mermaid/MermaidBlockBuilder.cpp` | Fence/literal/source-map til injisert interpreter og blokklokal fallback. |
| `src/interpreter/mermaid/MermaidInterpreter.cpp` | C++-portadapter, RAII for parserens ABI-resultat. |
| `src/interpreter/mermaid/rust/src/{lib,profile,model}.rs` | Parserinngang, profilvalidering og upstream→XFMD mapping. |
| `src/renderer/diagram/MermaidDiagramLayout.cpp` | Layoutport, ABI-mapping og feilkontroll. |
| `src/renderer/diagram/DiagramTextLayout.cpp` | Etikettlinjer/mål gjennom ITextMetrics. |
| `src/renderer/diagram/rust/src/{lib,layout,model}.rs` | XFMD→upstream mapping og compute_layout; ingen parserkall. |
| `src/renderer/diagram/DiagramSceneBuilder.cpp` | Nodeformer, kantstier, pilspisser, semantisk tegnerekkefølge. |
| `src/renderer/diagram/DiagramPlacement.cpp` | Diagram til RenderFrame, kildemapping, leserekkefølge og sideskalering. |
| `src/application/diagrams/DiagramPreparation.cpp` | Bakgrunnsforberedelse med injisert layoutport og request-nøkkel. |
| `src/application/diagrams/DiagramCache.cpp` | Begrenset worker-lokal LRU med eksplisitt eier. |
| `src/application/adapters/DiagramPainter.cpp` | Utføre rene stier med Cairo; tekst forblir normal DisplayListPainter-tekst. |
| `src/application/composition/mermaid/` | Tynn Rust staticlib-fasade som samler eksporter; ingen parsing/layoutlogikk. |
| `cmake/Mermaid.cmake` | Cargo/CMake, offline bootstrap, pin, linker og installasjonsavhengigheter. |

Planlagt Cargo-workspace på repository-roten har én kontraktcrate, separate
interpreter-/renderer-crates og en composition-crate. De to adaptercratene
avhenger av kontrakter og samme låste upstream, aldri av hverandre. Én samlet
staticlib unngår to uavhengige Rust-runtime-/allocatorpakker i C++-linkingen.
C++ interpreter/renderer avhenger bare av egne adaptere og C ABI; Application
lenker konkrete implementasjoner i composition root. Kilder over 300 linjer
vurderes per rolle; egen fil for mapping og policy hindrer store adapterfiler.

## 4. Verdikontrakter og C++/Rust-bro

Planlagte C++-signaturer, ikke kode som finnes:

```cpp
DiagramParseResult IDiagramInterpreter::parse(const DiagramSource&);
DiagramSceneResult IDiagramLayout::layout(const DiagramModel&,
    const DiagramLayoutRequest&, ITextMetrics&);
```

DiagramSource har UTF-8-literal, absolutt SourceRange for kodeblokken og et
literal→dokument-byte-kart. cmark-strip av listeinnrykk og CRLF må håndteres.
DiagramModel har profilversjon, retning, noder i deklarasjonsorden, ordnede kanter
med egne ID-er og grupper med parent-ID. Modell inneholder labeltekst og semantikk,
aldri Mermaid-kilde som layout skal tolke, Rust Graph, cmark-node eller GUI-peker.
Oppslag etter ID gir ikke sorteringsorden. Gjentatte nodeerklæringer følger pinens
semantikk; ikke slå sammen parallelle kanter. SourceRange kan være Approximate.

DiagramScene har bounds, ordnede path-primitiver, semantisk paintrolle og etiketter
med logical ID, lesetekst, lokale bokser, shaping-data og blokkanker. Bakgrunn,
kanter og tekst tegnes i eksplisitt rekkefølge. Ugyldig topologi, ikke-finite mål
eller grenser avvises ved hver adaptergrense. Renderer kan aldri reparsere en
streng for å rekonstruere en manglende modellvariant.

ABI v1 har planlagte innganger `xfmd_mermaid_parse_v1`,
`xfmd_diagram_layout_v1`, `xfmd_mermaid_parse_free_v1` og
`xfmd_diagram_layout_free_v1`. Eksport-fasaden videresender til respektiv crate.

- C-layout/repr(C), eksplisitt abi_version/struct_size, faste enum-/heltallsbredder,
  pointer+length for lånt input og eid output. Null tillates bare ved lengde null.
- Alle inputpekere lånes kun under kallet; ingen callbacks eller beholdte C++-objekter.
  C++ kopierer output og bruker Rust free-funksjon via RAII, også ved exception.
  En privat allokeringsholder kan eie hvert ABI-resultat, men kan aldri sendes
  videre til layout som parsermodell; layout får nye lånte verdistrukturer.
- Ingen std::string, Rust Vec/String, traits eller bool-layout over grensen. Ingen
  casting mellom C++- og Rust-graf. Utvidelser valideres med ABI-versjon.
- Ved feil er output null/zero og status/diagnostikk eid av dokumentert resultatholder.
  ParseResult og LayoutResult har hver sin free-funksjon; double-free er ugyldig bruk.
- `catch_unwind` med unwind-profil ved alle Rust-eksporter; ingen panic eller C++
  exception over ABI. Statusverdier: Ok, Syntax, Unsupported, Limit, InvalidAbi,
  InvalidModel, Layout og Panic. Diagnostikk har UTF-8, byteområde/kvalitet og ingen
  native pekere. Parserdiagnostikk med bare linje/kolonne eller generisk 1:1
  mappes konservativt til blokkens Approximate-område; presisjon oppgraderes bare
  når offsetkonvensjonen faktisk er bevist. OOM/process-abort kan ikke fanges av catch_unwind og må ikke
  omtales som garantert recoverable. Fuzz/ASan/Miri brukes med sine faktiske grenser.
- Ingen Rust-parserhandle som renderer må forstå. Tredjepartsbytte kan endre begge
  private mappere, men skal ikke endre dokumentarbeidsflyt eller portene.

## 5. Tekstmåling og graftegning — teknisk gate

Biblioteket måler egne etiketter. XFMDs SharedTextMetrics/FontCatalog er
trådeide og skal **ikke deles muterbart** med parser-worker. Hver forberedelses-
og eksportworker oppretter egne metric-/fontressurser med samme fontkonfigurasjon
og verifisert FontSetId. Bare immutable glyph-/fontidentitetsverdier sendes videre.

Planlagt integrasjon gir layout en tabell med forhåndsmålte etikettlinjer,
width/height/baseline og identitet gjennom ABI. En liten pin-bundet upstream-patch
må tilby en layoutinngang som bruker denne tabellen for noder, kanter og grupper,
uten å erstatte målene gjennom intern wrapping. Adapteren skal ikke bruke standard
compute_layout dersom den igjen beregner andre mål. Seam, alle kallsider og
nødvendige wrap-varianter må bevises i P26; API-et finnes **ikke** ferdig i pinnen.
Patch isoleres, testes og foreslås upstream. Stor permanent fork er et nytt veivalg.

Layoutens nodebokser, edge points og label anchors oversettes til DiagramScene.
XFMD lager støttede formprimitive/pilspisser; geometri tester særlig pilretning,
kantendepunkter, self-loops, nodegrenser og etikettkollisjon. Tegning skal ikke
kopiere store deler av SVG-rendereren. Hvis nødvendig geometri ikke kan hentes
avgrenset, stoppes denne gaten og designet revideres før kodeintegrasjon.

## 6. Arbeidsflyt, cache og publisering

Eksisterende ParserWorker tolker allerede i bakgrunn, men PreviewCoordinator
utfører vanlig Markdown-layout på GUI-tråden. Derfor forbereder DiagramPreparation
alle dyre diagramscener i prepare-kjeden, sammen med eksisterende EmbeddedVisuals.
Vanlig MarkdownRenderer plasserer bare ferdige scener. SemanticBlock får planlagt
Diagram-kind med ren modell og valgfri, immutable presentasjonsressurs; fraværende
scene gir kildefallback, aldri skjult synkron Rust-layout under paint/resize.

ParserWorker request/completion utvides med diagramprofil, FontSetId og
forberedelsesgenerasjon. Application sender data-snapshot, ikke GUI-referanser.
PreviewCoordinator sammenligner hele identiteten før modelReady/present.
Fonter/profil utløser ny prepare; viewport, A4 og zoom skalerer samme naturlige
scene og kjører ikke Rust. Palett inngår ikke i geometrinøkkelen.

Cache-nøkkel: modellinnhold, backend-pin/patch-ID, ABI-/profilversjon,
fontidentitet og layoutparametre. Cache bruker ikke dokumenttoken eller palett som
geometri-identitet; scene-kildespans bindes til gjeldende blokk ved placement,
slik at like diagrammer på to steder ikke får samme source anchor. Token og
generasjon inngår derimot alltid i publiseringskontrollen. Hashkollisjon kontrolleres
mot kanonisk modellnøkkel. Cache er worker-lokal; immutable resultater kan deles.

Startgrenser som skal valideres i P26: 64 KiB per blokk, 128 noder, 512 kanter,
32 grupper, nesting 8, 16 diagrammer per dokument, 8 MiB per scene og 32 MiB LRU.
Byte-/syntaksnesting sjekkes før upstream-parsing med quote-bevisst profilkontroll;
node-/kant-/gruppegrenser kontrolleres på modellen før layout.
Overflow i størrelsesberegning er feil. Ingen automatisk fil-/nettressurslasting
fra kilden; appens eksisterende fontlasting er en konfigurert lokal ressurs.

Kansellering sjekkes før/etter Rust og mellom blokker. Nytt arbeid erstatter ventende
jobb; foreldede resultater forkastes. Upstream har ikke dokumentert kooperativ
cancel-port. En C++-timer kan ikke trygt avbryte en pågående Rust-tråd. P26 må måle
verste aksepterte input og shutdown; ved uakseptabel tid trengs cancellation-seam
eller separat worker-prosess, med revidert design. Ingen hard tidsgrense loves nå.

ExportPipeline får den samme injiserte prepare-kjeden, ikke en hardkodet alternativ
Mermaid-vei. Gyldig eksisterende A4-frame kan gjenbrukes; ellers bygges samme scene
fra eksportens eget snapshot/fontsett. Publisering/kansellering beholdes i
ExportCoordinator/PdfFilePublisher. Diagramfeil i PDF viser samme forklaring/kilde
som preview; ingen stille manglende figur.

## 7. Farger, sider, kopiering og mapping

Scene-paths bruker roller for tekst, kant, nodeflate og kantetikettflate. Eksisterende
ReadingPalette utvides med avledede diagramroller; slider/tema er repaint av samme
frame. Ingen innbakte CSS-/RGB-strenger bestemmer skjermpaletten. PDF bruker
utskriftspalett med samme geometri og ekte tekst; den er ikke et PNG-bilde.

Naturlig scene bruker points. Rusts logiske px konverteres én gang ved adapteren
(96 px = 72 pt); etikettmål konverteres motsatt før Rust. Layout nøkkel inneholder
enhets-/fontpolicy. DiagramPlacement skalerer uniformt til tilgjengelig bredde.
I A4 er diagrammet en udelelig blokk: flytt til neste side hvis nødvendig, og
skaler ved behov til hel innholdshøyde. Ingen kant/etikett skal klippes. Zoom og
merking bruker samme transform; store grafer kan kreve zoom for lesbarhet.

DiagramPlacement legger etiketter inn som vanlige DrawRuns/readingText, med
skalerte glyph-mål og source quality Approximate for hele kodeblokken. Leserekkefølge:
gruppeetiketter i deklarasjonsorden, nodeetiketter i første forekomsts orden,
deretter kantetiketter i kantorden; én etikett per linje, én blokkseparator.
Tom dekorasjon bidrar ikke med tekst. Parallelle kantetiketter beholdes. Ctrl+A/C,
PRIMARY og drag gjenbruker PreviewSelection; tekst velges, grafnoder redigeres ikke.
Mapping skal ikke late som en deescaped label har eksakt offset i Mermaid-kilden.
Individuelle diagramlenker inngår ikke i Flowchart 1.

## 8. Leveransegater og avklarte begrensninger

P25 leverer dette Proposed-designet. P26 må vise C++/Rust-link, fullconsumption/
profilkontroll, ren modell roundtrip, mål-seam, grafgeometri og input-budsjetter.
P27 integrerer worker/cache og diagramvisning; P28 leverer merking/farger/PDF;
P29 samler native/regresjon/ytelse og installasjonsbevis. Se konkret milestone-plan.

Cargo features starter med `default-features = false`; CLI/PNG/Scene er ikke
nødvendige for valgt produksjonsvei. Pin og patch-hash, sjekket Cargo.lock og eksakt
rust-toolchain.toml velges i P26 etter rent Linux-bygg; edition 2024 alene er ikke
MSRV-bevis. Bootstrap henter eksplisitt og verifiserer dependenciene; senere
`--locked --offline`-bygg skal fungere. Documenter transitiv lisensoversikt og
reproduserbar dependency-oppløsning, uten å love bit-identiske binærfiler.

Cargo-tester og C++-sanitizers har forskjellig dekning. Vanlig C++ ASan betyr ikke
at all Rust-kode er instrumentert. P26 fastsetter Rust FFI-/fuzz-/Miri-kontroll og
supplerende instrumentering der verktøykjeden støtter det. Lagkontrollen utvides
til Cargo-avhengigheter og rendererens forbud mot parserkall. Ingen stubber eller
ny build-option legges til før implementasjonen begynner.

P25-kontroller og avgrensninger: [designbevis](../evidence/P25.md).
