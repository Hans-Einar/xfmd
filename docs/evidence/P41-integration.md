# P41 — Brukeraksept og integrasjon

Oppdatert 2026-09-18. Brukeren har åpnet galleriet og vurdert alle diagrammene
som OK, uten behov for justeringer, og har bestilt merge og installasjon.

## Gjeldende kode og verifikasjon

- XFMD: `109b9a96e8da058da3cde4e145c8c6d75dacd9de` på `phase/p41-mermaid-coverage`.
- Fork-pin: `589517a7107ab95ee7dfa5ae88c2f339c0a284dc`.
- Arkiv-SHA256: `e081da327340e05efebef869cb97c8e90246d5dc8f04162279729907c8ff5243`.
- Pinnen har bare rustfmt-endringer etter P41s `c2ccccb`-baseline. Den gamle
  baseline-kontrollen (59 CTests / 419 forktester) står i [P41](P41.md).
- `cargo fmt --all -- --check` består nå. Tidligere CI stoppet på formatering.
- Eldre P38 sanitizer-CI rapporterte lekkasje ved libavoid-porter. P41
  dedupliserer identiske porter; det er ennå ikke påvist her at dette løser
  lekkasjen. Ny Release-/sanitizer-CI kjører på gjeldende pin.
- Lokal Release-build og 59/59 CTests bestått på 86,61 sekunder, inkludert
  25 isolerte GUI-tester. [Logg](p41-mermaid-coverage/integration-ctest.txt).
- Installasjonspakken er kontrollert i en isolert staging-mappe (166 filer).

## Merge og installasjon

Forkens PR #1–5 er merget i den stablede serien, og PR #6 integrerer serien
i hovedbranchen `master`: `afab5e982aead4d4cdec7c88455095733805ed36`.
Ingen åpne PR-er gjenstår i Hans-Einar/mermaid-rs-renderer.
XFMDs PR #24–34 er fortsatt åpne; integrasjon i `main` gjenstår.
Den installerte brukerbinæren er foreløpig ikke erstattet.

Dokumentet oppdateres med endelige commits, CI-resultater og installasjonskontroll
før leveransen avsluttes. Historiske fasebevis beholdes med sine opprinnelige pins.
