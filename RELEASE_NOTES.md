# Endringsnotater — WIP

## P9–P13: scrolling, sider og publisering

- Edit → Preferences med vedvarende scrollhastighet, valgfri akselerasjon og
  isolert prøvefelt. Samme profil brukes i editor, preview, sidetre og historikk.
- View → A4 page preview eller Window wrap. A4 har justerbare marger,
  fit-width/100 % og bevarer kildeanker ved visningsbytte.
- File → Export PDF / Ctrl+Shift+E eksporterer ulagret buffer i bakgrunnen.
  Samme shaping og paginering som preview; cancel og transaksjonell målfil.
- View → Full Screen / F11, Escape tilbake, med modal dialog først og bevart
  normal/maksimert arbeidsflate i Window Maker.
- Nytt dokument-/Markdown-ikon i FOX-vindu, desktop-entry og installerte størrelser.
- Felles Pango/Cairo-typografi, fysisk layout og presis frame-identitet.
  Færre layoutallokeringer og sortering per linje reduserer ressursbruken.

Skjermtegning bruker en gjenbrukbar bakbuffer med FOX-eid X11-overføring, slik at
hele viewporten tegnes samlet og native ressurser ryddes ved lukking.

P9–P13 endret ikke Markdown-dialekten. Bilde-/HTML-policy og xfw-IPC er uendret.
Fysisk touchpad og fler-monitor hotplug krever kontroll i faktisk desktop-oppsett.
Se [verifikasjonen](docs/evidence/P13.md) og [README](README.md).
Prosjektlisens er ikke valgt; dette er endringsnotater, ikke en formell release.


## P14: tabeller og museklikk

- GFM-tabeller via cmark-gfm, med kolonnejustering, wrapping, header og rammer.
  Samme native layout i preview og PDF; radene holdes samlet ved sideskift.
- Fonttegnet ↗ erstatter nettlenkens håndtegnede sirkel og uønskede forbindelseslinje.
- Rettet musegrab som ble stående etter klikk i preview og kunne blokkere menyer.
  Blandede knapper, drag og foreldet preview aktiverer ikke lenker.

## P15: sidepanelfaner, kapitteltre og referanser

- Files/Index-faner; Index har kapitteltre øverst og References nederst.
- Kapitteltreet bruker aktiv buffers heading-hierarki og oppdateres ved redigering.
- References grupperer lokale Markdown-filer og Hyperlinks. Utvid en fil for å lese
  hovedkapitlene i bakgrunnen. Filene leses ikke rekursivt.
- Enkeltklikk åpner filer/kapitler; Enter aktiverer, piltaster velger. Dirty-dialog,
  tilbake/frem og kildeankre gjenbrukes. Filrotens dobbeltklikk beholder sin funksjon.
- Nettlenker i referansetreet kan åpnes i standardnettleseren uten shell-evaluering.
- Revisjons- og stiavhengig invalidering beskytter mot gamle klikk og referansejobber,
  også ved Lagre som. Refererte kapittelankre finnes på nytt etter filåpning.

## P16: valg av nettleser og Xfe-studie

- Edit → Preferences → Hyperlinks lar deg velge nettleserprogram, inkludert
  `google-chrome-stable`, eller finne et program med Browse. `xdg-open` er standard.
- Preview og referansetreet bruker samme valg for HTTP(S); lenken sendes som ett
  argument uten shell. Feil ved programstart vises uten å endre dokumentet.
- [Xfe-studien](docs/xfe-integration-study.md) kartlegger komponenter, koblinger
  og anbefalt integrasjonsrekkefølge. Xfe-stil er ennå ikke importert.


## P17–P19: FOX UI-lag og Appearance

- Gruppert ikonverktøylinje med samme kommandoer/snarveier, valgt visningsmodus,
  dokumentnavn og vedvarende Light/Dark-toggle. Smale vinduer bruker menyfallback.
- Edit → Preferences har Appearance, Scrolling, Document og Programs.
  Light/Dark, Comfortable/Compact, Flat/Classic og UI-font kan forhåndsvises.
  OK lagrer samlet; Cancel/Escape/close angrer. Lagringsfeil beholder utkastet.
- Sentralt UiContext/ThemeProfiles/IconCatalog, UiButton med utskiftbar painter
  og factory, delte layoutkomponenter og native FOX-input. Ingen ny runtime-motor.
- Files/Index har felles seksjonsutforming; A4/Wrap/zoom ligger ved preview.
- Validerte, valgfrie appearance.ini-overrides med manuell reload og rollback.
  Dokument-/PDF-typografi er uendret av kontrolltemaet.

Se [produksjonsdesign](docs/design/fox-ui-layer.md) og
[testbevis med skjermbilder](docs/evidence/P17-P19.md).
