# P41 — Brukeraksept og integrasjon

Oppdatert 2026-09-18. Brukeren har åpnet galleriet og vurdert alle diagrammene
som OK, uten behov for justeringer, og har bestilt merge og installasjon.

## Gjeldende kode og verifikasjon

- XFMD integrert i `main`: `4008679311bc9d78e7f92fdb6bff63943b471eb7`.
  Verifisert kodecommit før dokumentoppdatering/merge: `109b9a96e8da058da3cde4e145c8c6d75dacd9de`.
- Fork-pin: `589517a7107ab95ee7dfa5ae88c2f339c0a284dc`.
- Arkiv-SHA256: `e081da327340e05efebef869cb97c8e90246d5dc8f04162279729907c8ff5243`.
- Pinnen har bare rustfmt-endringer etter P41s `c2ccccb`-baseline. Den gamle
  baseline-kontrollen (59 CTests / 419 forktester) står i [P41](P41.md).
- `cargo fmt --all -- --check` består nå. Tidligere CI stoppet på formatering.
- [CI 35320008006](https://github.com/Hans-Einar/xfmd/actions/runs/35320008006)
  består på kodecommit `109b9a9`: OFF **59/59** (140,91 s), ON **57/57**
  (154,06 s), inkludert CI-installasjon. Den eldre
  P38-lekkasjerapporten ved libavoid-porter forekommer ikke i denne kontrollen.
  P41 dedupliserer identiske porter; ingen lekkasjesuppresjon er lagt til.
  Senere endringer gjelder kun dokumentasjon og testlogger.
- Lokal Release-build og 59/59 CTests bestått på 86,61 sekunder, inkludert
  25 isolerte GUI-tester. [Logg](p41-mermaid-coverage/integration-ctest.txt).
- Installasjonspakken er kontrollert i en isolert staging-mappe og installert
  atomisk per fil under `~/.local` (167 filer, binæren sist). Alle installerte
  filer er bytekontrollert mot staging. `xfmd --version` består.

## Merge og installasjon

Forkens PR #1–5 er merget i den stablede serien, og PR #6 integrerer serien
i hovedbranchen `master`: `afab5e982aead4d4cdec7c88455095733805ed36`.
Ingen åpne PR-er gjenstår i Hans-Einar/mermaid-rs-renderer.
XFMDs PR #34 ble retargetert til `main` og merget med hele faseseriens commits.
#24 ble automatisk markert merget. #25–33 ble lukket etter at hvert head-SHA
var kontrollert som ancestor til `origin/main`; ingen fasecommits er forkastet.
Ingen åpne PR-er gjenstår i Hans-Einar/xfmd.

Installert binær: `~/.local/bin/xfmd`, identisk med `build/xfmd`.
SHA-256: `ddc912f1a270ab6bd4cd2244965a9cb324c967c00cba0b16b4761014b1ea1295`.
Backup: `~/.local/state/xfmd/install-backups/20260918-2312ee095684/`.
Kjørende brukervinduer ble ikke avsluttet; nye oppstarter bruker den nye binæren.
Versjonsstrengen er fortsatt `xfmd 0.1.0`; pin og binærhash identifiserer denne leveransen.

[Støttematrisen](../../mermaid_coverage.md) ligger nå i repo-roten ved siden av
`mermaid_evicence.md`. Historiske fasebevis beholder sine opprinnelige pins.
Etterfølgende dokumentasjonscommits endrer ikke den verifiserte programkoden.

