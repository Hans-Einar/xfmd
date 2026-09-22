# Kravspesifikasjon: xfmd

Status: **Implemented P0–P14, revisjon 1.3**, 2026-09-13.
UR-015–020 og SR-015–019 er implementert i P9–P13; se fasebevis for faktisk dekning. Tidligere designgrunnlag ble
godkjent før implementasjon. Krav er normative; målinger og begrensninger finnes
i [P13-verifikasjonen](docs/evidence/P13.md), med P7 som historisk baseline.

## 1. Formål og avgrensning

xfmd skal være en rask, lokal Markdown-viser med enkel redigering, bygget på FOX
og naturlig å bruke sammen med Xfe-verktøyene `xfw` og `xfi`. Brukeren skal kunne
lese, navigere, redigere og sammenligne kilde med rendret dokument i ett vindu.
Første leveranse er en selvstendig Linux-applikasjon uten nødvendige endringer i
`xfw`, `xfi` eller Xfe. «Companion» betyr komplementær bruk og filåpning, ikke at
andre programmer allerede har et integrasjons-API.

FOX er applikasjonens toolkit. Interpreter og renderer skal kunne erstattes
uavhengig gjennom avtalte kontrakter. Se [arkitekturen](softwareArchitecture.md).

## 2. Begreper og kravstatus

- **Use case (UC):** brukerens situasjon og forløp; kan krysse flere features.
- **User requirement (UR):** observerbart resultat eller brukerregel.
- **System requirement (SR):** intern kontrakt, kvalitet eller begrensning.
- **Feature (FTR):** avgrenset, sammenhengende evne med egen akseptanse.
- **Functionality (FUNC):** tjeneste/atferd med ett eierlag og eksplisitte kall.

UR-001–014 og SR-001–014 beholder tidligere leveransescope. Nye UR-015–020 og
SR-015–019 inngår nå i FirstRelease gjennom P9–P13.
UR-010/SR-014 for xfw-IPC står fortsatt Future. ID-er
er stabile; slettede krav beholdes som `Retired` med begrunnelse. Et dokument eller
en stub oppfyller ikke i seg selv et funksjonelt krav.

## 3. Use cases

| ID | Forløp og forventet resultat | Alternative forløp | Krav |
| --- | --- | --- | --- |
| UC-001 | Start `xfmd fil.md` eller åpne via filbehandler; les rendret dokument. Uten argument vises tom dokumentflate og utfylt sidetre med ~ som arbeidsrot. Mappeargument setter arbeidsroten. | Manglende/uleselig fil eller ugyldig encoding gir feil uten tap av gjeldende buffer. | UR-001, UR-002, UR-009 |
| UC-002 | Følg lokal dokumentlenke; bruk Alt+Venstre/Høyre til tilbake/frem med bevart leseposisjon. | Avbryt dirty-dialog, brutt lenke eller feilet lasting endrer ikke historikken. | UR-005, UR-009 |
| UC-003 | Vis/skjul sidepanel med F10; velg mappe og enkeltklikk `.md`/`.txt`. | Enkeltklikk på fil åpner; mapper navigeres. Uleselig mappe gir feil uten dokumentbytte. | UR-006, UR-009 |
| UC-004 | Velg side-ved-side, rediger, se preview etter pause, angre/gjør om, søk og lagre. | Parse-/lagringsfeil beholder edits; lukking/dokumentbytte spør om ulagret tekst. | UR-003, UR-004, UR-007, UR-009 |
| UC-005 | Scroll i editor eller preview og se tilsvarende kildeavsnitt i motsatt flate. | Resize, skjult panel eller gammel mapping gir re-layout eller midlertidig deaktivert sync. | UR-008 |
| UC-006 | Senere: bruk separat `xfw` som editor og xfmd som preview. | Frakobling beholder siste lokale visning og gir status. | UR-010 |
| UC-007 | Åpne Preferences, juster og prøv scrolling, lagre eller avbryt. | Feil verdier/lagring bevarer aktiv profil. | UR-011, UR-015, UR-016 |
| UC-008 | Bytt til A4, kontroller sideskift og eksporter PDF fra aktuell buffer. | Cancel/feil bevarer kilde og mål. | UR-017, UR-018 |
| UC-009 | Les eller rediger i fullscreen og gå tilbake til samme arbeidsflate. | Aktiv dialog bruker Escape først. | UR-019, UR-007 |

## 4. Brukerkrav

| ID | Krav | Akseptanse / beviskriterium |
| --- | --- | --- |
| UR-001 | Åpne lokale `.md`/`.txt` via CLI, dialog og sidepanel i samme aktive økt. `.txt` vises som ren tekst. | AT-001: mellomrom/Unicode i sti, tom/manglende fil; ingen uønsket ekstra prosess. |
| UR-002 | Vis H1–H6, avsnitt, fet/kursiv, lister, sitater, kode og lenketekst. Brødtekst/overskrifter er proporsjonale; kode er monospace; overskriftsnivåer er tydelige. | AT-002: fixtures og visuell kontroll av blandede fonter, nesting og linjebryting. |
| UR-003 | Tilby tekstredigering, vanlig utklippstavle, angre Ctrl+Z, gjør om Ctrl+Y, søk Ctrl+F og lagring Ctrl+S. | AT-003: lagre–åpne gir samme tekst; undo/redo oppdaterer dirty og preview. |
| UR-004 | Oppdater preview når det har gått 300 ms uten ny redigering. Behold fokus og editorens markør. | AT-004: simulert klokke bekrefter debounce; GUI-sjekk bekrefter fokus og nyeste revisjon. |
| UR-005 | Relative dokumentlenker løses fra mappen til den åpne filens absolutte sti, uavhengig av prosessens arbeidsmappe. Absolutte dokumentstier åpnes direkte. Vis `#` foran relative Markdown-lenker, `/#` foran absolutte Markdown-lenker og fonttegnet `↗` foran HTTP(S)-lenker. Lokale lenker og tilbake/frem gir dokumentnavigasjon med sesjonshistorikk og gjenopprettet leseposisjon. | AT-005: A→B→C, tilbake til B, ny lenke D sletter frem-grenen; avbrutt/feilet åpning endrer ikke køen. |
| UR-006 | Sidepanelet viser mapper og filer under arbeidsroten, med valgfrie filtype-/navnefiltre, kan skjules eksplisitt og åpner filer på enkeltklikk. Klikk på `/`, mapper og filer skal aldri automatisk skjule panelet; dokumentbytte bevarer valgt synlighet. Mapper navigeres også når navnet ender på `.md`. | AT-006: blandede filtyper, enkelt-/dobbeltklikk og F10. |
| UR-007 | Tilby preview alene (standard), editor alene og editor venstre/preview høyre i justerbar splitter. | AT-007: modusbytte bevarer dokument, dirty, undo og fokus. |
| UR-008 | Synkroniser scrolling begge veier etter kildeanker, også med wrapping og varierende teksthøyde, uten pendling. | AT-008: overskrift, lang liste og kodeblokk holder tilsvarende avsnitt synlig; resize og tom fil testes. |
| UR-009 | Vis dirty-status og handlingsrettede feil. Ved bytte/lukking tilby Lagre, Forkast eller Avbryt. Avbrudd/feil bevarer gjeldende dokument. | AT-009: skrivefeil, ekstern endring, avbrudd og feilet nytt dokument gir ingen stille tap. |
| UR-010 | Senere: motta dokument- og kildeankeroppdateringer fra `xfw` uten å endre interpreter/renderer. | AT-010: framtidig IPC-integrasjonstest; ikke første leveranse. |
| UR-011 | Wheel-/gesture-scrolling i sidetre, arbeidsstihistorikk, editor og preview bevarer små delbevegelser og når eksakt topp/bunn eller venstre/høyre. Dragging av scrollbar og standard modifikatortaster beholdes. | AT-025: små og hele wheel-deltaer, begge retninger/akser, endepunkter og reversering testes med ekte FOX-scrollbarer. |
| UR-012 | Start uten argument med ~ som arbeidsrot; `xfmd .` bruker absolutt PWD, og annet mappeargument brukes som rot. Filargument åpner dokument med foreldre-mappen som arbeidsrot. Treets rot er synlig og utvidet ved oppstart. Vanlig trenavigasjon kan ikke gå utenfor roten. Dobbeltklikk rot utvider eksplisitt til ~, deretter /. | AT-026: oppstart uten argument, relativ/absolutt mappe og fil; rotsekvens, feil sti og avgrensning. |
| UR-013 | Høyreklikk mappe tilbyr «Set work path». Arbeidsstier vises under treet som klikkbar, unik historikk (nyeste først, maksimum 32), lagret mellom oppstarter. Aktivering av ugyldig historikk bevarer roten og gir forklaring. Dokument, dirty og panelsynlighet bevares ved rotbytte. | AT-027: kontekstmeny, historikkvalg, persistens, slettet mappe og dirty-buffer. |
| UR-014 | Filtrer filnavn med delstreng, ? (ett Unicode-tegn) og * (null eller flere tegn). Aktive *.md / *.txt-knapper kombineres med OR før AND med navnefeltet. Ingen aktiv typeknapp betyr alle filtyper; tomt navnefelt betyr alle navn. Vis bare matchende filer og deres forfedremapper ved aktivt filter; behold roten også ved null treff. | AT-028: knappkombinasjoner, kjedet filter, wildcard/Unicode, dype treff, null treff, raske filter-/rotbytter og uleselige mapper. |

### Utvidelse 1.2 (FirstRelease / Implemented)

| ID | Krav | Akseptanse / beviskriterium |
| --- | --- | --- |
| UR-015 | Edit → Preferences åpner en FOX-dialog med scrollhastighet og akselerasjonskontroller. Endringer prøves i et avgrenset scrollfelt; OK lagrer og oppdaterer alle xfmd-scrollflater, Cancel bevarer tidligere aktive/lagrede verdier. Ingen endring i andre FOX-programmer. | AT-029: åpning via meny, prøvefelt, OK/Cancel, restart og gyldig hastighet i tre, historikk, editor og preview. |
| UR-016 | Brukeren kan aktivere scrollakselerasjon og justere styrke og maksimum separat fra grunnhastighet. Langsom bevegelse skal være presis, rask bevegelse kunne flytte mer; reversering og endepunkter gir ingen oppsamlet bevegelse. Dragging, tastaturscroll og synkronisering akselereres ikke. | AT-030: tidsbestemte inputserier, egen base/gain, av/på, begge akser, reversering og eksakt topp/bunn. |
| UR-017 | View tilbyr Window wrap og A4 page preview. Window wrap følger previewens innholdsbredde; A4 viser faktisk sidegeometri med marger og sideskift. Zoom/fit-width endrer A4-visningens skala, aldri papirbredden. Bytte bevarer kildeanker, dirty, undo og view mode. | AT-031: resize, formatbytte, A4 210×297 mm, marger, zoom, lang kode/lister og kildekart over sideskift. |
| UR-018 | File → Export PDF eksporterer gjeldende buffer, også ulagrede edits, som lokal flersiders PDF. Sidevisning og eksport bruker samme papirprofil, fontgrunnlag og layout. Feil/Cancel bevarer kilde og eksisterende målfil; eksport endrer ikke dirty eller dokumenthistorikk. | AT-032: A4-preview/PDF har samme tekstplassering og sideskift; valgbar Unicode-tekst, bufferrevisjon, feilet skriving og avbrudd. |
| UR-019 | View → Full Screen / F11 toggler fullscreen på aktuell skjerm. F11 og Escape uten aktiv modal dialog forlater modusen. Tidligere vindusgeometri, maksimert tilstand, splitter, sidebar og editor/preview-modus bevares; fullscreen er uavhengig av papirprofil. | AT-033: Window Maker-bekreftet fullscreen og retur, modal Escape, flere skjermer, dirty/undo og A4/continuous. |
| UR-020 | xfmd får en egen gjenkjennelig dokument-/Markdown-identitet med samme ikon i desktop-entry og FOX-vindu. Master er SVG med avledede størrelser som er lesbare ved 16–64 px. | AT-034: lys/mørk bakgrunn, 16/24/32/48/64/128 px, desktop-entry og faktisk window icon; motiv endrer ikke appnavn/MIME-identitet. |

## 5. Systemkrav

| ID | Krav | Akseptanse / beviskriterium |
| --- | --- | --- |
| SR-001 | `application`, `interpreter`, `renderer` er egne kildekataloger. Bare application integrerer FOX; delte kontrakter har ingen FOX-/parser-typer. | AT-011: byggeavhengigheter og headless kontrakttester; alternative implementasjoner krever bare wiring. |
| SR-002 | Én tydelig eier per dokumenttilstand og functionality; ingen direkte mutasjon mellom søskenwidgets eller feature-interner. | AT-012: eierskaps- og kallgjennomgang mot blueprints. |
| SR-003 | Interpreter produserer uforanderlig semantisk modell med revisjon og kildeankre; renderer konsumerer uten parserkall. | AT-013: fixture og ekte/fake implementasjoner bruker samme kontraktstest. |
| SR-004 | Bruk cmark-gfm 0.29.0.gfm.13 med CMARK_OPT_DEFAULT og eksplisitt table-utvidelse; andre utvidelser er av. Ingen stilltiende aktivering av voksende «GitHub dialect». | AT-014: versjon/flaggliste og corpus med Unicode, entiteter, HTML som inert tekst og ufullstendig syntaks. |
| SR-005 | Ingen nettverkslasting, skripteksekvering eller browser engine. Lenker sendes aldri til shell for evaluering. | AT-015: eksterne ressurser, `javascript:`, `data:` og HTML gir ingen slik sideeffekt. |
| SR-006 | Valider UTF-8 og input før dokumentbytte. Bevar linjeslutt og BOM ved lagring; ingen automatisk normalisering. | AT-016: UTF-8, LF/CRLF, BOM, ugyldige byte og NUL-policy testes byte-for-byte. |
| SR-007 | Lagring skriver midlertidig søskenfil og erstatter mål bare ved suksess, bevarer vanlige modusbits og kontrollerer ekstern endring. | AT-017: feil før rename, full disk og endret mål bevarer fil og dirty-buffer. |
| SR-008 | Preview/layout har dokument-ID, revisjon og layoutgenerasjon. Foreldet resultat publiseres eller brukes aldri til scrolling. | AT-018: dokumentbytte, edit og resize mens arbeid er ventende. |
| SR-009 | Mapping bruker eksplisitte UTF-8-byteoffsets/kildeankre, ikke likhet mellom byte, tegn, visuelle rader og linjenumre. | AT-019: Unicode, gjentatt tekst, entiteter, lister, tomme blokker og wrapping; mappingkvalitet rapporteres. |
| SR-010 | FOX-objekter/GUI-mutasjon eies av GUI-tråden. Én bounded parser-worker bruker bare rene data; FOX-måling/layout/paint bruker GUI-tråden. | AT-020: timer-/objektlevetid og blokkering måles; worker-design krever eksplisitt revisjon. |
| SR-011 | Filgrense 8 MiB; benchmark-corpus til 1 MiB. Referansemaskin: p95 parse+layout ≤100 ms, første visning ≤500 ms for 1 MiB, RSS ≤100 MiB. | AT-021: 30 varme kjøringer og egen kaldstart; oppgi maskin/bygg/input. Overskridelse krever tiltak eller kravrevisjon. |
| SR-012 | Features/functionality har krav-ID, eier, kontrakter, plumbing, feilvei, gjenbruk og verifikasjon. | AT-022: validator og manuell semantisk sporbarhetskontroll. |
| SR-013 | Roller deles i fokuserte filer; nye tjenester begrunnes i krav og gjenbruk eller nødvendig ansvarsgrense. | AT-023: filkart og review; ingen skjult funksjonalitet i vindusklasse/generisk hjelpefil. |
| SR-014 | Senere: IPC versjonerer protokoll, avgrenser meldingsstørrelse, kontrollerer lokal peer og dokument/revisjon. | AT-024: framtidige tester av feil peer, partial reads, gammel revisjon og frakobling. |

### Systemkontrakter 1.2 (FirstRelease / Implemented)

| ID | Krav | Akseptanse / beviskriterium |
| --- | --- | --- |
| SR-015 | Scrollinput normaliseres én gang i application. Standardvei er FOX SEL_MOUSEWHEEL; direkte libinput/evdev/grab er ikke nødvendig. Kilde, enhet, tidsenhet og akse er eksplisitte eller Unknown. Ingen dobbelbehandling ved eventuell senere XI2-adapter. | AT-035: ekte X11-dispatch, kvantisert fallback, komprimerte events, små syntetiske deltaer, akser og ingen dobbeltforsterkning. |
| SR-016 | Renderer/kontrakter forblir FOX-/Cairo-/parser-frie. Sidegeometri bruker fysiske points (1/72 inch); sidepreview og PDF deler formede glypher/fontidentitet og sidemodell. Viewport/DPI/zoom er separat transform. | AT-036: fontfallback og glyph-/clusterdata, PDF MediaBox, identiske sidelinjer ved DPI/zoom-bytte; rastertoleranse dokumenteres. |
| SR-017 | Eksport har frosset SourceSnapshot, PaperSpec og FontSetId. Jobben publiserer atomisk fra søsken-tempfil etter suksess; dokumentbytte, edits, cancel og shutdown har eksplisitt policy. Ressurser og tråder er begrenset og eies uten FOX-kall fra worker. | AT-037: pågående eksport under edit/bytte, ugyldig/slettet mål, full disk, cancellation før commit og bounded shutdown. |
| SR-018 | Applikasjonspreferanser har versjonert skjema, grenser og validering ved både lesing og skriving. Ugyldig eller ukjent innhold gir dokumentert fallback/bevaring. Aktiv profil skifter først etter bekreftet lagring; WorkPaths-historikken bevares. | AT-038: roundtrip, manglende/ugyldige verdier, lagringsfeil, ukjente felter, schema-version og eksisterende historikk. |
| SR-019 | FrameKey identifiserer dokument/revisjon, layoutprofil, fontsett og generasjon. SourceAnchor beholder UTF-8-bytebetydning. Viewportmeldinger skiller UserWheel, UserDrag, Keyboard, Sync og Restore; bare UserWheel gjennomgår scrollakselerasjon. | AT-039: ingen echo/dobbel gain; gamle layoutprofiler avvises; ankerrestore gjennom A4, page gap, zoom og fullscreen. |

## 6. Detaljpolicy for første leveranse

Dette er eksplisitte forslag slik at implementasjonen slipper å gjette:

- **Markdown er tolerant tekst.** «Validere `.md`» betyr encoding, ressursgrenser
  og støttet profil. Ufullstendig Markdown under skriving blokkerer ikke lagring.
  Egen lint-/kvalitetsfeature krever et brukerkrav om diagnoser og forslag.
- `.md`/`.txt` matches uten hensyn til ASCII store/små bokstaver. Andre endelser
  avvises med forklaring. NUL avvises som ikke-støttet editorinput; tom fil er
  gyldig. Blandede linjesluttsekvenser må bevares via editoradapteren; hvis ikke,
  avvis redigering eksplisitt fremfor å normalisere stille.
- CommonMark-kjerne prioriteres. GFM-tabeller inngår fra P14. Task lists, strikethrough og fotnoter
  er senere utvidelser. Full CommonMark-konformitet påstås først etter tester.
- HTML vises inert. Bilder vises med alt-tekst/plassholder; heller ikke lokale
  bilder dekodes i første leveranse. CSS, JavaScript, plugins og IDE er utenfor scope.
- Relative lenker løses mot dokumentets mappe. `..` er tillatt: dette er en lokal
  viewer, ikke en prosjekt-sandbox. Vanlige relative/absolutte lokale stier er
  tillatt; nett-URI-er og andre schemes åpnes ikke automatisk. Fragmentlenker
  markeres som ikke støttet i første versjon; ingen stille feilnavigasjon.
- Historikk er liste med cursor, ikke FIFO. Foreslått grense 100 poster, kun i
  minnet. Lik gjeldende destinasjon gir ikke duplikat. Filer leses på nytt ved
  tilbake/frem; historikken er ikke en kopi av gamle dokumentversjoner.
- Eksternt endret fil avbryter vanlig lagring. Tilby ny lasting etter dirty-sjekk
  eller lagring under annet navn, uten automatisk merge. Symlinker lagres til
  eksplisitt oppløst mål. Flere hardlinks avvises for erstatningslagring med tilbud
  om annet navn. ACL/xattr-policy avklares i P0; metadata tapes ikke stille.

## 7. Sporbarhet og gjennomgang

[Blueprint-registeret](src/blueprint/README.md) kobler hvert UR/SR til designobjekter
og AT-ID-er. Load/save, splitter og filtrering er functionality, ikke automatisk
features. Før implementering gjennomgås fontstrategi, presis mapping, ytelsesbudsjett,
Markdown-profil og filpolicy. IPC, fragmentlenker og lokale bilder krever senere
kravrevisjon. Ingen SDL-kompilator eller avhengighet til SDP innføres nå.

## 8. Arbeidsrot og filfilter

Arbeidsroten avgrenser sidetreet; eksplisitt Åpne og dokumentlenker beholder sin
sti-policy og flytter ikke arbeidsroten. Symlinker til filer utenfor roten og
symbolske mappelenker traverseres ikke i treet. Skjulte mapper/filer tas med.
Dobbeltklikk en rot utenfor home går også til home, deretter /; på / beholdes /.
Begge typeknapper er av som standard, så oppstart viser direkte mappeinnhold uten
et fullstendig rekursivt søk. Navnefilter uten wildcard er delstreng; med wildcard
matches hele filnavnet. ASCII-bokstaver matches uten hensyn til store/små bokstaver.

Rekursiv filtrering utføres i separat worker uten FOX-kall og publiserer treff
fortløpende. Vanlig treutvidelse leser bare den valgte mappen. Rot-/filterbytte
kansellerer gammel skanning; GUI-objekter tilhører bare GUI-tråden. Status viser
pågående søk, null treff eller antall uleselige mapper. Historikk er separat fra
dokumenthistorikk og endrer ikke prosessens arbeidsmappe.

## 9. Avgrensning for P9–P13

[Designrevisjon 1.1](softwareDesign.md) og [P9–P13](implementationPlan.md#5-planlagt-utvidelse-p9p13)
beskriver integrasjonen. A4 er første papirformat, portrett og 20 mm marger som
standard. Sideprofil har egen kontrakt slik at flere formater kan
legges til senere. Window wrap er fortsatt standard. Kode beholder horisontal
scroll i Window wrap; i A4 brukes visuell wrapping uten å endre kildebytes.
Markdown-dialekten, bilde-/HTML-policy og UR-010/IPC utvides ikke av PDF-eksport.

SR-008/009 er baseline for SR-019. SR-010s GUI-eierskap består: ingen FOX-ressurser
deles med eksport-worker. P11/P12 og P13 verifiserer typografi-/PDF-adapternes trådeierskap. SR-011s eksisterende terskler gjelder baseline continuous;
P9 definerer side-/PDF-grenser, og P12/P13 dokumenterer målingene.


## P14: konkretisert tabell- og inputatferd

UR-002 / SR-004: GFM-tabeller viser header, celler, kolonnejustering og bryter
tekst innenfor kolonnen. Rader beholdes samlet i A4/PDF. Maks 64 kolonner og
50 000 celler; for smal A4 eller rad høyere enn siden gir forklaring uten kildetap.
AT-002/014/032 dekker escaped pipes, Unicode, inline-stiler, tomme/ujevne rader,
kildeankre, smal preview og tabeller over flere sider. Ingen andre GFM-utvidelser
aktiveres automatisk. Header gjentas ikke på fortsettelsessider i P14.

UR-005 / SR-010: Preview frigjør pointer-grab ved release før lenkecallback,
også når frame er foreldet. Bare et venstreklikk startet på samme lenke og uten
andre knapper eller drag aktiverer den. Høyre-/midtklikk endrer ikke kildetekst.
AT-005/020 dekker blandede knapper, gjentatte klikk, release utenfor og videre
knappebruk; Unicode-markøren bruker samme fontmåling/tegning som lenketeksten.

## P15: sidepanelmoduser, indeks og referanser

| Krav | Normativ atferd | Akseptanse |
| --- | --- | --- |
| UR-021 | Sidepanelet har Files- og Index-faner. Index deles vertikalt i kapitteltre øverst og References nederst. F10 gjelder hele panelet. | AT-040: fanebytte, splitter og F10 bevarer dokument/dirty. |
| UR-022 | Kapitler og underkapitler vises etter heading-nivå i gjeldende buffer. Enkeltklikk navigerer til overskriften i editor/preview uten å lese filen på nytt. | AT-041: nivåhopp, UTF-8, redigering og byteankre. |
| UR-023 | References har Markdown og Hyperlinks. Lokale .md-lenker listes én gang per normalisert sti; utvid filen for å lese dens øverste heading-nivå. Enkeltklikk åpner fil/kapittel med felles dirty-policy og historikk. | AT-042: relative/escaped stier, tabell-lenker, manglende filer, toppnivå og dirty-cancel. |
| SR-020 | Referanseoverskrifter lastes ved utvidelse på én arbeidstråd via parserporten; ingen rekursiv skanning eller nettlasting. Gamle jobber/klikk forkastes ved dokumentrevisjon. | AT-043: stale resultater, bytte under lasting, lesefeil og ryddig stopp. |

Presisering av SR-005: eksplisitt klikk på HTTP(S) under Hyperlinks kan åpne
standardnettleseren via argv til xdg-open, aldri shell. Alle andre lenker der
hopper til forekomsten i dokumentet. Previewens eksisterende lokal-lenke-policy
endres ikke. Ingen lenker aktiveres ved bygging av treet eller valg med opp/ned/venstre.
Kategorinoder velger/utvider treet; de har ingen fil å åpne. Dobbeltklikk på
filtreets rot beholder «bredere arbeidsrot». «Top level» er laveste heading-nummer
som finnes i referert fil; H2 vises hvis filen ikke har H1. Filreferanser med
fragment åpner filen; kapittelbarn navigerer med byteanker, ikke URL-slug.

## P16: programvalg for nettlenker

| Krav | Normativ atferd | Akseptanse |
| --- | --- | --- |
| UR-024 | Edit → Preferences lar brukeren velge programmet som åpner HTTP(S)-lenker fra både preview og referansetreet. `google-chrome-stable` kan velges eller skrives inn, alternativt programnavn/full sti eller systemvalget `xdg-open`. OK lagrer, Cancel forkaster. | AT-044: vedvarende verdi, Cancel, ugyldig verdi, manglende program og samme rute fra begge flater. |

P16 presiserer SR-005 og P15-policy: bare eksplisitt aktivering åpner HTTP(S) i
valgt eksternt program. Program og URL er separate argv-elementer; ingen shell
eller kommandolinje-evaluering. Feltet inneholder én executable, uten parametre.
Systemstandard er xdg-open; lokale innstillinger kan velge Chrome. Andre URI-schemes
blir ikke eksekvert. Konfigurasjon og analyser av Xfe endrer ikke renderer-arkitekturen.

## P17–P19: konsistent FOX-utseende og arbeidsflate

| Krav | Normativ atferd | Akseptanse |
| --- | --- | --- |
| UR-025 | Appearance har Light/Dark, Comfortable/Compact, Flat/Classic og kontrollfontstørrelse. Tema, mål og ikoner har felles eiere og validerte profilverdier. Previewens lesefarger følger Light/Dark med egne profiler (UR-030); dokumentlayout og PDF-utseende endres ikke. | AT-045: profiler, ugyldige overrides/fallback, persistens og uendret dokument/layoutprofil. |
| UR-026 | Toolbar grupperer ikonhandlinger for Open/Save og Back/Forward, Sidebar, eksklusiv Editor/Split/Preview og Light/Dark-toggle. Meny/tastatur/toolbar deler enabled/checked via CommandRouter. Files og Index samt lokal preview-header følger samme stil/mål og beholder eksisterende navigasjon. | AT-046: native klikk/Space, disabled/checked, tema-toggle, smalt vindu og eksisterende navigasjons-/scrolltester. |
| UR-027 | Preferences grupperer Appearance, Scrolling, Document og Programs i én transaksjon. Live utseendeprøving lagres først ved OK; Cancel/kryss gjenoppretter aktivt utseende. Skrivefeil beholder gammel lagret/aktiv profil og viser feil. | AT-047: OK/Cancel/kryss, skrivefeil, tema fra begge innganger, øvrige innstillinger og fokus. |


## P20: ekstern filåpning og tydeligere visningsmoduser

| Krav | Normativ atferd | Akseptanse |
| --- | --- | --- |
| UR-028 | Files-treet åpner .md/.txt i XFMD og andre regulære filer med xdg-open. Ekstern åpning endrer ikke buffer, dirty, undo eller dokumenthistorikk. Feil rapporteres; filsti er eget argv-element uten shell. | AT-048: klikk/tastatur, mellomrom/metategn, store suffikser, feil og uendret dokument. |
| UR-029 | Enter og Space aktiverer valgt node i sidetrærne. Høyrepil aktiverer leaf-noder; grennoder beholder vanlig utvidelse/barnenavigasjon. Opp/ned/venstre åpner ikke filer. | AT-049: Files, Index og References med native tastetrykk; lazy grener regnes ikke som leaf. |

UR-026 / AT-046 presiseres: knapperekkefølgen er Editor/Split/Preview med
layoutikoner. Retur fra Editor/Preview til Split gjenoppretter en brukbar,
lagret deling; gjentatte bytter, resize og kollapsede paneler må ikke skjule
preview bak en editor i full bredde. Snarveier Ctrl+1/2/3 beholdes.

## P21: lesefarger i preview

| Krav | Normativ atferd | Akseptanse |
| --- | --- | --- |
| UR-030 | Light/Dark velger egen lesepalett i både kontinuerlig og A4-preview, også ved levende Appearance-prøving/Cancel. Over preview ligger bakgrunnskontroller til venstre og tekstkontroller til høyre: fargetone øverst, lysstyrke nederst. Endringer tegnes under dragging uten parsing, shaping, reflow, endret anker eller dirty. Alle tekstelementer, kode, tabeller og sidebakgrunn følger paletten; lenker beholder understreking. | AT-050: native slider-input, begge temaer/formater, dekorasjoner og uendret frame/revisjon/scroll; ingen endring av PDF-palett. |
| UR-031 | De fire sliderposisjonene lagres separat for Light og Dark og gjenopprettes ved temabytte og omstart. Manglende/ugyldige verdier får temaspesifikke standarder. Lagring skjer ved avsluttet brukerjustering; feil vises og gjenoppretter sist lagrede profil. | AT-051: separate profiler, restart, gamle preferanser, ugyldige verdier, skrivefeil og Cancel i Preferences. |

Fargetone -1 er nøytral gråtone; 0–359 er hue med fast metning. Lysstyrke 0–100
skalerer RGB-kanalene (HSV Value), ikke skjermens fysiske baklys eller perseptuell
luminans. Standarder er nøytral mørk bakgrunn/lys tekst i Dark og omvendt i Light.
Kontrast vises etter sRGB/WCAG-formelen, uten å hevde at én farge er medisinsk best.
PDF bruker fortsatt standard utskriftspalett. Samme formede glypher/layout gjenbrukes.

## P22: minimal felles kontrollflate

| Krav | Normativ atferd | Akseptanse |
| --- | --- | --- |
| UR-032 | Meny, filhandlinger, visningsvalg og farger deler en lav topplinje; ved liten bredde brytes grupper uten overlapping. PREVIEW-tittel, kontrasttekst og Reset-knapp fjernes. Fargene gjelder både editor og preview med fortsatt separate Light/Dark-profiler. | AT-052: faktisk kontrollgeometri ved bredt/smalt vindu, alle moduser/temaer, live farger i begge flater og restart. |
| UR-033 | Wrap, A4 og Fit width/100% styrer begge dokumentflater. Editorens A4-modus viser redigerbar kilde med papirbasert tekstbredde; preview viser paginerte sider. Visningsendring bevarer buffer, markering, undo og kildeposisjon. | AT-053: editorbredde/font ved resize/format/zoom, begge paneler og fortsatt redigering. |
| UR-034 | Hover over preview-lenke viser oppløst lokal filsti eller nettadresse i statuslinjen. Avsluttet hover, scroll eller ugyldig frame fjerner gammel lenkeinformasjon. Hover åpner aldri dokumenter/programmer. | AT-054: relativ/absolutt/nettlenke, A4/scroll/leave og ingen navigasjon. |

P22 erstatter P21s plassering over preview og den synlige kontrastindikatoren.
Fargetone står over lysstyrke i to kompakte kolonner, med forklaring/verdi i tooltip.

## P23 — merking, matematikk og bilder

| ID | Krav | Akseptanse |
| --- | --- | --- |
| UR-035 | Preview skal støtte musemerking, Ctrl+A og Ctrl+C. Kopiert tekst er lesetekst, uten Markdown-markører; eksplisitte linje-/avsnittsskift bevares. Dra over en lenke skal merke, ikke navigere. | AT-055: Unicode, flere blokker/sider, clipboard/PRIMARY, stale frame og lenkedrag i native FOX. |
| UR-036 | LaTeX-matematikk støttes inline med `$…$`/`\(…\)` og separat med `$$…$$`/`\[…\]` samt math-kodegjerder. Kodeeksempler forblir bokstavelige. Formler følger lesefarger og vises også i PDF. | AT-056: brøk, rot, sum, indeks, matrise, kodegjerde og ufullstendig syntaks i begge visningsprofiler. |
| UR-037 | Markdown-bilder med lokal relativ eller absolutt filsti skal vises med bevart aspekt. Relative stier løses fra dokumentets mappe. PNG, JPEG, GIF og SVG støttes; feil gir synlig alttekst/forklaring. Bilder inngår i PDF. | AT-057: lokale/URL-kodede stier, manglende/ugyldig/stort bilde, lenket bilde, A4 og PDF. |

P23 erstatter tidligere bildeplassholder-avgrensning og utvider SR-004 med
den eksplisitte matematikkprofilen over. SR-005 gjelder fortsatt: ingen automatisk
nettverkslasting. Eksterne bilde-URL-er får forklarende plassholder. HTML er inert.
Matematikk er formelsats, uten TeX-fil-I/O, shell eller brukerdefinerte makroer.
Ressursarbeid skjer utenfor GUI-tråden med avgrenset input og bildestørrelse.

## P24: nylig brukte filer

| ID | Krav | Akseptanse |
| --- | --- | --- |
| UR-038 | Sidepanelet viser Recent files under Recent folders. Vellykket åpning og lagring registrerer absolutte filstier, nyest først, uten duplikater, maksimalt 32. Listen lagres separat fra arbeidsmapper og gjenopprettes ved oppstart. Valg bruker vanlig dokumentnavigasjon med dirty-kontroll, uten å endre arbeidsrot eller skjule panelet. | AT-058: rekkefølge, grense, restart, åpning/Save As, kansellering og manglende fil. |

## P25–P29: Mermaid-diagrammer

UC-008: Brukeren leser og redigerer Mermaid-blokker i et Markdown-dokument,
merker/kopierer diagrametiketter og eksporterer samme diagram til PDF.
Kravene nedenfor er implementert i P26–P29; [P29-bevis](docs/evidence/P29.md)
angir faktisk testdekning og begrensninger.
Første leveranse bruker profil **XFMD Flowchart 1**, definert i
[Mermaid-designet](docs/design/mermaid-integration.md). Andre diagramtyper utvides
først når modell, plumbing og akseptanse er definert; bibliotekets annonserte
støtte er ikke automatisk XFMD-støtte.

| ID | Krav | Akseptanse |
| --- | --- | --- |
| UR-039 | Et eksplisitt mermaid-kodegjerde skal vise flowchart/graph etter XFMD Flowchart 1, inkludert brukerens to eksempler. Vanlig kode og .txt forblir bokstavelig. Ugyldig, uferdig eller ustøttet innhold beholder hele kildeblokken med lokal forklaring uten å hindre resten av dokumentet. | AT-059: begge brukerdiagrammer, retninger, former, kanter, subgraphs, Unicode, ugyldig/ustøttet syntaks og vanlige kodeblokker. |
| UR-040 | Diagrammet skal følge Light/Dark og levende lesefarger uten ny parsing, layout, dirty eller scrollendring. Wrap/A4/zoom skal bevare aspekt og vise hele diagrammet; PDF bruker samme geometri med utskriftspaletten. | AT-060: tema/slider, resize, A4/sidegrense, skalering, flersidet dokument og vektorbasert PDF. |
| UR-041 | Diagrammet vises som bibliotekets ferdige SVG, med Approximate kildeanker til kodeblokken. Separate merkbare etikettfelt er utsatt; vanlig Markdown-merking beholdes. | AT-061: én skalerbar SVG-run, ingen dupliserte etiketter, vanlig tekstkopiering og kildesynkronisering. |
| SR-021 | Mermaid-parser og diagramlayout skal være uavhengig utskiftbare bak XFMD-eide, FOX-/Rust-/bibliotekfrie verdikontrakter. Renderer skal ikke parse Mermaid-kildetekst. C++/Rust-broen skal ha eksplisitt ABI-versjon, eierskap, feil og separate parser-/layoutinnganger. | AT-062: alternativ/falsk parser og layout, roundtrip-modell, C-ABI-livsløp, feil/panic og lag-/dependency-kontroll. |
| SR-022 | Diagramarbeid skal skje utenfor GUI-tråden med begrenset input, kø og cache. Ny revisjon gjør gamle svar ugyldige. Preview/PDF skal bruke samme forberedelse og fontgrunnlag. Ingen automatisk nettverk, shell, HTML, script eller filressurser fra diagramkilden. | AT-063: raske edits, stale svar, kansellert eksport, ressursgrenser, fontendring og input som forsøker aktive/eksterne konstruksjoner. |
| SR-023 | mermaid-rs-renderer og Rust-verktøykjeden skal bygges fra låste versjoner med sjekket Cargo.lock, lisensoversikt og dokumentert offline-bygg. Manglende byggavhengigheter skal gi tydelig configure-feil; full støtte må ikke avhenge av installert Node/Chromium eller nettverk ved kjøring. | AT-064: rent Linux-bygg, offline etter bootstrap, installert program, dependency-/lisenskontroll og Rust-/C++-verifikasjon. |

| SR-024 | Flowchart-ruting skal være utskiftbar uavhengig av nodeplassering. Libavoid integreres i bibliotekforken med låst kilde, eksplisitte port-/kantidentiteter, begrensede etikettpass, kooperativt avbrudd og synlig feil/fallback. | AT-065: frosne posisjoner/mål, rute-/kollisjonsmålinger, determinisme, native SVG, Legacy-valg og budsjett. |

P33 presiserer UR-039: lange kantetiketter skal kunne brytes ved ordgrenser før
layout, med faktiske linjemål brukt både til hindringer og SVG. Eksplisitte
linjeskift bevares; enkeltord deles ikke. Se [tekstpolicy](docs/design/mermaid-label-wrap.md).

P34 presiserer SR-024: en fremmed forbindelse skal ikke passere mellom en
kantetikett og segmentet etiketten er tilordnet. Dette valideres på ferdige
ruter; plassmangel skal gi eksplisitt diagnose.

P35 presiserer UR-039/041: kantetiketter skal om mulig ha en tynn SVG-peker
med prikk på tilhørende kant. Venstre side prioriteres; pekeren skal ikke
krysse andre noder, etiketter eller forbindelser. Manglende fri plass oppgis
som diagnostikk uten å skjule etiketten. Logiske ruter endres ikke.

## P36 — typed Mermaid-dekning

UR-039 og SR-021 presiseres med [versjonert matrise](mermaid_coverage.md):
diagramtyper bevarer egen semantikk og får eksplisitte profiler. Sequence 1 skal
vise ordnede Commands/svar, deltakere, notater og avgrensede fragmenter. Ustøttet
syntaks gir lokal kildefallback; diagrammer evaluerer ikke SDL-regler eller guards.
AT-059/060/062/063 utvides med APT-importens tre utfall, adskilt fra aktivering,
modell-roundtrip, Unicode, feilscope, preview og PDF. Senere typer følger matrisen.

P37–P41 utvider UR-039/040 og SR-021/022 med prioriterte typed diagramprofiler,
ordnet sekvensscope og ett praktisk `mermaid_evicence.md` for løpende lesing.
AT-059/060/062/063 gjelder hver profil. Dokumentgrensen skal fortsatt være
avgrenset, men romme minst ett eksempel per implementert diagramtype.

## Byggidentitet og leveranseflyt

| ID | Krav | Akseptanse |
| --- | --- | --- |
| UR-042 | CLI, vindustittel og høyrejustert tekst i nederste statuslinje viser major.minor, branch (PR-nummer på main når kjent) og numerisk commitnummer. | AT-066: samme innbakte identitet i CLI/vindu/statuslinje; dirty og ukjent kilde markeres. |
| SR-025 | Identiteten beregnes fra full Git-historikk uten nettverk eller en konfliktskapende tellerfil. Et eksplisitt bygg oppdaterer metadata også uten ny CMake-konfigurering; samme commit gir stabil identitet. | AT-067: lineære commits, branches, merge, main etter PR, detached, dirty, shallow og kildearkiv; uendret metadata omskrives ikke. |

## SDL-dokumentnavigasjon (Sprint 004)

| ID | Krav | Akseptanse |
| --- | --- | --- |
| UR-044 | To uavhengige Markdown-paneler viser navigator og hoveddokument. En registrert SDL-handling genererer valgt dokument i avsendervinduet. | AT-070: ekte panelklikk, bevart navigator, fokusbytte, flere vinduer og lukket mål. |
| SR-027 | Lokal XFMD1-protokoll adresserer konkret vindu/panel, har begrenset størrelse og samme bruker som peer. Verktøy registreres med program/argv; Markdown inneholder aldri shellkommando. | AT-071: feil vindu/panel, ugyldig/for stor forespørsel, stale sekvens og disconnect gir ingen feilåpning. |
