# FOX UI-lab — separat visuell prototype

Dette er underlag for [UI-forslaget](../../../fox_ui_improvements.md) og
[UX-forslaget](../../../fox_ux_improvements.md), ikke produksjonskode i XFMD.
Ingen endring i XFMDs CMake-targets, installasjon eller brukerinnstillinger.

## Bygg og se en variant

Fra repository-roten, med FOX 1.6, Cairo, X11, pkg-config og C++17-kompilator:

```sh
docs/design/fox-ui-lab/build.sh
/tmp/xfmd-fox-ui-lab docs/design/fox-ui-lab/profiles.ini light workspace
```

Bytt `light` til `graphite` eller `compact`. Siste argument kan være `workspace`,
`reader` eller `controls`; alle profil-/visningskombinasjoner støttes. Profilen
lastes én gang ved oppstart. Endre INI-filen og start denne lille laben på nytt.
Menyene angir at dette er en prototype; lukk med vinduskrysset.

Generer alle fire dokumentasjonsbilder i separate Xvfb-sessioner med midlertidig
HOME, uten å styre brukerens desktop:

```sh
docs/design/fox-ui-lab/capture.sh
# Alternativt en annen utmappe:
docs/design/fox-ui-lab/capture.sh /tmp/xfmd-ui-examples
```

## Hva som faktisk er bygget

- Én profilfil styrer semantiske farger, gap, inset, kontrollhøyde, radius og
  valget mellom egen knappetegning og FOXs ordinære knappetegning.
- `UiButton : FXButton` endrer paint og mål; standard FOX-input beholdes.
  Prototypen velger tegningsgren fra profilen. Det foreslåtte produksjons-API-et
  med ButtonPainter og UiFactory er ikke ferdig implementert her.
- Originale enkle linjeikoner tegnes med FXDCWindow. Ingen Xfe-ressurser er importert.
- Menyer, faner, splittere, trær, editor og felter er native FOX. TreeItem gir
  ekstra radhøyde. Dokumentillustrasjonen bruker bare FOX-tegning og FOX-fonter.
- Cairo brukes kun til å skrive skjermopptaket til PNG, ikke til kontrolltegningen.
- Lys/mørk bruker samme workspace-bygger og fixture. Kompaktbilde viser reader
  for å illustrere UX, men `compact workspace` er også et gyldig valg.

## Hva prototypen ikke beviser

Open/Save, navigasjon, filter, modusknapper og Appearance-feltene er ikke koblet
mot XFMD. Faner, tekstfelt og treutvidelse har FOX sin vanlige grunnatferd, men
produserer ingen dokumentarbeidsflyt. Dokumenttekst og status er statiske;
editorredigering oppdaterer ikke illustrasjonen. Kontrollprøvens Hover/Pressed/
Focus er tvungne visuelle eksempler, ikke opptak av faktiske inputsekvenser.

Laben tester ikke lagring av tema, live-reload, rollback, skaleringsbytte eller
all tastaturatferd. Den har ikke produksjonens scroll- og kildeankerkode.
Den statiske dokumenttegningen er dimensjonert for eksempelflatene; responsiv
Markdown-layout må fortsatt leveres av XFMDs eksisterende renderer.

## Gjennomført kontroll, 2026-09-14

Bygget med systemets FOX 1.6.57 og `-Wall -Wextra`, uten kompilatoradvarsler.
Alle fire varianter kjørte til vellykket PNG-opptak under Xvfb.
En egen `--check-close`-kjøring sendte FOXs close-melding via timer og avsluttet
med status 0; labvinduet beholder eksplisitt eierskap for å unngå dobbel sletting. Bildene ble
inspisert for lesbarhet, overlapper, palett, valgt modus og knappeprøvene.
Eksplisitt `app.repaint()` før opptak tømmer pending paint-kø; bare en tidsventing
var ikke tilstrekkelig for et komplett første bilde.

Prosjektets blueprint-/symbol-/laggrensekontroller og lokale dokumentlenker er
kontrollert separat. Ingen produktkode er endret, og ingen bred ny testpakke er
opprettet for denne illustrative prototypen. Forslagene beskriver testkravene ved
senere produksjonsintegrasjon.
