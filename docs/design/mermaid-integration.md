# Mermaid i XFMD — designrevisjon 1.1 (historisk native baseline)

Gjeldende dekning: [23 familier og eksplisitte delprofiler](../../mermaid_coverage.md).
Presentasjonsbeslutning: [P31/P32 SVG og libavoid](mermaid-svg-routing.md).
Teksten nedenfor beskriver P25–P30 der den avviker fra denne beslutningen.

Status: **Implemented**, 2026-09-16. P25 var designfasen; P26–P29 implementerer
og verifiserer den. [Krav](../../xfmd_requirements.md),
[feature](../../src/blueprint/feature/Feature-010--Mermaid-Diagrams.md),
[faseplan](../../implementationPlan.md) og [testbevis](../evidence/P29.md)
beskriver omfang og faktisk verifikasjon. Ingen full Mermaid-kompatibilitet hevdes.

[Rutestudien](mermaid-routing-study.md) sammenligner brukerens forslag til
begrenset side-/portsøk med bibliotekets faktiske algoritmer. Den beskriver en
mulig fork og SVG-retning; dette er forslag, ikke endring av implementert baseline.

## 1. Beslutning og bibliotek

`mermaid-rs-renderer` brukes for parsing og graf-layout, med
`default-features = false`. Pin: `3726ccbffe0e8032361eb9668694b24f77858060`,
MIT, Rust 1.92.0. Cargo.lock låser transitive avhengigheter. Arkivets SHA256 er
`8bedf9632b455e829998e0ae9f6429b7a5e6fc8460e0a4ccf7df85b08ff94066`.
CLI, PNG-backend, Node, Chromium og JavaScript inngår ikke.

Kildegrunnlaget er upstreams [parser/API](https://github.com/1jehuang/mermaid-rs-renderer/blob/3726ccbffe0e8032361eb9668694b24f77858060/src/lib.rs),
[IR](https://github.com/1jehuang/mermaid-rs-renderer/blob/3726ccbffe0e8032361eb9668694b24f77858060/src/ir.rs)
og [layouttyper](https://github.com/1jehuang/mermaid-rs-renderer/blob/3726ccbffe0e8032361eb9668694b24f77858060/src/layout/types.rs).
Versjonsnummeret alene identifiserer ikke pinnen: tag v0.3.1 peker på en annen commit.

`render_scene` brukes ikke: den tar Mermaid-kilde, parser på nytt og gjør tekst
til konturer. XFMD bruker separate porter og tegner native former med vanlig
Pango/Cairo-tekst. Det er ingen parserkall fra renderer-craten.

## 2. Profil: XFMD Flowchart 1

Støttet:

- Eksplisitt gjerde med første info-token `mermaid`, også tilde/liste/sitat.
- `flowchart`/`graph` med LR/RL/TD/TB/BT og Unicode-etiketter.
- `A[text]`, `A(text)`, `A{text}`, `A((text))`; implisitte noder og gjentatt bruk.
- Solide, stiplete og tykke kanter med ingen, én eller to pilspisser,
  `|kantetikett|`, kjeder, parallelle kanter, self-loops og sykluser.
- `subgraph ID [label]`, nesting og lokal `direction`; vanlige `%%`-kommentarer.

Andre diagramtyper, former, init/CSS/class/style/click-direktiver, HTML,
Markdown-etiketter, ikoner og bilde-/scriptressurser er utenfor profilen.
De gir blokklokal kildefallback med forklaring. Profilkontrollen konsumerer hele
kilden før upstream og kontrollerer node-ID-er, kantendepunkter og antall grupper
etterpå. Detaljer står i `profile.rs` og de testede profiltilfellene.

Pinnen feilklassifiserer `((text))` som DoubleCircle. Adapteren normaliserer
bare etter profilvalidering; `(((text)))` avvises og blir ikke en enkel sirkel.
Rå flowchart-tekst, andre kodegjerder og `.txt` er bokstavelig tekst.

Grenser: 64 KiB/blokk, 128 noder, 512 kanter, 32 grupper, gruppedybde 8,
16 diagrammer/dokument, 8 MiB beregnet scenebudsjett og 32 MiB LRU.

## 3. Lagdeling og filansvar

Headers ligger ved C++-implementasjonen. Filene nedenfor finnes; ingen
planlagte symboler presenteres som eksisterende kode.

| Eier / kilde | Ansvar |
| --- | --- |
| `src/contracts/diagram/DiagramModel.h` | Rene node-/kant-/gruppeverdier og retning. |
| `src/contracts/diagram/DiagramScene.h` | Immutable bokser, former, kantruter, etiketter og fontidentitet. |
| `src/contracts/diagram/IDiagramInterpreter.h`, `IDiagramLayout.h` | Uavhengige porter. |
| `src/contracts/diagram/DiagramAbi.h`, `DiagramWire.h`, `rust/` | Versjonert verdiprotokoll, boundskontroll og RAII. |
| `src/interpreter/mermaid/MermaidBlockBuilder.cpp` | Gjerde, kilde, parserdelegasjon og lokal fallback. |
| `src/interpreter/mermaid/MermaidInterpreter.cpp` | C++ parserport og ABI-eierskap. |
| `src/interpreter/mermaid/rust/src/{lib,profile,model}.rs` | Profilkontroll, upstream-parser og ren modellmapping. |
| `src/renderer/diagram/DiagramTextLayout.cpp` | Målte/formede etikettlinjer gjennom ITextMetrics. |
| `src/renderer/diagram/MermaidDiagramLayout.cpp` | Ren modell/mål til Rust; validert scene fra returverdier. |
| `src/renderer/diagram/rust/src/{lib,model}.rs` | Rekonstruer Graph fra verdier, kjør målt layout, returner geometri. |
| `src/renderer/diagram/DiagramPlacement.cpp` | Uniform skalering, atomisk flowblokk, DrawRuns og kildeanker. |
| `src/application/diagrams/DiagramPreparation.cpp` | Koordinerer injisert layout, feil og kansellering. |
| `src/application/diagrams/DiagramCache.cpp` | Eksplisitt worker-lokal LRU. |
| `src/application/adapters/DiagramPainter.cpp` | Utfører sceneformene og pilene med Cairo og semantisk palett. |
| `src/application/composition/DiagramServices.cpp` | Registrerer parser og separate preview-/eksportressurser. |
| `src/application/composition/mermaid/src/lib.rs` | Én staticlib med C-eksporter, panic-grense og deallokering. |
| `cmake/Mermaid.cmake`, `tools/bootstrap_mermaid.py` | Låst bygg, kontrollert patch og Cargo-link. |

DiagramSceneBuilder fra P25-utkastet utgikk: adapteren dekoder direkte til
semantiske former/bokser og kantruter; native-adapteren utfører dem. Ingen
layoutalgoritme flyttes inn i XfmdWindow eller FOX-arbeidsflytene.

## 4. Kontrakter, enheter og eierskap

`IDiagramInterpreter::parse(DiagramSource)` gir modell eller lokal feil.
`IDiagramLayout::layout(DiagramModel, DiagramLayoutRequest, ITextMetrics&)`
gir en immutable scene. Sistnevnte mottar aldri kilde eller opaque Rust-Graph.
Bytte av adapter skjer i DiagramServices, med ny prepare/cache-instans.

C-ABI v1 har parse/layout og separate free-funksjoner. Resultatholderen har
`abi_version`, `struct_size`, `status`, lånt data-peker, størrelse og owner.
Layout-requesten inneholder modell, tidsbudsjett i millisekunder og
etikettmål. Payloaden bruker little-endian u32-tellere, UTF-8 med u32-byteantall og finite
IEEE754 f64-geometri. Modellen er samme eksplisitte verdi-format på begge sider;
ingen pointer er serialisert. C++ kopierer verdier og frigjør Rust-resultatet
med ResultOwner også ved exception. Resultat-eierskap må ikke kopieres.

Status 0 er verdi, 1 er profil/layoutfeil, 4 er ugyldig ABI/input og 7 er fanget
uventet Rust-panic. Layout-deadline er en forklarende status-1-feil. OOM/abort er
ikke en vanlig fangbar Rust-panic og gir ingen påstått prosessisolasjon.

Tekst måles ved 12 pt gjennom vanlig ITextMetrics. Rust får logical pixels
(16 px font), og adapteren konverterer geometri med 0,75 én gang tilbake til pt.
Etikettlinjer beholder ShapedText/glyphs; DrawRun.textScale anvendes likt ved
tegning, merking og treff. Nodeformer og kanter forblir vektorer.

## 5. Plumbing

1. `DiagramServices::interpreter` injiserer MermaidInterpreter i CmarkInterpreter.
2. `ModelBuilder::appendNode` → MermaidBlockBuilder → IDiagramInterpreter →
   C-ABI → Rust `profile::inspect`/`model::map_graph` → DiagramModel.
3. `ParserWorker::run` kaller prepare med et ticket-basert cancellation-kall.
   DiagramServices oppretter WorkerResources ved første kall på workertråden:
   egen SharedTextMetrics, MermaidDiagramLayout og DiagramPreparation/Cache.
4. EmbeddedVisuals forbereder eksisterende bilde-/matematikkinnhold;
   DiagramPreparation slår opp cache, måler tekst og kaller IDiagramLayout.
5. Rust rekonstruerer Graph og bruker måleseamen i compute_layout. Adapteren
   validerer geometri, dimensjoner, node-/kant-/gruppeantall og budsjett.
6. MarkdownRenderer/BlockLayout → DiagramPlacement → normal RenderFrame.
   Scenen er én atomisk flowblokk; A4 skalerer også mot tilgjengelig sidehøyde.
7. DisplayListPainter bruker DiagramPainter for former og vanlig teksttegning
   for etiketter. PDF bruker den samme displaylisten med utskriftspaletten.
8. ExportPipeline bruker samme prepare-kjede med eksportjobbens eget fontsett
   og cancellation-kall. En gyldig eksisterende A4-frame kan fortsatt gjenbrukes.

Lesetekst bindes før visuell sortering: grupper i deklarasjonsrekkefølge,
noder i første deklarasjonsrekkefølge, deretter kantetiketter. Hver etikettlinje
avsluttes med linjeskift. Kildemapping for diagram og syntetisk diagnose er
Approximate mot gjerdet; det konstrueres ikke fiktive presise node-offsets.

## 6. Cache, feil og kansellering

Cacheidentiteten er full serialisert modell + Flowchart1/layout1/font12 +
FontSetId. Full verdilikhet unngår hashkollisjon. Backend/config er fast for
prepare-instansens levetid. Ingen SourceRange, DocumentToken eller palett ligger
i scenen. Bredde/A4 endrer placement, ikke Rust-layout. Feilscener caches ikke.

ParserWorker har én aktiv og én ventende jobb. Ticket forkaster også eldre
refresh av samme DocumentToken. Cancellation sjekker ticket/stopping under
mutex før/etter diagramlayout og mellom blokker. Scene-fonten valideres ved
placement; eksisterende FrameKey kontrollerer token, font og layoutgenerasjon.

P26 fant en 128/512-graf som overskred 30 sekunder. Den låste patchen legger
trådlokal deadline på to sekunder med kooperative checkpoints i layout-løkker,
labelplassering, A*-kø og rutekandidater. P29-CI avdekket en lengre vei mellom
kontrollpunktene; patchen dekker derfor også indre løkker i rangering,
flowchart-planlegging, kantpipeline, ruting og etterbehandling.
P30 beholder kontrollpunktene, men avleser klokken ved første og deretter hvert
64. kontrollpunkt i hvert layoutkall. Dette reduserer fristoverhead i små
geometrioperasjoner uten å endre algoritme, geometri eller tidsbudsjett.
Unwinding gjenoppretter måletabellen
og deadline via RAII. Dette er ikke hard preemption eller hard realtime.
Vanlige brukerdiagrammer og 128-noders kjede lykkes; tett graf blir fallback.
AddressSanitizer-bygg bruker et eksplisitt ti-sekunders instrumenteringsbudsjett
fra DiagramLimits. Produksjonsbygget beholder to sekunder. Dette kompenserer
for instrumentering uten å deaktivere tidsavbrudd eller minnekontroller.
Budsjettet velges av C++-bygget, aldri av Mermaid-kilden; Rust avviser verdier
utenfor 1–10000 ms.

## 7. Verifikasjon og begrensninger

| Akseptanse | Faktiske kontroller |
| --- | --- |
| AT-059 | Rust-profiler, DiagramLayoutTest, DiagramPreparationTest, MermaidGuiTest og begge brukerfixtures. |
| AT-060 | DiagramReadingTest, MermaidGuiTest, MermaidPdfTest; farge, resize, A4 og vektor-PDF. |
| AT-061 | Native PRIMARY/clipboard/Ctrl+A, skalerte glyph-rektangler, Unicode og PDF-tekst. |
| AT-062 | Parserfri modell, injisert layout, Rust-ABI-livsløp/panic/fuzz, lag-/dependency-kontroller. |
| AT-063 | DiagramWorkerTest, cache-/font-/grensetester, deadline og ordinære eksport-/stale-regresjoner. |
| AT-064 | Cargo.lock, arkivhash, lisensinventar, Release/ASan, offline-bygg og installert smoke-test. |

Resultater og skjermbilder står i [P26](../evidence/P26.md),
[P27](../evidence/P27.md), [P28](../evidence/P28.md) og [P29](../evidence/P29.md).
Ingen testmatrise hevdes å bevise all mulig Mermaid-syntaks eller absolutt
verste kjøretid. Ny diagramtype krever ny profil, modell/plumbing og akseptanse.

## 8. Reproduserbart bygg

Se [README](../../README.md) og [lisensinventar](../../LICENSES/mermaid-dependencies.md).
Bootstrap verifiserer arkivhash og pin/patch-stempel; Cargo bruker --locked.
Offline etter bootstrap krever Cargo-cache, `.deps/` og MicroTeX-kildene fra
CMake-konfigureringen. Installasjonen inneholder lisensene og trenger ingen
Rust-verktøy eller Mermaid-prosess ved kjøring. Upstream-oppgradering innebærer
ny pin/hash, patchreview, Cargo.lock, lisensinventar og hele profil-/ABI-matrisen.
