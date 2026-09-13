# XFMD: arkitektur- og designrevisjon 1.1

Status: **Proposed**, 2026-09-13. Undersøkt baseline: `0712c29`, P0–P8.
Dette dokumentet er måldesign og integrasjonsplan, ikke en rapport om ferdige
runtime-endringer. Se [krav](xfmd_requirements.md), [arkitektur](softwareArchitecture.md),
[blueprints](src/blueprint/README.md) og [milepælplan](implementationPlan.md#5-planlagt-utvidelse-p9p13).

## 1. Abstract

Dagens tredeling mellom application, interpreter og renderer er et godt grunnlag.
Vi beholder FOX og source-anchor-synkroniseringen. Den eksisterende wheel-adapteren
utvides med en egen bevegelsesmekanisme og applikasjonspreferanser. Papirvisning og
PDF krever en større endring: felles typografigrunnlag, fysisk sidegeometri og en
tegnemodell som begge utdata bruker. En separat PDF-renderer med andre fontmål vil
undergrave WYSIWYG og anbefales ikke.

De nye brukerresultatene organiseres som **FTR-006 Justerbar scrolling** og
**FTR-007 Sidevisning og trofast PDF-utgivelse**. Fullscreen utvider eksisterende
arbeidsflate. Ikon er applikasjonsidentitet, ikke en selvstendig feature.
Implementasjonen deles i P9–P13 med risikoprober før kontraktene låses.

## 2. Arkitekturrevisjon av dagens kode

| Område | Observert implementasjon | Konsekvens for utvidelsen |
| --- | --- | --- |
| Input | `FoxWheelScrollBar` erstatter standardbarene i editor, preview, sidetre og WorkPathList, begge akser | Ett reelt integrasjonspunkt finnes allerede; unngå fire nye handlers |
| Presisjon | Rest beholdes mellom små `FXEvent.code`-verdier, mål klemmes til endepunkter; FOXs wheel-timer gjenbrukes | Bevar dette som kompatibilitetskontrakt før akselerasjon legges til |
| Preview-input | `FoxRenderHost` kaller `enable()` og videresender wheel til scrollbar | Denne tidligere feilen må ikke gjeninnføres ved ny host/tegning |
| Synkronisering | `ScrollCoordinator` og `AnchorMapper` bruker source-byteankre, sekvens og guards | Bevegelsespolicy skal ikke blandes inn i ankeroversettelsen |
| Rendering | `MarkdownRenderer` lager pixelbaserte runs; `FoxTextMetrics` måler FOX-fonter og fallback | Egnet for dagens skjermflate, utilstrekkelig kontrakt for identisk PDF-paginering |
| Presentasjon | `FoxRenderHost::present` krever frame-bredde lik viewport-bredde | A4 må validere layoutprofil og bruke egen view-transform |
| Live preview | Immutable modell, bounded parser-worker og generasjoner i `PreviewCoordinator` | Behold modellen; utvid nøkkelen og hold eksportjobben separat |
| UI og lagring | `XfmdWindow`/CommandRouter bygger/delegerer; WorkPaths bruker FXRegistry | Preferences kan integreres uten nye ansvar i DocumentSession |

Evidens finnes i [wheel-verifikasjonen](docs/evidence/wheel-scrolling.md),
[FoxWheelScrollBar](src/application/adapters/FoxWheelScrollBar.cpp),
[FoxRenderHost](src/application/adapters/FoxRenderHost.cpp),
[PreviewCoordinator](src/application/preview/PreviewCoordinator.cpp) og
[ScrollCoordinator](src/application/scroll/ScrollCoordinator.cpp).

De største implementasjonsrisikoene er dobbel behandling av wheel-events,
pixel/point-forveksling, fontfallback som skifter mellom preview og PDF, og gamle
frames som blir presentert etter formatbytte. Dette løses med eksplisitte
kontrakter og få felles komponenter, ikke et nytt generelt UI-rammeverk eller en
pluginarkitektur. Dockapps-biblioteket trekkes ikke inn i FOX-applikasjonen.

## 3. Hvor scrollingen skjer

Den vanlige kjeden er touchpad → libinput i Xorg-driveren → X-events → FOX →
widgetens scrollbar. libinput gjenkjenner blant annet tofingerscrolling og leverer
aksebevegelse; widget-/toolkitlaget bestemmer hvordan dokumentet flyttes.
[libinput: scrolling](https://wayland.freedesktop.org/libinput/doc/latest/scrolling.html).
Scroll er ikke det samme som de flerfinger-sveipene vi bruker i Window Maker.
[libinput: gestures](https://wayland.freedesktop.org/libinput/doc/latest/gestures.html).

Den undersøkte lokale FOX 1.6.57-koden i `/tmp/fox-1.6.57/src/FXApp.cpp`
komprimerer core Button4/5 og lager `SEL_MOUSEWHEEL` med ±120 per tick.
`FXScrollArea` sender dette videre til scrollbar. Denne veien dokumenterer ikke
rå touchpad-distanse, fingerantall eller et pålitelig fingerløft. XFMDs smådelta-
tester viser at vår adapter bevarer brøkdeler **når den mottar dem**, ikke at
fysiske touchpad-events har denne oppløsningen gjennom FOX.

P9 skal logge input ved XFMDs egne vinduer og sammenholde fysisk touchpad,
musehjul og syntetiske events. Vi begynner med `SEL_MOUSEWHEEL`. En eventuell XI2-
adapter er et senere, betinget valg dersom prøven viser utilstrekkelig input.
Den må da være en eksklusiv inputvei med core-event-deduplisering. Vi åpner ikke
en global libinput-leser og bruker ikke evdev-grab: XFMD skal bare endre egen
scrolling. Denne planen øker derfor heller ikke hastigheten i Xfe eller andre
FOX-programmer.

## 4. Features, functionality og omorganisering

| Ønske / brukerresultat | Featureintegrasjon | Ansvarlig functionality |
| --- | --- | --- |
| Edit → Preferences, hastighet og akselerasjon i alle flater | Ny FTR-006; eksisterende FTR-004 fortsetter å eie sync | Ny FUNC-014 Preferences og FUNC-015 Scroll Motion; FUNC-010 åpner dialog |
| Wrapping mot vinduet eller A4 | FTR-001 får visningsmodus; ny FTR-007 binder papir og eksport sammen | FUNC-004 layout, FUNC-005 host, FUNC-007 preview; ny FUNC-016 typografi og FUNC-017 sidekomposisjon |
| PDF av gjeldende buffer med samme sideskift | FTR-007 | Ny FUNC-018 PDF Export, gjenbruk av dokument/parser/layout |
| Fullscreen | Arbeidsflate for eksisterende features | FUNC-010, separat liten `FoxWindowMode`-adapter |
| XFMD-ikon | Identitet på tvers av installasjon og vindu | Ny FUNC-019 Application Identity |

Feature beskriver et sammenhengende brukerresultat og ende-til-ende-akseptanse.
Functionality eier en konkret kontrakt som flere features kan bruke. Preferences
er ikke lagt inn i teksten eller undo-systemet. PDF er ikke vanlig Save As og
skal ikke markere dokumentet som lagret.

Vi trekker wheel-dynamikk ut av FUNC-005 til FUNC-015 og fontressurser/glyphtegning
til FUNC-016. FUNC-004 trekker ut målt flyt, mens FUNC-017 eier pagineringen.
FUNC-009 beholder mapping; FUNC-007 beholder previewens livssyklus. FUNC-001–003
beholder dokument-, fil- og parseransvaret. Eksport er ny konsument av snapshots
og parserporten, ikke eier av en ny dokumentøkt. FTR-005/WorkPathHistory og filter
beholder sin semantikk; historikkwidgeten bruker bare den nye felles scrollprofilen.

## 5. Scrollpolicy og Preferences

`FoxWheelScrollBar` normaliserer input én gang. En ren `ScrollDynamics` får
profil, akse, monoton tid, mål og gyldig range. Hver scrollbar/akse har eget state.
`ScrollCoordinator` mottar den faktiske viewportbevegelsen etterpå og leverer
absolutte mottakerposisjoner med `Sync`-origin. Ingen gain på synkroniserte posisjoner.

Foreslått normalisering og akselerasjon:

```text
q = FXEvent.code / 120
v = sum(abs(q)) over siste 80 ms / 0.080 s
G = acceleration ? clamp(1 + k * max(0, v/v0 - 1), 1, maxGain) : 1
dTarget = -q * baseUnit * speed * G
target = clamp(previousTarget + dTarget, min, max)
```

`baseUnit` er dagens FOX-linje-/sideavledede enhet i innholdspiksler. Åtte-ms
historikkbøtter gir bounded state; en monoton testklokke gjør kurven testbar.
Komprimerte events gir samme antall ticks, men deres opprinnelige tidsfordeling er
ukjent. Målt `v` er derfor wheel-input-rate, ikke fysisk fingerhastighet.
Vi integrerer hver ny bevegelse; vi beregner ikke tidligere totaldistanse på nytt
med siste hastighet. Det forhindrer at retardasjon trekker dokumentet tilbake.

Forslag til første defaults: speed=1.5, acceleration=false, k=0.5, v0=8 enheter/s,
maxGain=3. UI viser hastighet, akselerasjon av/på, styrke og maksimal forsterkning;
v0 forblir dokumentert intern standard i første versjon. Grenser og skjema står i
FUNC-014/015. Speed=1 og acceleration=false brukes først som regresjonsreferanse.

Retningsskifte, ny målflate, endret profil og 200 ms inaktivitet nullstiller
hastighetshistorikk/rest. Utadgående rest kastes ved endepunkter, slik at neste
motsatte bevegelse virker umiddelbart. Alt-/Ctrl-modifiserte wheel-operasjoner
beholder dagens line/page-semantikk uten personlig gain. Tastatur, thumb-drag,
Sync og Restore går direkte til posisjon.

Migreringen gjøres i to trinn: først trekkes dagens algoritme ut uten endret
oppførsel; deretter erstattes avhengigheten av FOXs interne wheel-timer med én
eid, retargeterbar bevegelsestimer. Forslag er 8 ms tick og høyst 80 ms innhenting
mot siste mål. Ingen ekstra distanse etter input; dette er ikke fingerløftbasert
inerti. Cancel/destruksjon stopper timeren; ingen konkurrerende FOX-autoscroll.
Varsler om bevegelse og avslutning skal følge widgetens eksisterende kontrakt.

Preferences åpnes fra Edit. Dialogen får et eget prøvefelt som bruker samme
scrollmotor, men isolert state. OK validerer og lagrer før aktiv profil publiseres;
Cancel forkaster draft. `PreferencesService` har én instans per Application.
`FoxPreferencesStore` bruker eksisterende registry med egne Scroll/Page-grupper,
bevarer WorkPaths og ukjente felter og skriver ikke over nyere ukjent skjema.
P9 må verifisere feilrapportering/atomisitet i registry-adapteren før denne
lagringskontrakten låses. Ingen endring av globale libinput- eller FOX-innstillinger.

## 6. Sidegeometri, typografi og kildeankre

A4 portrett er 210×297 mm, omtrent 595.276×841.890 points. Standardmarger foreslås
til 20 mm. Window wrap er fortsatt oppstartsmodus. PaperSpec er en verdi med
format, retning og marger; flere formater kan komme senere uten ny renderermotor.

Renderer bruker `LayoutUnit` i points (1/72 inch). Skjerm/DPI/zoom tilhører
`ViewTransform` i application. A4-resize og zoom endrer ikke linjeskift; Window
wrap får ny innholdsbredde og reflow. Sidegap er visningsdekor, ikke PDF-innhold.

| Kontrakt | Baseline | Planlagt endring og konsumenter |
| --- | --- | --- |
| LayoutRequest | width, generation | LayoutProfile/PaperSpec, FontSetId og FrameKey; coordinator og renderer migreres sammen |
| Text measurement | Integer TextExtent fra FXFont | Points og felles shaping-port; metrics og painter bruker samme fontsett |
| RenderFrame/DrawRun | Pixelrektangler og tekst | PageLayout med glyphposisjoner, clusters, kildemapping og fysisk geometri |
| Frame identity | DocumentToken + generation | FrameKey inkluderer profil og fontsett; continuous bredde inngår, paged viewportbredde gjør ikke |
| Source mapping | Byte-range ↔ dokument-y | Byte-range ↔ sideindeks og points; view-transform utenfor renderer |
| Viewport callback | Lokal posisjon og guard | Typed origin og FrameKey; programmatisk mottaker får aldri inputgain |

UTF-8-bytebetydningen til SourceAnchor beholdes. Treffsøk transformerer først
skjermposisjon til sidekoordinater. Sidegap bruker nærmeste dokumentkant, ved lik
avstand neste side. Formatbytte fanger source anchor før layout og restaurerer
først når riktig FrameKey er presentert. Gammelt frame kan vises som eksplisitt
utdatert under arbeid, men brukes ikke som treffkart for den nye profilen.

Den anbefalte tekniske kandidaten er **Cairo + PangoCairo/Fontconfig** som
application-adapter, mens FOX beholder alle widgets. `FontCatalog` eier stabile
fontidentiteter; `ITextShaper` returnerer backend-frie glyph-/clusterverdier.
Renderer bestemmer linjer/plassering. `DisplayListPainter` spiller samme geometri
på skjerm og PDF, med samme fontidentitet og fallback. Ingen FOX/Cairo/Pango-pekere
legges i `src/contracts`. Cairo har en PDF-surface med sideformat i points;
det beviser ikke alene vår WYSIWYG-kontrakt.
[Cairo PDF surfaces](https://cairographics.org/manual/cairo-PDF-Surfaces.html).

P9 må bevise at valgt adapter kan gjenbruke fontene, bevare valgbar Unicode-tekst,
forme ligaturer/fallback og produsere like glyphposisjoner. Hvis kandidaten ikke
kan dette, revideres FUNC-016 før Ready. Vi beholder ikke en uavhengig FXFont-
målemodell for preview og en annen for PDF.

FUNC-004 produserer målt FlowLayout med visuelle linjer. FUNC-017 fordeler linjene
på sider, forsøker å holde overskrift med minst to følgende linjer og splitter
lange avsnitt/lister/kode ved linjegrenser. Kode wraps visuelt i A4 ved gyldige
clustergrenser, uten kildeendring; Window wrap beholder horisontal kode-scroll.
For store udelelige objekter gir eksplisitt feil fremfor stille klipping.
Tomt dokument gir én side. Ingen nye Markdown-tabeller, bilder eller nettinnhold
innføres som en skjult del av PDF-arbeidet.

WYSIWYG betyr samme innhold, fontsett, linje-/sideskift og glyphplassering ved samme
profil og bufferrevisjon. Raster-antialiasing mellom skjerm og PDF-leser kan avvike.

## 7. PDF-arbeidsflyt og feilsikkerhet

File → Export PDF (foreslått Ctrl+Shift+E) fryser gjeldende SourceSnapshot,
PaperSpec og FontSetId, inkludert ulagret tekst. Eksport fra Window wrap lager
samme A4-layout som papirvisningen ville brukt. Eksisterende frame kan bare
gjenbrukes ved eksakt key-match. Ellers brukes offentlige parser-/rendererporter;
PreviewCoordinators debounce eller private jobb kjøres ikke som eksport-API.

Én eksportjobb om gangen. Edits og dokumentbytte etter start endrer ikke jobben;
status knyttes til eksportert dokument/revisjon. En parallell parser får egen
instans fra composition root; delt interpreter antas ikke trådsikker. Worker
bruker egne ikke-FOX-contexts, GUI-tråden eier widgets. PDF-jobben har cancel og
bounded shutdown; P9 fastsetter målte ressursgrenser før Ready.

Skriv til søsken-tempfil, fullfør PDF, kontroller status og flush før atomisk
publisering. Eksisterende mål erstattes først etter brukerens vanlige overwrite-
valg og kontroll av forventet målidentitet. Eksterne filraces har samme dokumenterte
begrensning som lokal lagring. Cancel før commit fjerner temp; etter commit er
jobben fullført. Feil må bevare eksisterende mål og dokumentbufferen.
Ingen `markSaved`, endring av undo eller navigasjonshistorikk.

En felles binær filpubliseringsprimitiv med vanlig Save vurderes bare når begge
reelle brukstilfellene kan bevare sine egne kontrakter. En generell eksportplugin-
modell er ikke nødvendig for én PDF-implementasjon.

## 8. Fullscreen og ikon

View → Full Screen / F11 toggler en `FoxWindowMode`-adapter. Ved inngang tas
snapshot av geometri og maksimert tilstand; splitter/sidebar og visningsmodus
beholdes. Ved utgang restaureres tilstanden, med begrensning til tilgjengelig
skjerm dersom skjermoppsettet er endret. Escape håndteres av modal dialog først,
deretter fullscreen. Fullscreen påvirker ikke lagring eller papirprofil.

FOX 1.6.57-headeren som ble undersøkt har ingen bekreftet fullscreen-metode.
P9 skal derfor verifisere Window Makers støtte og en EWMH-request/observering
før vi låser adapteren. Maksimering alene regnes ikke som fullscreen. Ingen
redigering av brukerens WMState eller globale shortcuts.

[Ikonforslaget](docs/design/xfmd-icon-proposal.svg) bruker et brettet dokumentark,
et geometrisk M og korte renderlinjer. Turkis/blå aksent skiller kilde og
presentasjon uten tekstetikett. SVG er master; små varianter må kontrolleres i
16–64 px, med en forenklet 16 px-variant ved behov. Eksisterende installasjonsikon
er foreløpig beholdt. FUNC-019 knytter senere SVG/PNG/FOX-ressurser til samme
`Icon=xfmd` og eksisterende desktop-/MIME-identitet.

## 9. Verifikasjon og beslutningsporter

| Nivå | Relevant bevis |
| --- | --- |
| Rene enhetstester | Scrollkurver med fake clock, reversering/rest, profilschema, page breaking og transform-rundtur |
| Kontraktstester | FrameKey/stale avvisning, source-byteankre, samme glyphgrunnlag på to outputtargets |
| Native FOX/X11 | Alle fire scrollflater, begge akser, preview enable, fysisk touchpad vs wheel, sync uten dobbel gain |
| PDF-integrasjon | Uavhengig leser sjekker sidetall, MediaBox og tekst; raster sammenlignes ved samme DPI |
| Transaksjoner | Lagringsfeil, full disk, cancel, dokumentbytte under eksport, ingen dirty-/historikkendring |
| Brukerflate | Preferences OK/Cancel, formatbytte, fullscreen/restore, ikon ved små størrelser og installasjon |

Test mekanismer og risikokontrakter; unngå en test per trivial menydelegering.
Eksisterende endepunkt-/anker-/UTF-8-regresjoner kjøres ved berørte endringer.
P9 måler korte representative dokumenter og eksisterende 1 MiB-fixture for
continuous, paged og PDF. Baseline SR-011 beholdes. Separate grenser for antall
sider, minne, outputstørrelse, tid og cancel-respons dokumenteres før P11/P12 Ready;
de skal ikke arves ukritisk fra én skjermflate. Ingen 30-minutters stresstest.

## 10. Oppsummering

Behold lagene, parseren, dokumentøkten og synkroniseringen. Utvid én wheel-inputvei
med en ren, testbar bevegelsespolicy. Gjør fysisk layout og fontgrunnlag felles før
PDF-eksport bygges. Seks avgrensede functionality-objekter dekker de nye behovene;
to features samler brukerakseptansen. [P9–P13](implementationPlan.md#5-planlagt-utvidelse-p9p13)
leverer dette i verifiserbare steg uten å endre den kjørende appen i denne designfasen.
