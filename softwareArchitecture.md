# Software Architecture Design: xfmd

Status: **Proposed**, revisjon 0.2, 2026-09-12. Målarkitektur, ikke eksisterende
kode. Se [krav](xfmd_requirements.md), [blueprints](src/blueprint/README.md) og
[implementeringsplan](implementationPlan.md).

## 1. Intensjon og horisontale lag

FOX er applikasjonens permanente toolkit. Interpreter og renderer isoleres fordi
de skal kunne utvikles og erstattes uavhengig. Application kan bruke FOX til
vinduer, kommandoer, filsystemintegrasjon, timere og senere IPC.

```text
Presentasjon: renderer — semantisk modell → layout, tegnekommandoer, hit-testing
                     ↑ kontrakter / data ↓
Applikasjon: FOX-skall + arbeidsflyter + tilstand + adaptere
                     ↓ kontrakter / data ↑
Interpretasjon: interpreter — kildetekst → semantisk modell og kildeankre
```

Dette viser ansvar, ikke et krav om at hvert kall må traversere alle lag i samme
retning. Application orkestrerer begge tjenestene. Compile-time-regelen er
`application → contracts`, `interpreter → contracts`, `renderer → contracts`.
Bare composition root kjenner konkrete implementasjonsfabrikker. Interpreter og
renderer kjenner verken hverandre, widgets eller applikasjonskoordinatorer.

`src/contracts/` er en liten ekstra katalog for rene C++-verdier og porter, ikke
et fjerde funksjonelt lag eller generell «common»-samling. Kontrakter plasseres
der bare når de krysser laggrenser. Dette realiserer SR-001–SR-003 og SR-013.

## 2. Presentasjon versus FOX-kobling

Renderer eier typografi, block/inline-layout, tekstbryting, lenkeregioner og
layout↔kildeanker. Application eier FOX-objekter, fontressurser, scrolling,
event-mapping og tegning til vinduet. Renderer ber om fontmål gjennom
`ITextMetrics` og leverer `RenderFrame` med begrensede tegneprimitiver.
`FoxRenderHost` oversetter til FOX-tegning. Markdown-rendering bor dermed i
presentasjonslaget, mens FOX-plumbing bor i applikasjonslaget.

Editor bruker `FXText`. Preview planlegges som `FXScrollArea`-basert host med egen
tegning. Den opprinnelige antakelsen om rik typografi utelukkende med `FXText` er
ikke videreført som garanti: installert FOX 1.6.57 har `FXHiliteStyle` uten
fontfelt og én tekstfont på `FXText`. Blandede fontstørrelser, kursiv og monospace
må bevises i P0. En FXText-preview kan være forsøksadapter, men oppfyller ikke
UR-002 automatisk.

Dette designvalget utvider opprinnelig FXText-binding for å oppfylle visningskrav.
Ingen HTML/CSS-motor innføres. Host-kontrakten dekker text runs, bakgrunn/rektangel,
linje og clipping; ikke vilkårlige FOX-kall eller callbacks til arbeidsflyter.

## 3. Kildekart og individuelle roller

Følgende filer er **planlagte**; nå finnes bare katalogenes README-filer. Headers
ligger ved tilhørende `.cpp`. Én hovedrolle per filpar.

| Katalog / planlagte filer | Ansvar og grense |
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
| `interpreter/Md4cInterpreter.cpp`, `ModelBuilder.cpp`, `SourceMapBuilder.cpp` | MD4C-adapter, semantikk og dokumentert kildeposisjonsstrategi. |
| `renderer/MarkdownRenderer.cpp`, `BlockLayout.cpp`, `InlineLayout.cpp`, `HitTester.cpp` | Layoutorkestrering, block/inline-algoritmer og lenketreff. |
| `contracts/DocumentTypes.h`, `SemanticDocument.h`, `RenderFrame.h`, `IInterpreter.h`, `IRenderer.h`, `ITextMetrics.h` | Delte verdier/porter uten global tilstand. |

Rene hjelpere som HistoryStore kan bo i application uten å bruke FOX. Det er ikke
et argument for nye abstraksjonslag. Del etter ansvar/endringsårsak, ikke antall
metoder. En vindusklasse eller `DocumentController` som samler alt skal ikke
innføres. Cirka 300 linjer er et signal for vurdering, ikke mekanisk filoppdeling.

## 4. Kontrakter og data

Følgende er planlagte signaturer, ikke eksisterende API:

```cpp
ParseResult IInterpreter::parse(const SourceSnapshot&, const ParseOptions&);
LayoutResult IRenderer::layout(const SemanticDocument&, const LayoutRequest&,
                              ITextMetrics&);
HitResult IRenderer::hitTest(const RenderFrame&, Point);
TextExtent ITextMetrics::measure(TextView, FontSpec);
```

- `SourceSnapshot`: DocumentId, monotont økende Revision, eid UTF-8-tekst,
  dokumenttype og formatmetadata; ingen lånte pekere til FXText.
- `SemanticDocument`: blokker/inlines, tekst, semantiske stiler, lenkemål,
  kildeområder og dokument/revisjon; ingen MD4C-enums, fontobjekter eller pixels.
- `SourceRange`: halvt åpent `[startByte, endByte)` i snapshotets UTF-8.
  `SourceAnchor`: byteoffset, eventuell blokkandel og `Exact/Approximate/Unavailable`.
  UI-linjenumre er avledet og 1-baserte; offsets er 0-baserte.
- `RenderFrame`: dokument/revisjon, layoutgenerasjon, bredde, font/theme-key,
  eid display list, dokumenthøyde, lenkeregioner og anker↔geometrikart. Frame
  er uforanderlig og tilhører bare modellen det ble laget fra.
- `LayoutRequest`: bredde, fonttema, skalering og generasjon. Frame bruker
  dokumentkoordinater; FOX-host oversetter til viewportkoordinater én gang.
- Resultater har verdi eller typet feil; diagnoser skilles fra fatal feil.
  Parsefeil betyr ikke automatisk «ugyldig Markdown».

`DocumentSession` eier kanonisk tekst og lagret baseline. FXText er en redigerbar
projeksjon. Brukeredits sendes som bytebaserte endringer, økten bekrefter ny
revisjon, og programmatisk projeksjon undertrykker ny edit-hendelse. Undo/redo går
via samme port; dirty sammenligner innhold med baseline, ikke bare revisjonstall.
Undo tilbake til lagret tekst gjør dermed dokumentet rent igjen.

Application eier tjenesteinstanser via RAII i composition root; FOX-parenting eier
widgets. Unngå dobbelt eierskap med unique_ptr. Koble fra timere/callbacks før
økt eller host destrueres. Modeller/frames eier data; MD4C callback-minne må ikke
lekke ut som ugyldige string views.

## 5. Plumbing og transaksjoner

### Redigering og preview

`EditorWidget::onChanged` → `EditController::applyEdit` →
`DocumentSession::applyEdit` → `PreviewCoordinator::schedule` →
`FoxScheduler::restart` → `PreviewCoordinator::refresh` →
`IInterpreter::parse` → `IRenderer::layout` → `FoxRenderHost::present`.

Kun siste snapshot publiseres. Ny edit starter 300 ms-timeren på nytt. Åpning
rendrer umiddelbart; resize gjenbruker modellen og gjør bare layout. Parsefeil
beholder editor og eventuelt siste frame merket «utdatert»; scrolling og
lenkeaktivering mot gammelt frame deaktiveres. Blueprintenes kapittel 5 er det
detaljerte kallkartet; teksten her er oversikt.

### Dokumentbytte, lagring og historikk

Alle åpninger går via `DocumentCoordinator::requestOpen`, også CLI, sidepanel og
lenker. Dirty-beslutning skjer før bytte. Kandidatfil leses og valideres før økten
erstattes. Lesefeil/avbrudd beholder dokument og historikk. Etter vellykket bytte
varsles `NavigationCoordinator::commitVisit`; previewfeil etterpå viser ny kilde
med feilstatus, aldri gammelt frame som om det tilhørte ny fil. Alle vellykkede
bytter registreres likt; back/forward flytter cursor først etter suksess. Første
åpning oppretter første historikkpost.

Lagring tar snapshot, kontrollerer forventet filidentitet, skriver tempfil,
flusher og erstatter målet. Baseline oppdateres bare for faktisk lagrede bytes;
senere edits forblir dirty. Ekstern endring gir konflikt. Stat/hash-kontroll før
rename reduserer, men eliminerer ikke kappløp mot eksterne skrivere: vi lover
ikke atomisk compare-and-swap mot vilkårlige prosesser.

### Scroll og mapping

`EditorWidget::onViewportChanged` eller `FoxRenderHost::onViewportChanged` →
`ScrollCoordinator::onViewportChanged` → `AnchorMapper::map` → motsatt adapter.
Hendelsen har opprinnelse/sekvens-ID. Koordinatoren undertrykker programmatisk
ekko, clampler mål og ignorerer endringer under bytte. Resize ugyldiggjør layout;
kildeanker gjenopprettes etter ny layout.

Total prosent er ikke primær synkronisering. Mapping bruker kildeområder og
geometri; skjult syntaks knyttes til relevant synlig blokk. Tomt/ukjent område får
eksplisitt tilnærming eller deaktivert sync. Fallback er ikke bevis for presisjon.

## 6. Interpreter: avklar kildekart tidlig

MD4C er første kandidat. API-et har block/span/text-callbacks, men ikke generelle
kildeområder på alle block-callbacks. Ikke anta et ferdig AST eller perfekt
kildekart. Se [MD4C-headeren](https://github.com/mity/md4c/blob/master/src/md4c.h).

P0 skal bevise mapping av gjentatt tekst, escapes, entiteter, kode, nestede lister
og tomme blokker. Inputpekere brukes bare der dokumentert/kontrollert; genererte
fragmenter krever annen strategi. Globalt tekstsøk er ikke korrekt løsning. Ved
utilstrekkelig kvalitet velges annen interpreter eller avgrenset utvidelse med
vedlikeholdsplan. Ikke skjul feil med falske «exact»-ankre. Entitetsnormalisering
tilhører interpreter, ikke renderer/FOX-host.

## 7. Tråder, feil og ytelse

Start single-threaded med FOX-event loop. Debounce reduserer antall kjøringer,
men gjør ikke lang parsing asynkron. P0 måler parse+layout; P1 innfører inputgrense.
Hvis SR-011 ikke nås, revider pipeline eksplisitt: eventuelt worker for ren parsing
og GUI-tråd for FOX-måling/paint, med bounded queue og revisjonskontroll. Ingen
worker påstås å eksistere i dagens design.

Koordinatorer viser handlingsrettet status; interpreter/renderer returnerer feil,
ikke dialoger. Benchmark referanseinput før optimalisering.

## 8. Byttbarhet og framtidig IPC

Byttbarhet betyr kildekompatibel erstatning innen kontraktene, ikke runtime-plugin
ABI. Bare `Application.cpp` registrerer ny fabrikk. Alternative små
interpreter-/renderer-implementasjoner i kontrakttester beviser at arbeidsflyter
og FOX-host ikke må skrives om. Nye tegneprimitiver er kontraktsendring med
påvirkningsanalyse, ikke «gratis byttbarhet».

Senere IPC går til DocumentCoordinator/ScrollCoordinator via egen application-
adapter. Protokollen må ha dokument/revisjon, kildeanker, framing, størrelsesgrenser
og frakoblingsregler. Statisk `/tmp/xfmd-<user>.sock` og `SCROLL:42` er utilstrekkelig.
Foretrekk privat runtime-katalog med brukerrettigheter; avklar fallback og peer-
kontroll før implementering. `FXApp::addInput` er aktuell mekanisme, men IPC trenger
egen senere blueprint. Ingen endringer gjøres i andre repositoryer nå.

## 9. Verifikasjon og teknisk grunnlag

Headless tester dekker interpreter, renderer med deterministiske fontmål,
historikk, mapping og tilstand. Integrasjonstester dekker ekte FOX-eventflyt,
fontmåling, paint, filfeil og levetid. Fake fonttester erstatter ikke visuell QA.

Lokalt kontrollert 2026-09-12: FOX 1.6.57 i `/usr/include/fox-1.6/fxver.h`;
`FXText.h` har `FXHiliteStyle` og `getTopLine/setTopLine` med tekstposisjoner;
`FXApp.h` har timere/inputregistrering. Dette er miljøobservasjon, ikke ferdig
valgt distribusjonsbaseline. FOXs nettreferanse kunne ikke hentes. P0 verifiserer
konkrete API-er mot valgt versjon.

MD4Cs [README](https://github.com/mity/md4c) beskriver UTF-8, tolerant input og
callbacks. Encodingvalidering og semantisk modell er derfor xfmds ansvar. MD4C
ble ikke funnet via lokal pkg-config. Dependency-versjoner låses i P0; lenker
til `master` er undersøkelsesgrunnlag, ikke uforanderlig byggspesifikasjon.
