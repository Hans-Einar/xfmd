# Mermaid: bibliotekets SVG og utskiftbar ruting

Status: P31 verifisert; P32 implementert, samlet verifikasjon pågår, 2026-09-16. Denne beslutningen erstatter
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

## P32 kjøretidsvalg og begrensninger

Payload 3 legger til eid diagnostikk etter SVG. Miljøvalg settes før oppstart:
`XFMD_MERMAID_ROUTER=libavoid` (standard) eller `legacy`, og
`XFMD_MERMAID_CROSSING_JUMPS=0` (standard) eller `1`. Ugyldige verdier gir
blokklokal feil. Cacheidentiteten inkluderer begge valg.

Forken tillater maksimalt tre etikett/rutetransaksjoner per plassering og én
eksplisitt, diagnostisert plassering med større avstand ved plassmangel.
Sammenligning med låste posisjoner bruker aldri denne utvidelsen. Resterende
parallelle nærføringer rapporteres; null kryssinger er ikke et mål i seg selv.

Native admission: mer enn 256 forbindelser eller 2048 kandidatporter avvises
før C++-transaksjon. Parserens modellgrense kan være høyere; en gyldig modell
kan derfor få en eksplisitt layoutdiagnose. Dette beskytter mot observert lang
nudging på 128 noder / 512 kanter. 128-noders kjede støttes. Grensen erstatter
ikke kooperativ frist og innebærer ingen hard tidsisolasjon.

P32-pin: `6ff5a54ce39d912e399493ec4639a7b8ff8bdc7f`. Forkens dokumentasjon og
før/etter-SVG/PNG ligger i `docs/libavoid-review.md` og `docs/libavoid-preview/`
på samme commit. Arkivkontrollsummen står i `cmake/mermaid-source.json`.

Ubuntu-regresjonen bruker et siste etikett-only-pass på ferdige ruter, uten
flere native transaksjoner. En frosset Pango 1.52.1-fixture i forken reproduserer
den tidligere feilen. Self-loops bruker distinkte retningsbundne grensepunkter
i libavoid, siden virtuelle shape-pin-endepunkter kunne kollapse til nullrute.

SVG-cacheoppryddingen frigjør også Pango sitt per-tråd standardfontkart; se
[P31-livsløpsbevis](../evidence/P31-lifecycle.md). Ingen nye LSan-suppresjoner.
