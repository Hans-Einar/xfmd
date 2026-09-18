# BoxUI — faktisk verifikasjon

Dato 2026-09-18, Linux/X11, FOX, CMake Release, Rust 1.92.0, isolert Xvfb.
Programcommit **92d0dcf439caf9c193af632e69b3cbc7bb77df0b**, commitnummer 195.
Bibliotek **61a85b670dc1755b52c0fdf82c39497d0fe39512**; se pin og arkivhash i cmake.

- `cmake --build build --parallel 4`: bestått uten compiler-feil.
- `ctest --test-dir build --output-on-failure`: **65/65 bestått**, 284,22 sekunder.
  [Fullt resultat](ctest.txt); [Rust-testutdrag](rust-tests.txt) har 28 beståtte enhetstester.
- Blueprint: 41 objekter, 69 krav, 254 dokumenterte implementerte callee-symboler.
- BoxUiCoreTest: mixed fences, duplikat-ID, ukjent widget, ABI-eierskap/free,
  cancel, tre child-familier, frame-varianter og rendererplassering ved flere bredder.
- BoxUiSessionTest: opt-in, suspend/resume, kontekstkonflikt, stale revisjoner,
  idempotent duplikat/conflict, 4096-grense, ugyldig UTF-8, separate blokker,
  missing/stale/current og gammel sourceSession.
- BoxUiGuiTest: faktiske native FOX-kontroller i Xvfb; Unicode/clipboard, Tab,
  Enter/Escape, utkast ved resize/theme, museaktivering, autorepeat/endrede
  publikasjoner, A4/zoom/scroll og umiddelbar kildeinvalidering.
- BoxUiPdfTest: akseptert C2, simuleringsmerke, omliggende Markdown og ingen
  UNSUBMITTED-DRAFT eller PDF-JavaScript.

[Lys preview](boxui-light.png), [mørk preview](boxui-dark.png) og
[PDF-raster](boxui-pdf.png) er visuelt inspisert: kompakte knapper, synlig C2,
lesbart state-diagram, ingen overlapp eller avkuttet tekst i dette eksemplet.
[PDF](boxui-native.pdf), [statisk SVG](boxui-static.svg) og
[preview-SVG](boxui-preview.svg) beholdes for sammenligning. Preview-knapper er
native; PDF viser bibliotekets statiske knapper og eksplisitt disabled-årsak.

`cmake --install build --prefix /home/warloc/.local` er utført. Installert binær
har samme SHA-256 som testet bygg:
`5334defd1079d4ef511e77a3a3d1a86a3f2f959fcd2da280396b58b999028434`.
Versjon: `0.1 sprint/003/phase/048-boxui-verification:195`.
[Byggmetadata](build.json) identifiserer akkurat dette bygget. Senere dokumentcommits
endrer ikke den installerte byggidentiteten.

## Bevisets grenser

Ingen fysisk IME-, skjermleser/AT-SPI- eller sanitizer-kjøring i denne fasen.
Syntetiske deltakere er lokale og synkrone; ekstern transport, produksjonsbindinger,
SDL-typekontroll og varig exactly-once er ikke levert. Ingen automatisk scenariostart.
Tidsavbrudd er kooperativt. Dette er Implemented med konkrete delbevis, ikke en
påstand om generell Verified-status eller full BoxUI-fremtidsdekning.
