# xfmd — X File Markdown Viewer/Editor

Native Markdown-viser og editor for Linux, bygget på FOX som companion til `xfw`
og `xfi`. Første leveranse er implementert: typografisk visning, redigering med
undo/redo, live preview, lokal lenkehistorikk, justerbar scrolling, A4-preview,
PDF-eksport, fullscreen, tekstmerking i preview, LaTeX-matematikk, lokale bilder
og kapittel-/referanseindeks.

![XFMD-ikon](packaging/icons/xfmd-64.png)

## Dokumentindeks og referanser

Sidepanelet har **Files** og **Index**. Files beholder arbeidsrot, filfilter og
historikk; enkeltklikk åpner en fil. Index har et kapitteltre øverst og et
referansetre nederst, med en flyttbar skillelinje. Indeksen følger aktiv buffer.

- Klikk en overskrift for å hoppe dit; Enter og Space aktiverer også; høyrepil aktiverer endenoder og utvider grener.
- **References → Markdown** viser lokale `.md`-lenker. Utvid en fil for å lese dens
  hovedkapitler. Klikk filen eller et kapittel for å åpne. Kollaps/utvid leser på nytt.
- **References → Hyperlinks** viser øvrige lenker. HTTP(S) åpnes i valgt nettleser;
  andre lenker hopper til forekomsten i gjeldende dokument.
- Filbytte beholder dagens kontroll for ulagrede endringer og tilbake/frem-historikk.

Hovedkapitler er det øverste nivået filen faktisk bruker (H1, ellers H2 osv.).
Referanser leses ved utvidelse, ett nivå dypt, med samme 8 MiB-grense som filåpning.
Brutte lenker står synlig med feilmelding; trebygging laster aldri fra nettet.

Sidepanelets **Files**-fane har **Recent files** under **Recent folders**.
De 32 sist åpnede/lagrede filene huskes mellom omstarter. Klikk eller Enter åpner
med vanlig kontroll for ulagrede endringer; arbeidsroten beholdes. Fillisten
lagres i FOX-registry-seksjonen `RecentFiles`, separat fra `WorkPaths`.

## Bygg og kjør

Krever Rust/Cargo **1.92.0**, Python ≥3.11, `patch`, C++17-kompilator, CMake ≥3.20, Ninja, pkg-config, FOX ≥1.6.57 (1.6 API),
libcurl-verktøyet `curl`, X11/RandR, Cairo og PangoCairo/Fontconfig (inkludert utviklingsfiler).
Diagram-SVG krever librsvg ≥2.46 med utviklingsheaders. Bilder/formler krever GdkPixbuf med SVG-loader, cairomm-1.0, pangomm-1.4 og tinyxml2.
På Debian/Ubuntu: `libgdk-pixbuf-2.0-dev librsvg2-dev libcairomm-1.0-dev libpangomm-1.4-dev libtinyxml2-dev`.
På AlmaLinux/Fedora: `gdk-pixbuf2-devel librsvg2-devel cairomm-devel pangomm-devel tinyxml2-devel`.
PDF-verifikasjon bruker Poppler-verktøyene `pdfinfo`, `pdftotext` og `pdftoppm`. Installer DejaVu Sans/Mono og gjerne Droid Sans
Fallback eller Noto Sans CJK. Tester krever Python 3 og Xvfb. Fullscreen-testen bruker Window Maker når den er installert.

```sh
./tools/bootstrap_dependencies.sh
python3 tools/bootstrap_mermaid.py
cargo fetch --locked
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON
cmake --build build --parallel 4
ctest --test-dir build --output-on-failure
./build/xfmd tests/fixtures/markdown/basics.md
```

Bootstrap laster eksplisitt ned hashkontrollert cmark-gfm 0.29.0.gfm.13 til `.deps/`.
Første configure laster også hashkontrollert MicroTeX fra commit
`0e3707f6dafebb121d98b53c64364d16fefe481d`; ingen GTK-widgets eller nettlesermotor
bygges. For offline-bygg kan `FETCHCONTENT_SOURCE_DIR_MICROTEX` peke til denne
utpakkede kilden. Bruk `-DBUILD_TESTING=OFF` for bare
applikasjonen; `-DXFMD_SANITIZERS=ON` i separat Debug-bygg for ASan/UBSan.

```sh
cmake --install build --prefix "$HOME/.local"
```

Installasjon inkluderer binærfil, desktop-entry, ikon, man-side og tredjepartsnotis.
Prosjektlisens er fortsatt ikke valgt av eieren; ingen formell release er publisert.
[cmark-notisen](LICENSES/cmark.txt) gjelder den statisk lenkede parseren.

## Bruk

| Handling | Tast |
| --- | --- |
| Åpne / lagre / lagre som | Ctrl+O / Ctrl+S / Ctrl+Shift+S |
| Angre / gjør om / søk | Ctrl+Z / Ctrl+Y / Ctrl+F |
| Preview / editor / delt visning | Ctrl+1 / Ctrl+2 / Ctrl+3 |
| Merk alt / kopier i aktiv preview | Ctrl+A / Ctrl+C |
| Sidepanel | F10 |
| Fullscreen / tilbake | F11 / Escape |
| Eksporter gjeldende buffer til PDF | Ctrl+Shift+E |
| Tilbake / frem | Alt+Venstre / Alt+Høyre |

Sidepanelet beholdes ved mappevalg og filåpning; skjul/vis det selv med F10.
Markdown og `.txt` åpnes lokalt; `.txt` vises uformatert. Preview oppdateres etter
300 ms redigeringspause. Filgrensen er 8 MiB. UTF-8/BOM og eksisterende linjeslutt
bevares. Ekstern filendring gir konflikt; hardlenker krever Lagre som.

Lenker støtter lokale dokumentstier, inklusive relative stier og prosentkoding.
Relative Markdown-lenker merkes med `#`, absolutte med `/#`, og nettlenker med
`↗`. Relative stier regnes fra mappen til den åpne filen.
Lokale preview-lenker støttes uten fragment/query. HTTP(S) åpnes fra både preview
og referansetreet med programmet i **Edit → Preferences → Hyperlinks → Browser program**.
Velg eksempelvis `google-chrome-stable`, `firefox` eller `xdg-open` (systemstandard),
eller skriv full sti / bruk Browse. Feltet tar ett program uten argumenter; URL-en
overføres separat uten shell. OK lagrer valget; Cancel beholder det gamle.
Skript og HTML-eksekvering støttes ikke.
Bilder vises som alternativtekst. xfw-IPC, bilder og andre Markdown-utvidelser er senere
scope. Preview har fontfallback; editorens glyphdekning avhenger av valgt systemfont.

## Utvikling og design

- [Krav og use cases](xfmd_requirements.md)
- [Arkitektur og filkart](softwareArchitecture.md)
- [Analyse: Xfe-komponenter, utseende og videre integrasjon](docs/xfe-integration-study.md)
- [FOX UI-forslag og native prototyper](fox_ui_improvements.md)
- [FOX UX-forslag: verktøylinje og arbeidsflate](fox_ux_improvements.md)
- [Arbeidsmåte](docs/working-method.md) og [blueprints](src/blueprint/README.md)
- [Faser og milepæler](implementationPlan.md)
- [Oppdatert bidragsguide](CONTRIBUTING.md) og [opprinnelige agentregler](AGENTS.md)
- [Samlet verifikasjon](docs/evidence/P7.md)

AGENTS.md er bevart etter instruksen om ikke å overskrive en eksisterende fil.
Dens designfasestatus og planlagte byggkommandoer er historiske; denne README-en
og CONTRIBUTING.md beskriver implementasjonen. Arkitektur- og arbeidsreglene gjelder.

## Arbeidsområder og filfilter

- `xfmd` starter med utvidet hjemmekatalog (`~`) i sidetreet.
- `xfmd .` setter treets rot til gjeldende arbeidsmappe. `xfmd /sti/til/mappe`
  velger en annen rot; `xfmd fil.md` åpner filen med dens mappe som arbeidsrot.
- Dobbeltklikk rotnoden for å utvide til `~`, og igjen for å utvide til `/`.
  Vanlig mappeutvidelse holder seg innenfor valgt rot.
- Høyreklikk en mappe og velg **Set work path**. Tidligere arbeidsstier finnes
  under treet; ett klikk aktiverer en sti. De 32 nyeste unike stiene lagres mellom
  oppstarter. Start uten argument bruker fortsatt `~`.
- Filterknappene kombineres slik: `(*.md OR *.txt) AND navnefilter` når begge er
  på. Ingen aktiv knapp betyr alle filtyper. Knappene er av som standard.
  Tekst uten wildcard matches som delstreng; `?` matcher ett Unicode-tegn og `*`
  null eller flere tegn i hele filnavnet. `report?.*` matcher `report1.md` og
  `reportø.txt`, men ikke `report12.md`. ASCII-matching er case-insensitive.
- Aktivt filter søker i undermapper i bakgrunnen og viser bare filer med treff
  og mappene som leder til dem. **Refresh** leser treet på nytt etter filendringer.
  Status viser søk og uleselige mapper. Skjulte filer tas med; symbolske
  mappelenker traverseres ikke, og filsymlinker utenfor arbeidsroten utelates.

Arbeidsrot og historikk endrer ikke dokumentbuffer, prosessens PWD eller
sidepanelets synlighet. Dokumentlenker og Åpne-dialogen flytter ikke arbeidsroten.
Klikk på `.md`/`.txt` (også store bokstaver) åpner i XFMD. Andre regulære filer
åpnes med `xdg-open`, uten å bytte dokument eller spørre om å lagre det. Feil fra
starteren vises i XFMD. Dette gjelder Files-treet; Åpne-dialogen og CLI beholder
XFMDs dokumentpolicy. Enter og Space aktiverer valgt node; høyrepil aktiverer
fil-/endenoder. På mapper og lazy referansegrener beholder høyrepil vanlig
utvidelse og navigasjon til barn. Opp/ned/venstre åpner ikke filer.

## Preferences, scrolling og sidevisning

**Edit → Preferences** justerer grunnhastighet og valgfri scrollakselerasjon.
Prøv innstillingene i dialogens eget felt. OK lagrer profilen og oppdaterer
editor, preview, sidetre og historikkliste; Cancel beholder tidligere profil.
Standard er hastighet 1,5× med akselerasjon av. Alt bruker linjescroll og Ctrl
sidescroll uten akselerasjon. FOX/X11 kan levere kvantiserte hjulhendelser;
applikasjonen bruker ikke global libinput-hook og endrer ikke andre programmer.

**View → Window wrap / A4 page preview** velger flytende eller paginert layout.
A4 er portrett, med 20 mm marger som kan endres i Preferences. Fit page width
og Actual size (100%) endrer bare visningsskala. Formatbytte bevarer kildeanker,
redigering og delt visning. Lang kode får visuell wrap på papir, men kilden endres ikke.

**File → Export PDF** eksporterer hele gjeldende buffer, inkludert ulagrede edits.
Eksport fra Window wrap bruker samme A4-profil. PDF og preview deler fontforming,
linje-/sideskift og glyphposisjoner. Du kan fortsette å redigere under eksport;
status oppgir hvilken revisjon som ble eksportert. **File → Cancel PDF export**
avbryter før publisering. Eksport lagrer ikke Markdown-filen og endrer ikke undo.
Eksisterende PDF erstattes først etter vellykket skriving/flush; symlinks/hardlinks
som mål avvises. Nye PDF-er er private (0600); eksisterende POSIX-modus bevares.
Eierskap og utvidede attributter på tidligere PDF kopieres ikke.

**View → Full Screen / F11** bruker Window Makers fullscreen på aktuell skjerm.
F11 eller Escape går tilbake; en aktiv dialog får Escape først. Normal/maksimert
geometri, sidepanel, splitter og visningsmodus bevares.

Alle ikonstørrelser er committet og installeres med samme desktop-identitet.
Ved endring av SVG-masteren: `python3 tools/generate_icons.py` (krever rsvg-convert).

## Verifikasjon av P9–P13

[Faseplanen](implementationPlan.md) og [sluttrapporten](docs/evidence/P13.md)
knytter implementasjon til tester, målinger og kjente begrensninger.
[Endringsnotater](RELEASE_NOTES.md) beskriver leveransen. Fysisk touchpad og
fler-monitor hotplug må fortsatt prøves i den aktuelle desktop-oppsettingen.


### Tabeller og lenkemarkører (P14)

GFM-tabeller støtter header, kolonnejustering, inline-stiler/lenker og tekstbryting.
Preview og PDF bruker samme native layout. I A4 holdes hver rad samlet; header
vises på starten av tabellen og gjentas ikke på fortsettelsessider. Svært høye
rader må deles i kilden. For mange kolonner for A4 krever mindre marger eller
oppdeling; Window wrap kan scrolle brede tabeller horisontalt. Maks 64 kolonner
og 50 000 celler. Andre GFM-utvidelser er ikke aktivert.

HTTP(S)-lenker vises med Unicode ↗. Preview frigjør musegrab etter klikk, også
ved endret layout; høyre-/midtklikk i preview limer ikke inn eller følger lenker.
Editorens vanlige midtklikk/paste påvirkes ikke.


## Utseende og arbeidsflate

Verktøylinjen har Open/Save, Back/Forward, Sidebar og et eksklusivt valg mellom
Editor, Split og Preview, med kode-, delt rute- og øyeikon. Light/Dark-knappen bytter og lagrer tema, og beholder
øvrige utseendevalg. Ved smalt vindu flyttes tilgangen til skjulte verktøy over
til de eksisterende menyene og snarveiene. A4/Wrap og relevant zoom står ved preview.

**Edit → Preferences → Appearance** gir Light/Dark, Comfortable/Compact,
Flat/Classic og UI-fontstørrelse 8–18. Endringer forhåndsvises i appen. OK lagrer;
Cancel, Escape eller vinduskrysset gjenoppretter aktivt utseende. Lagringsfeil
beholder utkastet i dialogen og gjenoppretter aktivt utseende i arbeidsflaten.
Scrolling, Document og Programs er egne faner i samme dialog/transaksjon.
Tema endrer kontrollene og editorens farger. Dokumenttypografi, hvit preview-side
og PDF-farger er uavhengige av tema.

For å prøve egne farger og mål: kopier [appearance.ini](packaging/appearance.ini)
til `$XDG_CONFIG_HOME/xfmd/appearance.ini` (vanligvis
`~/.config/xfmd/appearance.ini`). Trykk **Reload style file** i Appearance.
Palettfeltene i `[light]`/`[dark]` er `surface`, `panel`, `field`, `text`, `muted`,
`border`, `accent`, `selected`, `hover` og `danger`, alle som `#RRGGBB`.
`[comfortable]`/`[compact]` støtter `gap` (2–16), `inset` (4–24), `height` (24–48),
`radius` (0–10) og `iconSize` (14–32). Mål skaleres med UI-fontstørrelsen.

Manglende verdier bruker innebygde standarder; filen er begrenset til 64 KiB.
Ugyldig reload viser en feil og beholder sist fungerende profil. Cancel angrer
også den levende profilforhåndsvisningen, men skriver ikke tilbake den eksternt
redigerte filen. Neste oppstart leser filen på nytt. Det finnes ingen filovervåker
eller ny runtime-avhengighet. Eksempelprofil installeres under `share/doc/xfmd`.

Se [implementasjonsplan](implementationPlan.md), [UI-laget](docs/design/fox-ui-layer.md)
og [testbevis og produktskjermbilder](docs/evidence/P17-P19.md).


P20 retter Editor → Split slik at editorens fullbredde ikke kollapser preview.
Den forrige paneandelen gjenopprettes ved retur til Split, tilpasset gjeldende
bredde. Klikk Split igjen for å hente tilbake et sammenklemt panel. Snarveiene
Ctrl+2 (Editor), Ctrl+3 (Split), Ctrl+1 (Preview) er beholdt.
[Testbevis og skjermbilde](docs/evidence/P20.md).

## Lesefarger

Light/Dark gjelder editor og dokumentpreview, inkludert A4-sidene. I topplinjen
ligger fargetone og lysstyrke for bakgrunn (venstre) og tekst (høyre). Venstre ende
av hue-skalaen er nøytral gråtone; 60° er gul og 120° grønn. Lysstyrke justerer
RGB-fargen, ikke monitorens baklys. Bildet oppdateres mens du drar.

Hvert tema husker sine fire sliderverdier mellom omstarter. Verdiene lagres ved
avsluttet justering. Ved
lagringsfeil gjenopprettes tidligere verdier og feilen vises i statuslinjen.
Lesefarger endrer ikke PDF,
dokumenttekst, layout eller leseposisjon. Se [forskningsnotatet](docs/design/reading-colors.md).

### Kompakt felles topplinje (P22)

Meny, fil-/visningsikoner, Wrap/A4/zoom og farger deler én lav topplinje, med
radbryting ved liten bredde. PREVIEW-overskrift, kontrasttekst og Reset-knapp er
fjernet. BG og Text har fargetone øverst og lysstyrke nederst; tooltip viser verdi.
Begge dokumentflater bruker valgt lesepalett, fortsatt lagret separat per tema.
A4 gir editoren papirbasert tekstbredde med Fit width/100%, mens preview viser
sideskift. Wrap gjenoppretter vindusbredde og normal editorfont. Lenke-hover i
preview viser oppløst lokal sti eller nettadresse nederst uten å åpne målet.

## Tekstmerking, matematikk og bilder

Dra med venstre museknapp for å merke preview-tekst. Ctrl+C kopierer leseteksten,
utelater dekorative lenkemarkører og beholder eksplisitte linjeskift. Linux PRIMARY
støttes også. Dra over en lenke merker teksten; et vanlig klikk følger lenken.
Ny dokumentrevisjon nullstiller merkingen, men bevarer allerede kopiert tekst.

Formler støtter `$x^2$`, `\(x^2\)`, `$$\frac{a}{b}$$`, `\[\sum_i x_i\]`
og kodegjerder merket `math` eller `latex`. Brøker, røtter, indekser, summer,
integraler og matriser typesettes av [MicroTeX](https://github.com/NanoMichael/MicroTeX).
Vanlige kodegjerder og inline-kode tolkes fortsatt bokstavelig. Dette er formelsats,
ikke en komplett TeX-installasjon; filinnlesing og makrodefinisjoner er av.

Bilder: `![Beskrivelse](tests/fixtures/markdown/diagram.svg)`. PNG, JPEG, GIF (første frame) og SVG støttes med
bevart aspekt; relative stier regnes fra dokumentets mappe. Lagre nye dokumenter
før relative bilder brukes. Eksterne URL-er lastes ikke ned automatisk. Manglende
eller ugyldige ressurser viser alttekst/formelkilde og en forklaring.

Maks 16 MiB per bildefil, 16 millioner piksler, 256 ressursforekomster og 16 KiB
per formel. Store bilder skaleres til tekstbredden/siden. Formler følger lesefargene;
bilder beholder egne farger. Begge inngår i PDF-eksporten.
Installasjonen inkluderer MicroTeX-ressurser og deres opprinnelige lisenser i
`share/xfmd/math/`; selve biblioteket er MIT-lisensiert.


## Mermaid-diagrammer

Gjerder med første infotoken `mermaid` vises med bibliotekets SVG via librsvg/Cairo. Flowchart 1 støtter
`flowchart`/`graph` med LR/RL/TD/TB/BT, rektangler, avrundede rektangler,
beslutningsnoder, sirkler, grupper, kjeder, sykluser og solide/stiplete/tykke
kanter. Bruk `A[Etikett] -->|Kanttekst| B{Valg}`. Begge brukerdiagrammene
finnes i `tests/fixtures/markdown/mermaid.md`.

Fargekontrollene virker direkte. Diagrametikettmerking er utsatt; diagramkilden
kan kopieres fra editoren. A4 og PDF bruker samme SVG og vektorgeometri. Lange LR-diagrammer skaleres ned til
visningsbredden. Sequence 1 støtter i tillegg en eksplisitt sekvensprofil,
se forfatterveiledningen nedenfor. Andre typer og init/CSS/HTML/click/ressursdirektiver
vises som kilde med forklaring, uten å ødelegge resten av dokumentet.

Flowchart-grensene er 64 KiB, 128 noder, 512 kanter, 32 grupper, gruppedybde 8 og
16 diagrammer per dokument. Dyr layout avbrytes kooperativt etter omtrent
to sekunder og gir lokal fallback; dette er ingen hard realtime-garanti.
AddressSanitizer-bygg har ti sekunders budsjett for instrumenteringskostnaden;
den installerte Release-utgaven beholder to sekunder.

### Bygg uten nett

Kjør dependency-bootstrapene og `cargo fetch --locked` én gang med nett,
og gjør en vanlig CMake-konfigurering. Behold `.deps/`, Cargo-cachen og
CMake-byggets `_deps/` (MicroTeX). Deretter kan konfigurering og bygg kjøres
med `CARGO_NET_OFFLINE=true` og `-DFETCHCONTENT_FULLY_DISCONNECTED=ON`.
Den installerte applikasjonen trenger verken Cargo, Rust, Node eller nettleser
for å vise Mermaid. Rust-/Mermaid-lisenser installeres under `share/doc/xfmd/licenses/`.

Mermaid-flowcharts bruker nå Libavoid fra en låst fork. Sett
`XFMD_MERMAID_ROUTER=legacy` før oppstart for eksplisitt sammenligning med gammel
ruter. `XFMD_MERMAID_CROSSING_JUMPS=1` viser valgfrie kryssingsbuer. Kryssinger er
tillatt; etiketter og noder skal være hindringer. Ved plassmangel vises en lokal
feil ved Mermaid-kilden, uten skjult motorbytte. Se
[bygg, lisens og relinking](docs/design/mermaid-libavoid-build.md) og
[SVG-/rutebeslutningen](docs/design/mermaid-svg-routing.md).

Lange Mermaid-kantetiketter brytes automatisk ved ordgrenser rundt 120 pt.
Eksplisitte linjeskift bevares, og enkeltord deles ikke. Node-/gruppetitler
beholdes; ved identisk tekst i node/gruppe og kant brukes den felles uombrutte
målingen. Preview og PDF bruker de samme målte linjene.

Der det er fri plass, får Mermaid-kantetiketter en tynn peker med prikk på
forbindelsen de tilhører. Venstre side prioriteres, med høyre som alternativ.
Pekerne følger lesefargene og vises også i PDF; de endrer ikke diagramlayouten.

P36 legger til en avgrenset `sequenceDiagram`-profil. Se
[forfatterveiledningen](docs/design/mermaid-sequence-authoring.md) og
[versjonert Mermaid-matrise](docs/design/mermaid-coverage.md). Bibliotekets
annonserte diagramtyper er ikke automatisk støttet ende til ende i XFMD.

## Mermaid-galleri (P37–P41)

Start fra build-mappen: `./xfmd ../mermaid_evicence.md`.
[Det praktiske galleriet](mermaid_evicence.md) har 29 eksempler for de 23 familiene
i den låste avhengigheten. Dette er dokumenterte delprofiler, ikke full
JS-Mermaid-kompatibilitet. Ustøttet syntaks gir lokal kildefallback med forklaring.
[Oppdatert matrise og profilveiledninger](docs/design/mermaid-coverage.md) skiller
upstream, fork og faktisk preview/PDF-støtte. Alle familier virker offline.
