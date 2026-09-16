# Mermaid: bibliotekets SVG og utskiftbar ruting

Status: Ready for implementasjon, 2026-09-16. Denne beslutningen erstatter
native diagramtegning og studiens foreslåtte egen rutemotor. P25–P30-bevis
beholdes som historikk. P31 endrer bare presentasjon; P32 bytter bibliotekpin
og velger libavoid etter sammenligning i forken.

## Ansvar og kontrakter

- Interpreter beholder Mermaid → ren DiagramModel. Ingen renderer-avhengighet.
- `MermaidDiagramLayout` sender modell og målte etiketter til Rust-layout.
  Bibliotekets `render_svg` lager ferdig SVG fra samme Layout, uten ny parsing.
- DiagramScene eier SVG-bytes, størrelse, font-ID og diagnostikk. Eksisterende
  geometri beholdes som inspeksjonsdata; den brukes ikke til å tegne diagrammet.
- `DiagramPlacement::append` lager én VisualResource-run med Approximate
  kildeanker og bevart aspekt. Ingen separate Pango-DrawRuns for etiketter.
- Application-adapteren `DiagramPainter::paint` bruker librsvg til å tegne SVG
  direkte til Cairo. Samme vei brukes av preview og PDF; ingen rasterisering.
  Et lite trådlokalt, begrenset cache eier RsvgHandle. Ingen handle i contracts.

Layout-record får eksplisitt payload-versjon 2 under C-ABIens eksisterende
versjon 1-envelope. Uforenlige records avvises. SVG har 8 MiB-resultatgrense.
Bare bibliotekgenerert SVG fra den begrensede parserprofilen aksepteres;
Mermaid-kilden får ikke HTML, script, CSS, eksterne ressurser eller URL-tilgang.

## Tekst og farger

SVG bruker DejaVu Sans 16 px, tilsvarende målingens 12 pt etter 0,75-skalering.
Librsvg utfører SVG-tekst; XFMD oppretter ikke egne tekstfelt inne i diagrammet.
Etikettmerking er utsatt etter brukerens prioritering. Vanlig Markdown-tekst
kan fortsatt merkes og kopieres. Diagrammet beholder kildeanker; PDF beholder
vektorgeometri og SVG-tekst så langt librsvg/Cairo støtter tekstuttrekk.

Light/Dark og leseslidere endrer bare et kontrollert programgenerert CSS-
stylesheet som mapper bibliotekets standardpalett til ReadingPalette.
Ingen ny parsing av Mermaid, layout eller dokumentrevisjon. Egendefinert Mermaid
CSS/RGB er fortsatt utenfor eksisterende parserprofil; SVG-broen begrenser ikke
en senere eksplisitt utvidelse.

## Libavoid (separat P32)

Rutepipelinen bor i Hans-Einar/mermaid-rs-renderer, aldri i `.deps`-patcher.
`layout::routed::Engine` er uavhengig av nodeplasseringsmotoren. Legacy beholdes
som eksplisitt sammenligningsvalg. Feil gir lokal kildefallback med årsak;
ikke automatisk, skjult Legacy-ruting. Libavoid har kooperativt avbrudd,
endelig svingkostnad, null kryssingsstraff og bibliotekets egen nudging.

Forken eier C++-binding, pin, lisenskilder, portvalg, avgrensede etikettpass,
validering og valgfrie SVG-hopp. XFMD eier modellgrense, måledata, arbeidsbudsjett
og publisering av resultatet. Måleseam/checkpoints flyttes til forken som egen
commit før pin-endringen. Låst commit og SHA-256 skal dokumenteres i bootstrap.

## Verifikasjon og leveranse

P31: bygg og CTest med gammel ruter; native Light/Dark/slider, A4/resize, PDF,
Unicode og manglende separate etikett-runs. P32: samme kontroller med libavoid,
obligatorisk sporbarhetsgraf, øvrige brukerfixtures, feil/budsjett og diagnose.
Forken leverer frosne posisjoner/tekst før–etter, separat ende-til-ende,
SVG/PNG, metrikk-JSON og kjente begrensninger. Ingen generell påstand om
optimalitet eller hard tidsfrist. LGPL-kilder, lisens og relink-oppskrift
følger eventuell binærdistribusjon.

## P31 verifikasjon 2026-09-16

Implementasjon `d789d54`: Release-bygg og seriell CTest: **59/59 bestått**
(122 sekunder). GUI kjørt isolert i Xvfb. PDF-test verifiserer vektorinnhold,
uttrekkbar Unicode-tekst og ingen rasterbilder. Tema/fargeslidere, A4, zoom,
vanlig Markdown-tekstkopiering, ugyldig SVG og cache er dekket. Parallelle
kjøringer under byggeaktivitet traff gammel ruters budsjett; seriell fullkjøring
bestod uten å øke fristen. Begge blueprint-validatorer består.

Denne milepælen beholder upstream-pin og gammel ruting. SVG-omleggingen er
ferdig før P32 bytter rutemotor; diagrametiketter er ikke separate Pango-felt.
