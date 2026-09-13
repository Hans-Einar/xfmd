# XFMD som del av Xfe-familien

Dato: 2026-09-13. Status: kildebasert analyse og anbefaling; Xfe-komponentene
beskrevet som foreslåtte er ikke importert i XFMD. Nettleservalget i P16 er en
separat, implementert endring.

## 1. Abstract

XFMD har riktig teknisk fundament for å bli en Xfe-companion, men deler ennå
for lite av Xfe sitt presentasjonslag. FOX alene gir ikke samme utseende: Xfe
har egne komponenter, felles ikonlasting og omfattende tilpasninger av FOX.
Disse ligger i applikasjonskildene, ikke i et separat, støttet toolkit-SDK.

Anbefalingen er å gjenbruke Xfe sine temaer, handlingsikoner og utvalgt
tegningskode gjennom et lite, eksplisitt UI-lag. Behold XFMDs dokumentmodell,
asynkrone filindeks, scrolling og renderer. Bruk særlig teksteditoren **xfw**
som referanse for menyer, verktøylinje og dialoger. En sammenslåing med hele
Xfe-filbehandleren er ikke nødvendig for å oppnå familiens uttrykk.

## 2. Undersøkt grunnlag

| Kilde | Identitet | Hva den dokumenterer |
| --- | --- | --- |
| XFMD | P15-baseline `6115c0c`, P16-arbeid | FOX-vinduer og egne adaptere; cmark-gfm og Pango/Cairo i dokumentvisningen |
| Xfe upstream | `aaa3559eeb4ccfd1e0ce32fbc4ac0198385d4fdc`, Xfe 2.1.11 | Faktiske komponenter, byggeorganisering og FOX-tilpasninger |
| Vår Xfe-fork | `e78e9c6f2258337fac6c9257246f14d0be6a61ce` | Samme UI som upstream; forskjellen er quoting av automount-stier |
| Lokal installasjon | `/usr/bin/xfe`, versjon 2.1.11; FOX 1.6.57 | Relevant målmiljø, Xfe-konfigurasjon og installerte ikoner |

Forken ligger i `~/git/Linux/subrepos/xfe`. Upstream ble sammenlignet fra en
separat checkout. Analysen bygger på kildekode og konfigurasjon, ikke på en
ferdig utprøvd uttrekking av et bibliotek. Filstørrelsene nedenfor er omtrentlige
og gjelder denne versjonen. [Byggelisten][build] viser felles kildefiler kompilert
inn i `xfe`, `xfa`, `xfi`, `xfp` og `xfw`; den tilbyr ikke et installert `libxfe-ui`.

## 3. Hvor native er XFMD i dag?

| Område | XFMD i dag | Xfe / xfw |
| --- | --- | --- |
| Vinduer, menyer, dialoger | FOX og lokale subklasser | FOX, egne subklasser og redefinerte FOX-metoder |
| Sidepanel | FXTabBook, egne trær og dokumenthandlinger | DirPanel, DirList, Places og filbehandlerhandlinger |
| Faner | FOX-faner for Files/Index | FOX-fanetegning og en egen TabButtons for katalogfaner |
| Ikoner / stil | Eget appikon, hovedsakelig standardkontroller og tekstknapper | Felles ikonsett, konfigurerbare farger, fonter og avrundede kontroller |
| Dokumenttegning | cmark-gfm → egen layout → Pango/Cairo → X11-flate i FOX | xfw bruker FOX-teksteditor; ingen tilsvarende Markdown-/PDF-motor |
| Innstillinger | Egen FOX-registry under `~/.foxrc/xfmd` | Xfe-konfigurasjon under XDG-config, via endret registry-kode |

XFMD har altså et native FOX-grensesnitt, men dokumenttypografien er ikke bare
FOX-fonttegning. Pango/Cairo gir shaping, fontfallback og felles PDF-/previewlayout.
Dette kan beholdes uendret når vinduets kontroller får Xfe-stil. Ingen nettlesermotor,
GTK-overgang eller ekstra bakgrunnsprosess er nødvendig.

## 4. Hva Xfe faktisk har lagt oppå FOX

### 4.1 Felles kontrolltegning

[moderncontrols.cpp][modern] er omtrent 4 400 linjer og endrer blant annet
FXButton, FXCheckButton, FXTextField, FXMenuCommand, FXTreeList, FXTabItem og
FXScrollBar. Dette er ikke en samling nye klasser med et offentlig API:
[XFileWrite.cpp][xfw] inkluderer filen direkte og leverer nye definisjoner av
metoder i eksisterende FOX-klasser.

Dermed kan mye av det ønskede uttrykket hentes fra Xfe uten å erstatte våre
trær og fanemodeller. Men filen er heller ikke rent kosmetisk: den redefinerer
blant annet `FXScrollBar::setPosition`. Farger caches i statiske variabler,
og skalering bruker global `scaleint`. Direkte inkludering ville innføre globale
forutsetninger og koble oss til samme FOX-versjon og symboloppløsning.

### 4.2 Endret FOX-atferd

[foxhacks.cpp][hacks], omtrent 3 800 linjer, endrer registry, tekstmåling,
popup/fokus, hendelsesdispatch, scrolling, clipboard og deler av tastaturhåndteringen.
Det er en viktig del av Xfe, men ikke et trygt stiltema som kan slås på isolert.

XFMD har nylig rettet musegrab og implementert egen scrollmodell. Å importere
hele filen kan endre disse kontraktene, samt flytte alle våre innstillingsfiler.
Også [DirPanel.cpp][dirpanel] redefinerer `FXTreeList::onKeyPress` globalt for å
skille piltastnavigasjon fra klikk. Vi løser dette lokalt i navigasjonstreet.
Her bør vi sammenligne løsningene og gjenbruke kun det vi faktisk trenger.

### 4.3 Egne widgets og applikasjonskomponenter

| Komponent | Vurdering for XFMD | Foreslått gjenbruk |
| --- | --- | --- |
| Menyer / verktøylinje i [WriteWindow][write] | Samme grunnleggende FOX-kontroller som hos oss | Xfw-rekkefølge, grupper, padding, ikoner og snarveikonvensjoner; behold egne kommandoer |
| FXTabItem / FXTreeList-tegning i moderncontrols | Godt samsvar med Files/Index og kapitler/referanser | Uttrekk til eksplisitte subklasser eller delte tegningsfunksjoner |
| [TabButtons][tabs], ca. 600 linjer | Katalogfaner med FilePanel-/DirPanel-avhengighet, filstier, lukking og omrekkefølge | Ikke direkte erstatning for sidepanelmoduser; skill ut generisk presentasjon bare ved behov |
| [DirList][dirlist], ca. 2 700 linjer implementasjon | Filtre, filassosiasjoner og filesystemtilstand; mer enn et tre | Ikoner og visuelle konvensjoner først; behold vår DirectoryScanner og begrensede arbeidskø |
| DirPanel, ca. 4 600 linjer | Filoperasjoner, Places, mounts, dialoger og globale applikasjonsobjekter | Ikke importer panelet samlet |
| DialogBox, ca. 230 linjer | Relativt liten, men egen modal- og tastaturatferd; arver FXTopWindow | Egnet kandidat etter eksplisitt test av Escape, Enter, fokus og eiervindu |
| FileDialog / FileSelector | Mange følgeavhengigheter, inkludert FileList og PathLinker | Egen senere fase; ikke nødvendig for første visuelle løft |
| ComboBox, TextLabel, PathLinker | Nyttige enkeltkomponenter, varierende kobling til filbehandleren | Velg etter konkret behov; unngå å importere et helt rammeverk på forhånd |

## 5. Tema og ikoner: størst gevinst først

Xfw leser blant annet Xfe sine `basecolor`, `bordercolor`, `screenres` og font før
vinduet bygges. [Ikonlastingen][icons] bruker `SETTINGS/iconpath`, skalering og
installasjonsrelativ fallback. Hos oss peker Xfe på `/usr/share/xfe/icons/default-theme`.

Lag en **foreslått** `XfeAppearanceSource` som leser et avgrenset sett av Xfe-felter
til en egen verdi: palett, kontrollfont, ikonsti og skala. Bruk eksplisitt parsing
av system-/brukerkonfigurasjon; ikke redefiner `FXRegistry` for hele prosessen.
Les Xfe-innstillingene uten å skrive til dem. XFMDs egne dokument- og
nettleserinnstillinger skal fortsatt lagres i XFMDs konfigurasjon.

En **foreslått** `XfeIconCatalog` skal laste bare ikonene vi bruker, ikke alle
globale ikonvariabler fra `icons.cpp`. Konkrete navn finnes allerede:
`minifileopen.png`, `miniprefs.png`, `minidirback.png`, `minidirforward.png`,
`minifolder.png` og `minifolderopen.png`. Bruk dokumenterte fallback-ikoner hvis
Xfe eller et ikon mangler. Behold XFMDs eget applikasjonsikon.

Start med oppstartsinnlesing av tema; live-synkronisering med en annen prosess
er et eget behov. Bruk eksplisitte palettverdier fremfor upstreams statiske
førstegangscache hvis kontroller senere skal støtte temabytte.

Utseende og atferd må ha tydelig prioritet: vårt bestilte enkeltklikk skal gjelde
selv om Xfe er konfigurert med `single_click=0`. Egen scrollprofil og nettleservalg
skal ikke overskrives av Xfe-felter. Kontrollfonten kan følge Xfe; dokumentfont,
A4-geometri og PDF-layout må fortsatt styres av dokumentpresentasjonen.

## 6. Anbefalt arkitektur og vedlikehold

Opprett et avgrenset, **foreslått** `xfe_ui`-byggemål for utvalgte kontroller og
tegningsfunksjoner. Den langsiktige kilden bør være vår Xfe-fork, med pinned
revisjon og bevarte upstream-henvisninger. XFMDs appearance-adaptere og
kommandobindinger blir i `application`; interpreter og renderer skal ikke kjenne Xfe.

Begynn med små, eksplisitte subklasser og FOX-eierskap. Ikke importer hele
`moderncontrols.cpp` eller `foxhacks.cpp` som globale metodeerstatninger.
Et ekte delt bibliotek bør få en andre konsument, eksempelvis xfw, før vi
lover et generelt API. Uttrekket er da en forbedring i Xfe-forken, mens XFMD
forbruker en avgrenset del uten å måtte bygge filbehandleren.

Alternativet med bare å etterligne Xfe uavhengig gir rask første likhet, men to
vedlikeholdte tegningsimplementasjoner. En full fork av xfw gir på sin side mye
irrelevant editor-/filkode og svekker våre allerede etablerte kontrakter.
Selektivt kildegjenbruk med tydelig opphav er den beste mellomveien her.

Det forventes ikke en vesentlig løpende renderkostnad ved denne tilnærmingen,
men det er en vurdering, ikke et måleresultat. Mål oppstart, RSS og scrolling
før/etter; cache skalerte ikoner og unngå konfigurasjonslesing i paint-handlere.

## 7. Foreslått integrasjonsrekkefølge

Dette er et estimert videre løp, ikke gjennomført arbeid eller en ny godkjent
implementasjonsfase. Estimatene er utviklerdager og inkluderer korte reviews og
relevante regresjonstester; funn ved uttrekk kan flytte grensene.

| Fase | Milestones med egen commit | Anslag / ferdigkriterium |
| --- | --- | --- |
| P17: Xfe-utseende | M1 appearance-leser og fallback; M2 ikonmapping og xfw-lignende meny/verktøylinje; M3 visuell kontroll og dokumentasjon | 1–2 dager. Samme tema og handlingsikoner som installert Xfe, fungerer også uten Xfe |
| P18: kontrollfamilie | M1 avgrenset uttrekk med opphav og byggetarget; M2 tab-/tre-/knapptegning og sidepanelspacing; M3 input-/scroll-/skaleringstester | 3–5 dager. Synlig Xfe-familie, uten globale FOX-overrides |
| P19: dialoger og desktop | M1 dialogmønster og Preferences; M2 filvelger etter avhengighetsgjennomgang; M3 Markdown-filassosiasjon og åpning fra Xfe | 2–4 dager, med større usikkerhet for komplett FileDialog |
| Valgfri fellespublisering | Andre konsument i Xfe-forken, isolert bygg, upstream-forslag og vedlikeholdsnotat | 2–5 ekstra dager avhengig av omfang; ikke nødvendig for første visuelle leveranse |

Hver implementasjonsfase bør følge eksisterende phase-branch/PR-praksis.
Definer krav og blueprints først; gjenbruk eksisterende workspace-, preferences-
og navigasjonsansvar fremfor å lage nye features for hver widget.

Testene må bevare dirty-dialog/cancel, enkeltklikk kontra tastatur, tilbake/frem,
lazy referanser, blandede museknapper, scrollendepunkter, Escape i modal dialog
og fullskjerm. Visuell kontroll bør dekke lyst/mørkt tema og 100/150/200 % skala.
Test manglende/ugyldig Xfe-konfigurasjon og manglende ikoner. PDF-layout og
kildeankre skal være uendret av kontrolltemaet.

## 8. Opphav og avgrensninger

[Xfe beskriver GPL-2.0-or-later og ikonopphav fra GNOME/KDE/XFCE][readme].
Enkelte FOX-avledede filer har egne lisensmerknader. Ved faktisk kopiering må
vi bevare opphav og lisens per fil; XFMD har ennå ikke valgt prosjektlisens.
Valgte ikonressurser trenger tilsvarende sporbarhet før de pakkes med XFMD.
Å bruke installert Xfe-ikonsti med fallback er et naturlig første steg.

Denne analysen kopierer verken Xfe-kode eller ikoner inn i produktet. Ingen
prototype beviser ennå alle uttrekksgrenser. Det sikreste første leveransepunktet
er derfor tema/ikoner/menystruktur, etterfulgt av en liten kontrollfamilie med
tester, fremfor et løfte om umiddelbar kompatibilitet med alle Xfe-widgets.

## 9. Konklusjon

Brukerens forventning om et Xfe-preg er ikke fullt oppfylt av dagens standard-FOX-
kontroller. Vi kan rette dette uten å endre Markdown-motoren. Xfe har allerede
mye av det visuelle grunnlaget vi trenger; jobben er å gjøre den relevante delen
gjenbrukbar og koble den til XFMDs eksisterende arbeidsflyter.

Neste anbefalte implementasjon er **P17: Xfe-tema, Xfe-handlingsikoner og
xfw-lignende menyer/verktøylinje**, med Files/Index og dokumentrendering intakt.

[build]: https://github.com/roland65/xfe/blob/aaa3559eeb4ccfd1e0ce32fbc4ac0198385d4fdc/src/Makefile.am
[modern]: https://github.com/roland65/xfe/blob/aaa3559eeb4ccfd1e0ce32fbc4ac0198385d4fdc/src/moderncontrols.cpp
[hacks]: https://github.com/roland65/xfe/blob/aaa3559eeb4ccfd1e0ce32fbc4ac0198385d4fdc/src/foxhacks.cpp
[xfw]: https://github.com/roland65/xfe/blob/aaa3559eeb4ccfd1e0ce32fbc4ac0198385d4fdc/src/XFileWrite.cpp
[write]: https://github.com/roland65/xfe/blob/aaa3559eeb4ccfd1e0ce32fbc4ac0198385d4fdc/src/WriteWindow.cpp
[tabs]: https://github.com/roland65/xfe/blob/aaa3559eeb4ccfd1e0ce32fbc4ac0198385d4fdc/src/TabButtons.h
[dirpanel]: https://github.com/roland65/xfe/blob/aaa3559eeb4ccfd1e0ce32fbc4ac0198385d4fdc/src/DirPanel.cpp
[dirlist]: https://github.com/roland65/xfe/blob/aaa3559eeb4ccfd1e0ce32fbc4ac0198385d4fdc/src/DirList.h
[icons]: https://github.com/roland65/xfe/blob/aaa3559eeb4ccfd1e0ce32fbc4ac0198385d4fdc/src/icons.cpp
[readme]: https://github.com/roland65/xfe/tree/aaa3559eeb4ccfd1e0ce32fbc4ac0198385d4fdc#readme
