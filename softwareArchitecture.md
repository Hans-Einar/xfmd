# Software Architecture Design: xfmd

Status: **Implemented**, revisjon 1.0, 2026-09-12. Beskriver koden i første
leveranse. Historiske veivalg og målinger finnes i [P0](docs/evidence/P0.md) og
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
som tegner rendererens display list med FXDCWindow. ITextMetrics måler samme
fontsegmenter som host tegner. Proporsjonale overskrifter/brødtekst, fet/kursiv,
monospace, bakgrunner og lenkeregioner krever ingen HTML/CSS-motor.

Fontadapteren bruker DejaVu og søker CJK-fallback per segment. Målecache er
begrenset til 4096 korte tekstnøkler. Editorens ene font kan mangle glypher;
UTF-8 bevares uavhengig av glyphdekning. Fontvalg er systemavhengig.

## 3. Kildekart og individuelle roller

Følgende filer finnes i implementasjonen. Headers
ligger ved tilhørende `.cpp`. Én hovedrolle per filpar.

| Katalog / filer | Ansvar og grense |
| --- | --- |
| `application/main.cpp`, `Application.cpp` | Oppstart, levetid og composition root; registrerer interpreter/renderer, ingen arbeidsflytlogikk. |
| `application/ui/XfmdWindow.cpp` | Bygger menyer, toolbar, status og containere; kobler targets. |
| `application/ui/EditorWidget.cpp` | FXText-hendelser, tekst/byteposisjonsadapter og brukerredigering. |
| `application/ui/SidebarWidget.cpp` | FOX-mappetre og seleksjon; delegerer åpning. |
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
| `application/adapters/FoxRenderHost.cpp` | FOX paint/resize/input, viewport og utføring av RenderFrame. |
| `application/adapters/FoxTextMetrics.cpp` | Fontcache og tekstmåling via FOX; ingen Markdown-regler. |
| `application/adapters/FoxScheduler.cpp` | Debounce/kansellering og levetid via FOX-event loop. |
| `application/io/LocalFileStore.cpp`, `InputPolicy.cpp` | Lesing, formatmetadata, kontrollert erstatningslagring og inputgrenser. |
| `interpreter/CmarkInterpreter.cpp`, `ModelBuilder.cpp`, `SourceMapBuilder.cpp` | cmark-adapter, semantikk og dokumentert kildeposisjonsstrategi. |
| `renderer/MarkdownRenderer.cpp`, `BlockLayout.cpp`, `InlineLayout.cpp`, `HitTester.cpp` | Layoutorkestrering, block/inline-algoritmer og lenketreff. |
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
resultater forkastes. FOX-måling, layout og paint skjer på GUI-tråden. Resize
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

P0 valgte cmark 0.31.1, CMARK_OPT_DEFAULT, uten utvidelser. cmarks kildeposisjoner
brukes av SourceMapBuilder; ingen global første-match-søking. Inline-transformasjoner
merkes tilnærmet. Baseline er CommonMark 0.31.1; xfmds native presentasjonsadapter
hevder ikke full visuell conformance til alle CommonMark-eksempler.

InputPolicy avviser ugyldig UTF-8, NUL, ikke-støttet filtype og filer over 8 MiB.
HTML er inert tekst, bilder alttekst, lenker bare lokale dokumentstier. Ingen
nettverksklient, shell-evaluering eller browser engine finnes i applikasjonen.
Benchmark måler faktisk FOX-måling og parse/layout på et 1 MiB-corpus; resultater
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
