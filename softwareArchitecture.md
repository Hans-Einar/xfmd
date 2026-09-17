# Software Architecture Design: xfmd

Status: **Implemented P0–P15, revisjon 1.4**, 2026-09-13.
Kapittel 1–13 beskriver implementasjonen. Historiske veivalg og målinger finnes i [P0](docs/evidence/P0.md) og
[sluttverifikasjonen](docs/evidence/P7.md).

## 1. Horisontale lag og avhengigheter

FOX er permanent applikasjonsteknologi. Application orkestrerer to uavhengige
porter: interpreter produserer semantikk; renderer produserer presentasjon.

```text
src/interpreter/ → src/contracts/ ← src/renderer/
                         ↑
                  src/application/
             FOX-shell, arbeidsflyter, adaptere
```

Renderer eier layout, typografi, wrapping, hit-testing og visuelle kildeankre.
Application eier FOX-vinduer, tegning, fontressurser, hendelser og koordinering.
Interpreter eier Markdown-semantikk og kildeområder. Bare composition root i
Application kjenner de konkrete interpreter-/renderer-implementasjonene.
`contracts/` er små delte verdier/porter, ikke et fjerde funksjonelt lag.

## 2. Native presentasjon

Editor er FXText med monospace-font. Preview er FoxRenderHost, en FXScrollArea
som utfører DisplayListPainter på en Cairo image-overflate. SharedTextMetrics
former Pango-glypher som både skjerm og PDF bruker. Proporsjonale overskrifter/brødtekst, fet/kursiv,
monospace, bakgrunner og lenkeregioner krever ingen HTML/CSS-motor.

FontCatalog bruker DejaVu med Pango/Fontconfig-fallback. Shaping-cache er
begrenset til 8192 tekstnøkler på maks 256 bytes. Editorens ene font kan mangle glypher;
UTF-8 bevares uavhengig av glyphdekning. Fontvalg er systemavhengig.

## 3. Kildekart og individuelle roller

Følgende filer finnes i implementasjonen. Headers
ligger ved tilhørende `.cpp`. Én hovedrolle per filpar.

| Katalog / filer | Ansvar og grense |
| --- | --- |
| `application/main.cpp`, `Application.cpp` | Oppstart, levetid og composition root; registrerer interpreter/renderer, ingen arbeidsflytlogikk. |
| `application/ui/XfmdWindow.cpp` | Bygger menyer, toolbar, status og containere; kobler targets. |
| `application/ui/EditorWidget.cpp` | FXText-hendelser, tekst/byteposisjonsadapter og brukerredigering. |
| `application/ui/SidebarWidget.cpp` | Avgrenset FXTreeList, lazy barn og filtrerte treff; delegerer åpning. |
| `application/ui/ViewModeController.cpp` | Splitter, synlighet, fokus og modus. |
| `application/commands/CommandRouter.cpp` | CLI/menu/tastatur til samme operasjoner; enabled-state. |
| `application/document/DocumentSession.cpp` | Aktiv tekst, revisjon, lagret baseline, dirty og snapshots. |
| `application/document/DocumentCoordinator.cpp` | Åpne/lagre/bytte/lukke som transaksjoner; dirty-dialog og feil. |
| `application/document/EditController.cpp` | Undo/redo, søk og edits; synkroniserer editor med økten. |
| `application/preview/ParserWorker.cpp` | Én arbeidstråd, én aktiv og én ventende jobb; bare rene data. |
| `application/document/TextProjection.cpp` | UTF-8-byteprojeksjon mellom originaltekst og FOX-normalisert LF. |
| `application/ApplicationCommands.cpp` | Konkret kommandohåndtering, dialoger og UI-arbeidsflyter. |
| `application/preview/PreviewCoordinator.cpp` | Snapshot → interpreter → renderer → publisering av riktig revisjon. |
| `application/navigation/NavigationCoordinator.cpp` | Lenke/back/forward og vellykket commit til historikk. |
| `application/navigation/HistoryStore.cpp` | Cursor, poster og frem-gren; ingen I/O eller widgets. |
| `application/navigation/LinkResolver.cpp` | Lokal sti-/scheme-policy; ingen shell eller lasting. |
| `application/scroll/ScrollCoordinator.cpp` | Begge retninger, guard, gyldig mapping og gjenoppretting. |
| `application/scroll/AnchorMapper.cpp` | Kildeforankring og kontrollert tilnærming; ingen widgets. |
| `application/adapters/FoxRenderHost.cpp`, `FoxCairoCanvas.cpp` | Paint/resize/input, viewport og gjenbrukbar Cairo-bakbuffer med FOX-pixmapoverføring. |
| `application/adapters/SharedTextMetrics.cpp`, `FontCatalog.cpp`, `DisplayListPainter.cpp` | Pango-shaping, fontidentitet og felles Cairo-glyphreplay; ingen Markdown-regler. |
| `application/adapters/FoxWheelScrollBar.cpp` | Felles presis wheel-input for begge akser i sidetre, editor og preview; bevarer FOXs scrollbar-interaksjon. |
| `application/adapters/FoxScheduler.cpp` | Debounce/kansellering og levetid via FOX-event loop. |
| `application/io/LocalFileStore.cpp`, `InputPolicy.cpp` | Lesing, formatmetadata, kontrollert erstatningslagring og inputgrenser. |
| `interpreter/CmarkInterpreter.cpp`, `ModelBuilder.cpp`, `TableModelBuilder.cpp`, `SourceMapBuilder.cpp` | cmark-gfm-adapter, semantikk og dokumentert kildeposisjonsstrategi. |
| `renderer/MarkdownRenderer.cpp`, `BlockLayout.cpp`, `TableLayout.cpp`, `InlineLayout.cpp`, `HitTester.cpp`, `LinkMarker.cpp` | Layoutorkestrering, block/inline-algoritmer og lenketreff. |
| `contracts/DocumentTypes.h`, `SemanticDocument.h`, `RenderFrame.h`, `IInterpreter.h`, `IRenderer.h`, `ITextMetrics.h` | Delte verdier/porter uten global tilstand. |

Rene hjelpere som HistoryStore kan bo i application uten å bruke FOX. Det er ikke
et argument for nye abstraksjonslag. Del etter ansvar/endringsårsak, ikke antall
metoder. En vindusklasse eller `DocumentController` som samler alt skal ikke
innføres. Cirka 300 linjer er et signal for vurdering, ikke mekanisk filoppdeling.

## 4. Kontrakter, eierskap og feil

Portene er IInterpreter::parse, IRenderer::layout/hitTest og ITextMetrics::measure.
ParseResult/LayoutResult er shared_ptr til **const** SemanticDocument/RenderFrame.
Objektene eier data; ingen cmark-noder eller FOX-pekere krysser laggrensene.
Feil kastes som Error med ErrorCode og vises av application; vanlig ufullstendig
Markdown er tolerant input, ikke en lagringsblokkerende valideringsfeil.

SourceSnapshot inneholder token `{document, revision}`, eid UTF-8, path og plainText.
SourceRange er halvt åpent `[begin,end)` i originalens UTF-8-byteoffsets.
SourceAnchor har byte, fraksjon og Exact/Approximate/Unavailable. RenderFrame har
token, layoutgenerasjon, dimensjoner, DrawRuns, dekorasjoner, treff og ankergeometri.
LayoutRequest angir bredde og generasjon. Host oversetter dokumentkoordinater til
viewportkoordinater ved tegning og input.

DocumentSession eier rå tekst og lagret baseline. TextProjection normaliserer bare
FOX-projeksjonen til LF; EditController oversetter endringer tilbake og har eneste
undo-stack. Dirty sammenligner bytes med baseline, også etter undo. Application
eier tjenester med RAII; FOX-parenting eier widgets. Timere frakobles og worker
joines før avhengighetene destrueres.

## 5. Faktiske arbeidsflyter

### Redigering og preview

EditorWidget → EditController::applyProjectedText → DocumentSession::applyEdit →
PreviewCoordinator::schedule → FoxScheduler::restart → PreviewCoordinator::refresh →
ParserWorker::submit/run → IInterpreter::parse → GUI-poll → IRenderer::layout →
FoxRenderHost::present. Blueprintenes kapittel 5 spesifiserer symbolene.

300 ms debounce gjelder redigering; åpning starter straks. Én parser-worker har
én aktiv og én siste ventende jobb. Arbeid kanselleres logisk med token; gamle
resultater forkastes. Preview-shaping, layout og paint skjer på GUI-tråden. Resize
bruker gjeldende modell uten ny parsing. Foreldet frame er ikke interaktivt;
nytt dokument fjerner gammelt frame. Feil beholder redigerbar kilde og synlig status.

### Dokumenter, lagring og historikk

Application::open → NavigationCoordinator::openTarget → DocumentCoordinator::requestOpen.
Dirty-valg skjer før bytte, og kandidat leses/valideres før session erstattes.
Historikk oppdateres først etter vellykket bytte. Back/forward gjenoppretter anker;
ny reise kutter frem-grenen. HistoryStore begrenses til 100 poster. Save As
oppdaterer aktuell historikksti uten å legge til en ekstra reise.

LocalFileStore bevarer BOM/linjeslutt, eier/modus og xattrs inklusive ACL når
filsystemet tillater det. Søsken-tempfil flushes før publisering. Eksisterende
filer bruker rename; nye filer publiseres uten overskriving med link/unlink.
Identitetskontroll omfatter inode, timestamps, størrelse og innholdshash, gjentatt
før rename. Dette er ikke atomisk compare-and-swap mot andre prosesser.
Symlenker følger kanonisk mål; hardlenker avvises med Lagre som. Feil før publisering
beholder original og dirty. Feilet katalog-fsync etter publisering rapporterer
manglende bekreftelse på varighet, selv om lagringen er gjennomført.

### Scrolling og mapping

EditorWidget/FoxRenderHost viewport-callback → ScrollCoordinator → AnchorMapper →
motsatt adapter. Programmatisk oppdatering undertrykker ekko; koordinatorens guard
beskytter reentrans. Token/generasjon avviser gammel geometri. Resize og navigasjon
lagrer kildeanker som gjenopprettes når riktig frame finnes. Mapping bruker
kildeområder, ikke total scrollprosent. Entiteter/escapes/tabulatorer kan være
Approximate; kode har eksplisitte linjeområder. Fallback kalles aldri Exact.

## 6. Parser og ressursgrenser

P0 valgte cmark 0.31.1 uten utvidelser. P14 bruker cmark-gfm 0.29.0.gfm.13,
CMARK_OPT_DEFAULT med eksplisitt table-utvidelse. cmark-gfms kildeposisjoner
brukes av SourceMapBuilder; ingen global første-match-søking. Inline-transformasjoner
merkes tilnærmet. Parseren er nå GitHubs CommonMark/GFM-fork (0.29-basert); xfmds native presentasjonsadapter
hevder ikke full visuell conformance til alle CommonMark-eksempler.

InputPolicy avviser ugyldig UTF-8, NUL, ikke-støttet filtype og filer over 8 MiB.
HTML er inert tekst, bilder alttekst, lenker bare lokale dokumentstier. Ingen
nettverksklient, shell-evaluering eller browser engine finnes i applikasjonen.
Benchmark måler faktisk Pango-shaping og parse/layout på et 1 MiB-corpus; resultater
og kjente begrensninger føres separat fra kravene.

## 7. Byttbarhet og videre arbeid

Byttbarhet er kildekompatibel implementasjon av portene, ikke runtime-plugin ABI.
PortContractTest bruker alternative porter; ekte implementasjoner testes separat.
Nye tegneprimitiver eller modellfelt er kontraktsendringer med påvirkningsanalyse.
IPC, lokale bilder, fragmentlenker og dialektutvidelser krever ny kravrevisjon og
blueprint før kode. Ingen endringer er gjort i xfw/xfi eller andre repositoryer.

## 8. Verifikasjon

CTest dekker kontrakter, dokumenttransaksjoner, parser, renderer, koordinatorer
og ekte FOX under isolert Xvfb. ASan/UBSan brukes på samme tester. Struktur- og
lagkontroll er automatisert; semantisk plumbing og eierskap gjennomgås manuelt.
Se [bidragsguiden](CONTRIBUTING.md) for kommandoer, stil og videre arbeidsregler.

## 9. Lenkeaktivering og markører

FoxRenderHost kaller enable() fordi FXScrollArea ellers ikke mottar native
muse-/tastaturhendelser. NavigationGuiTest sender nå X11-knappetrykk/-slipp gjennom
FOX-dispatch; direkte onPointer-kall er ikke tilstrekkelig GUI-bevis.

LinkResolver beholder lokal path-policy: parent_path(åpent absolutt dokument) /
relativ lenkesti, deretter canonicalisering. Absolutte stier brukes direkte.
LinkMarker i renderer lager # for relative .md-stier, /# for absolutte og
Unicode ↗ for HTTP(S), formet med den vanlige tekstfonten. Ingen
Cairo-bueprimitiv eller farge-emoji brukes. InlineRun.linkId skiller nabo-lenker med samme URL, samtidig som
fet/kursiv inne i én lenke ikke gir flere markører. Syntetiske markører bevarer
original tekst og kildeoffsets; markørens source-range er tom og Approximate.

## 10. Sidepanelets FOX-meldinger

SidebarWidget er sitt eget meldingstarget. Egne selector-ID-er starter derfor
ved FXTreeList::ID_LAST (tidligere FXDirList::ID_LAST), aldri på vilkårlige små tall som overlapper arvede
kommandoer. SEL_CLICKED/ID_TREE_EVENT åpner bare faktiske filer etter event-dispatch (P15).
SEL_COMMAND fra vanlig treklikk skal ikke bli en ID_HIDE-kommando.
ViewModeController::toggleSidebar er eneste eksplisitte synlighetsendring;
mappevalg og dokumentåpning bevarer synlig/skjult tilstand.

## 11. Små gesture-deltaer og scrollgrenser

FOX 1.6 regner wheel-bevegelse i heltallspiksler per hendelse. FoxWheelScrollBar
bevarer rest i 1/120 pixel-enheter slik at små deltaer summeres, og bruker
barens aktive animasjonsmål ved ny input. Rester nullstilles ved bevegelse ut
over scrollgrensen; reversering har dermed ingen oppsamlet overskytende bevegelse.
Alt/Ctrl og dragging beholder sin egen policy. ScrollDynamics eier hastighet/
akselerasjon, mens FoxWheelScrollBar eier en 8 ms retargeterbar bevegelsestimer
og standard changed/command-varsler. Bare application kjenner FOX-detaljene.

Widget-konstruktørene erstatter begge standardbarene før create(), med samme
parent, target, selector, stil og range/page/line. FOX-parenting eier adapterne
og FOX avregistrerer timere ved destruksjon. Dette er en xfmd-lokal kompatibilitets-
rettelse, ikke en endring i systemets FOX-bibliotek eller i xfw.

## 12. Arbeidsrot, historikk og filtrert tre (P8)

WorkspacePanel i application/ui komponerer filterfelt, to typeknapper,
SidebarWidget, søkestatus og arbeidsstihistorikk i vertikal splitter. F10 styrer
hele panelet. SidebarWidget bygger eget FXTreeList med en eksplisitt rot; arvet
FXDirList kan ikke avgrense rotnavigasjonen og er erstattet. Egne hendelses-ID-er
starter ved basens ID_LAST. Begge tree-scrollbarer og begge scrollbarer i historikklisten bruker FoxWheelScrollBar.
Den lille WorkPathList-adapteren i WorkspacePanel.cpp erstatter bare standardbarene.

application/workspace/WorkPathHistory.cpp eier kanoniske stier, rotutvidelse og MRU.
FileNameFilter.cpp eier wildcard/delstreng og typekombinasjon. DirectoryScanner.cpp
eier én stoppbar worker for katalogjobber og en bounded kø med treff som GUI
henter via timer. Ingen FOX-kall fra worker. Vanlig navigasjon leser direkte barn;
aktivt filter søker rekursivt og GUI bygger bare forfedre til matchende filer.
Rot/filterbytte stopper tidligere jobb før noder erstattes. Ingen callbacks
bærer gamle nodepekere over rotbytte. Root-dobbeltklikk og kontekstmeny utsetter
rotbytte til etter FOXs event-dispatch for å unngå sletting av aktive noder.

Application::startPath velger CLI-mappe eller dokument; vanlig open/bytte flytter
ikke arbeidsroten. FOX-registry lagrer kun de 32 historikkstiene, mens defaultrot
fortsatt er home ved neste oppstart. GUI-worker og timere stoppes før widgets slettes.

## 13. Integrert utvidelse P9–P13

Se [P9](docs/evidence/P9.md) for tekniske beslutninger, [P10](docs/evidence/P10.md)
for scrolling, [P11](docs/evidence/P11.md) for sider, [P12](docs/evidence/P12.md)
for PDF og [P13](docs/evidence/P13.md) for samlet review.

| Eier | Implementerte filer / ansvar |
| --- | --- |
| application / FUNC-014 | preferences/PreferencesService, adapters/FoxPreferencesStore, ui/PreferencesDialog: versjonert draft/validering/lagring før publisering |
| application / FUNC-015 | scroll/ScrollDynamics (inkl. ScrollInput/Profile/Origin), adapters/FoxWheelScrollBar: én normalisering, bounded rate/gain og timer |
| application / FUNC-016 | adapters/FontCatalog, SharedTextMetrics, DisplayListPainter: thread-eide fonter og immutable glyphverdier |
| renderer / FUNC-017 | PageBreaker, PageComposer, PageAnchorIndex: fysisk sideflyt, keeps og ankre |
| application / FUNC-018 | export/ExportCoordinator og ExportPipeline, adapters/PdfOutput, io/PdfFilePublisher: frosset jobb, status og publisering |
| application / FUNC-019 | ui/IconResources, IconData.h og packaging: innbygde og installerte ikoner |
| application / FUNC-010 | adapters/FoxWindowMode: EWMH, faktisk WM-state og RandR-synlighetskontroll |
| contracts | LayoutProfile.h, ITextShaper.h, PageLayout.h, RenderFrame.h: points, papir, FrameKey, glypher og sider |

RenderFrame bruker points. ViewTransform eier DPI/zoom/pixelkoordinater og sidegap;
A4-resize/zoom reflower ikke dokumentet. FrameKey validerer token, profil, fontsett,
generasjon og continuous-bredde. SourceAnchor beholder UTF-8-bytebetydning.
Typed viewport-origin hindrer at sync/restore går gjennom akselerasjon.

Preview har én parser-worker og GUI-eid layout. PDF har én separat worker og eget
Pango-context/parser-instans, opprettet av ApplicationExport.cpp. Work callback
injiseres i ExportCoordinator; ExportPipeline bruker offentlige interpreter-/
renderer-porter. Ingen FOX-kall fra worker. SourceSnapshot/papir/fontsett fryses;
matchende paged frame kan gjenbrukes. Ellers bygges det med samme renderer.
PdfOutput bruker samme DisplayListPainter som skjermen.

RenderFrame eier en flat liste med immutable shapingfragmenter, og DrawRun har
indeks/lengde inn i den. Sammenhengende ord samles uten ny shaping. Sortering skjer
per visuell linje; hele dokumentet kopieres ikke til en sorteringsbuffer.
Dette reduserer allokeringer uten å endre glyphposisjoner eller kildemapping.

Eksport bruker privat søskentemp, finaliserer Cairo og fsync før atomisk
rename/link. Identitetssjekk beskytter mot oppdagede eksterne målendringer;
det er ikke atomisk compare-and-swap. Cancel serialiseres mot commit. Etter commit
rapporteres suksess. PDF er et nytt outputdokument: POSIX-modus på eksisterende mål
bevares, men eier/xattrs kopieres ikke. DocumentSession/undo/historikk endres ikke.

Ressursgrenser: 8 MiB input, 64 KiB shapingtoken, 1M glypher, 100k runs,
2000 sider og 128 MiB PDF. Cancel kontrolleres i layout, per side/stream og før
commit. cmarks ene parse-kall er ikke avbrytbart midt i kallet.

P13-review erstattet direkte Cairo Xlib-paint med FoxCairoCanvas. Cairo tegner
fortsatt de samme glyphene; FOX håndterer X11-pixmap og én samlet blit. Bakbufferen
gjenbrukes til viewporten endrer størrelse. To pixelbuffere begrenses til maks
16M piksler hver. Dette unngår Cairo-Xlib-ressurslekkasjen observert i våre
native levetidstester, uten sanitizer-suppression for denne tegnestien.


## P14 — native tabeller og inputretting

cmark-gfm erstatter cmark innenfor interpreter-adapteren, med kun table aktivert.
SemanticTable/Row/Cell er eide verdier. Renderer/TableLayout gjenbruker InlineLayout
og leverer vanlige DrawRuns/Decoration/AnchorRegion til skjerm og PDF. FOXs FXTable
brukes ikke: dens interaktive celler og separate scrolling passer ikke dokumentets
fysiske sideflyt. Pango/Cairo forblir tekst- og tegnemotor for preview/PDF, mens FOX
tegner editor og øvrig GUI. Unicode ↗ erstatter den håndtegnede nettlenkemarkøren.
FoxRenderHost eier nå hele knappeparet slik at arvet FXWindow::grab alltid balanseres
før callback; chord/drag/stale frame aktiverer aldri en lenke.


P14s sluttmåling avdekket kapasitetdobling i editorens TextProjection, uavhengig
av Markdown-rendereren. Offsetkart og projisert tekst reserverer nå kapasitet
fra inputlengden før konvertering. Det reduserer GUI-minnetoppen uten å endre
kildeoffsets eller bruke allocator-/plattformspesifikke oppryddingskall.

## P15: dokumentindeks

`application/index/DocumentIndex` trekker overskrifter og lenker ut av den aksepterte
SemanticDocument. PreviewCoordinator publiserer modellen før layout, slik at også
layoutfeil tillater navigasjon. Ingen ekstra parsing av aktiv buffer.
`ReferenceWorker` eier én tråd og kø for lazy lokale filoverskrifter; den får en
egen IInterpreter og LocalFileStore i composition root. Generasjon avviser gamle
resultater, maks 32 utestående forespørsler, eksisterende 8 MiB filgrense.
`ui/IndexPanel` komponerer to `NavigationTree`-widgets med typed actions som verdier.
`WorkspacePanel` eier FXTabBook. ApplicationIndex kobler portene og eier polltimer.
Navigering skjer etter FOXs release-dispatch og bruker NavigationCoordinator;
klikk beholder byteankre, dirty og historikk. Ingen renderer-avhengig trelogikk.

Utvidelse og aktivering utsettes til egne FOX-timere etter event-dispatch. FOXs
makeItemVisible kan også kalle expandTree; bare en reell lukket→åpen-overgang
bestiller lesing, og ingen barn slettes mens dette kallet bruker nodepekere.
Sti inngår i UI-invalidering fordi Lagre som kan endre lenkebasen uten ny revisjon.
`adapters/ExternalBrowser` tillater HTTP(S), starter xdg-open med argv og reaper
barneprosessen fra egen polltimer. Ingen shell, rendering eller automatisk nettlast.

## P16: valgt ekstern nettleser

PreferencesSnapshot får browserProgram; FoxPreferencesStore bruker Programs/browser
uten å endre versjon 1 eller overskrive ukjente nøkler. PreferencesDialog får
programfelt med forslag og filvalg. Application::openBrowser er felles inngang
fra preview og referansetre og leser aktiv preferanse ved hvert klikk.
ExternalBrowser får executable og URL separat. Standard er xdg-open, og ingen
browseravhengighet trekkes inn i interpreter/renderer.


## P17–P19: implementert FOX UI/UX-lag

Application eier nå UiContext med ThemeProfiles, IconCatalog og sentral normal
UI-font. UiFactory lager UiButton med Flat/Classic-painter; UiRow, UiForm,
PanelHeader og DialogActions samler layoutmål. Workspace og toolbar bruker
samme CommandRouter. Preferences har fire faner og ett draft, med separat
levende Appearance-preview og rollback. Light/Dark-knappen lagrer bare temavalget.

[Produksjonsdesign og filkart](docs/design/fox-ui-layer.md) beskriver konkrete
klasser, eierskap og avgrensning. [Testbevis](docs/evidence/P17-P19.md) skiller
native UI-verifikasjon fra designprototyper. Interpreter/renderer-kontraktene,
dokumentmodell og PDF-tegning er uendret.


## P20: ekstern filaktivering og panegeometri

ApplicationTree.cpp ruter Files-treets eksplisitte aktivering via InputPolicy:
.md/.txt til eksisterende dokumentåpning, øvrige regulære filer til
DesktopFileOpener. Adapteren eier begrensede, ikke-blokkerende xdg-open-jobber,
argv og exitstatus; Application eier feilmelding og polling via FoxScheduler.
HTTP(S)-åpning beholder ExternalBrowser og eget programvalg. Renderer og parser
får ingen ny avhengighet. Ingen ekstern åpning fra automatisk trebygging.

TreeActivation.h deler tastaturpolicy mellom SidebarWidget og NavigationTree;
klasse-spesifikk leaf-vurdering bevarer mapper og lazy grener. ViewModeController
eier tidligere paneandel og restaurerer begge bredder ved overgang tilbake til
Split. IconCatalog eier Editor/Split/Preview-symbolene.

## P21: lesepalett uten reflow

application/preferences/ReadingColors eier profilverdier, RGB-konvertering,
kontrast og skjermpalett. PreferencesSnapshot har separate lightReading/darkReading.
FoxPreferencesStore laster/lagrer disse additivt. PreviewColorControls i ui/controls
har fire sliders i to kolonner og gjenbruker UiRow/UiContext for layout og stil.
ApplicationAppearance kobler live repaint, commit og rollback til eksisterende service.

FoxRenderHost sender en ReadingPalette til DisplayListPainter. Renderer gir
Decoration en ren semantisk rolle (kode, tabellflate, kant) som PageComposer bevarer.
Paletten endrer ingen FrameKey, glypher, treffregioner eller kildeankre. PDF utelater
skjermpaletten og bruker opprinnelige printfarger. Theme-preview/Cancel velger riktig
lagret lesepalett uten å skrive noen sliderverdier.

## P22: felles kompakt arbeidsflate

CompactToolbar eier responsiv gruppering i application/ui/controls. XfmdWindow
bygger én felles kontrollflate. EditorPresentation.cpp eier EditorWidget-metoder
for lesepalett og papirbredde/zoom under FUNC-011; tekstmutasjoner forblir i EditorWidget.cpp.
ApplicationAppearance koordinerer begge flater og hover-status gjennom LinkResolver.
Renderer og interpreter kjenner ikke topplinjen eller editorens visning.

P22 filstørrelsesreview: FoxRenderHost.cpp er 308 linjer. Hover legger bare til
input-varsling ved eksisterende hit-testing/invalidering. Den beholdes samlet
med hostens øvrige input og frame-livsløp; ny topplinje og editorpresentasjon er
skilt ut i egne filer. Ingen parsing, lagring eller arbeidsflyt flyttes inn i hosten.

## P23 — tekstmerking og innebygde visuelle ressurser

Den tidligere bildeplassholder-policyen erstattes av UR-037; nettverk/HTML er
fortsatt utenfor renderer. `application/preview/PreviewSelection` eier merking av
logisk lesetekst. `application/adapters/FoxPreviewInput` eier input og clipboard.
`application/media/EmbeddedVisuals` koordinerer `ImageDecoder` og `MathTypesetter`
i worker/PDF-tråden. Rene ressursreferanser i semantikken får immutable visuelle
ressurser før layout; renderer kjenner bare mål og eierskap. FOX-/Cairo-adapteren
tegner dem. Fargebytte krever repaint, ikke ny parsing eller formelsats.

P23-filansvar: `MathSyntax` gjenkjenner matematikk uten I/O og bevarer kildeoffsets.
`VisualResource` og `EmbeddedContent` er rene kontrakter. `CairoVisual` eier native
ressurslevetid; `ImageDecoder` + `GifBudget` begrenser dekoding, `MathTypesetter`
eier MicroTeX-integrasjonen. `LinkResolver::resourcePath` gjenbruker den lokale
URI-policyen; `localPath` beholder .md/.txt-valideringen for dokumentnavigasjon.
`RenderFrame.maxRunHeight` avgrenser synlighets-/hit-test-søk også for høye bilder.

## P24: nylig brukte filer

`application/ui/RecentFilesPanel.cpp` eier liste, avgrenset MRU og separat
RecentFiles registry-seksjon. WorkspacePanel komponerer den under Recent folders.
Application kobler vellykkede opened/saved-hendelser til registrering og aktivering
til eksisterende open/navigasjon. Mapperot og back/forward-historikk er uavhengige.

## P25–P29: Mermaid-integrasjon

Grunnlag og native integrasjon er implementert i P26/P27. [Designrevisjonen](docs/design/mermaid-integration.md) beskriver
FTR-010 og FUNC-023/024/025, separate interpreter-/renderer-adaptere og en XFMD-eid
DiagramModel. Rust Graph eller Mermaid-kilde brukes ikke som layoutkontrakt.
Én Rust staticlib lenkes i composition root; separate adaptercrates har ingen
innbyrdes avhengighet. FOX forblir applikasjonsteknologien.

Cmark delegerer eksplisitte mermaid-gjerder via injisert IDiagramInterpreter.
Application forbereder diagramscener i worker via IDiagramLayout, med trådeide
fonter og generasjonskontroll. MarkdownRenderer plasserer ferdige scener;
DisplayListPainter/DiagramPainter tegner native Cairo og vanlig tekst i preview/PDF.
P25 endrer ikke dagens kjørevei. Filkart, ABI, måleseam, policy, cache og faser
finnes i designet; alle nye source-filer er Planned.


Mermaid-filansvar: `application/composition/DiagramServices` registrerer
parser/prepare-kjeden; `application/diagrams/DiagramPreparation` koordinerer,
`DiagramCache` eier worker-lokal LRU, og `adapters/DiagramPainter` utfører
Cairo-primitiver. `interpreter/mermaid/MermaidBlockBuilder` kjenner gjerdet,
`MermaidInterpreter` eier parser-ABI-adapteren. `renderer/diagram/DiagramTextLayout`
måler etiketter, `MermaidDiagramLayout` dekoder layout og `DiagramPlacement`
plasserer scenen i dokumentet. Rust-algoritmeadaptere ligger i respektive
interpreter/renderer-undermapper; C-eksporter ligger bare i composition root.


## Mermaid-presentasjon og ruting — revisjon P31/P32

[SVG-/rutebeslutningen](docs/design/mermaid-svg-routing.md) erstatter den tidligere
native form-/etikettmalingen. Renderer leverer bibliotekets SVG som en ren verdi;
Application eier librsvg/Cairo og palettadapter. Interpreter er uendret.
Rutemotoren og C++-bindingen til libavoid bor i bibliotekforken. Ingen rutelogikk
skal legges under XFMDs `.deps` eller vokse i FOX-adapteren. DiagramPainter er
SVG-presentasjon; DiagramPlacement er kun plassering/kildeanker.

P33: DiagramTextLayout former lange kantetiketter med ordombryting før layout.
Inputpayload 4 overfører faktiske linjer og mål; sceneformatet forblir 3.
[Tekstpolicy og framtidig begrenset omplassering](docs/design/mermaid-label-wrap.md)
beskriver ansvar og avgrensning. Libavoid plasserer fortsatt ikke noder.

P34: [Beskyttet etikettilhørighet](docs/design/mermaid-label-attachment.md)
eies av rutepipelinen i forken. Soner, hindringer og sluttvalidering deles;
FOX-/SVG-adapteren endres ikke. Eventuelt portsøk er et separat framtidig steg.
