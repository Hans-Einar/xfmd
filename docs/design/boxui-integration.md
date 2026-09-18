# Interaktiv BoxUI i XFMD

Implementasjon autorisert 2026-09-18. XFMD er native FOX-vert, uten HTML,
nettleser, Node eller dokumentstyrt kodekjøring. Krav: UR-043 / SR-026;
feature FTR-011, functionality FUNC-027–030. Se [eksemplet](../../boxui_evidence.md)
og [forfatterveiledningen](boxui-authoring.md).

## Produsent og kontrakt

Felles grense er `BX-HOST/0.1-draft1`, profile `boxui/0.1`, med separate
parse- og prepare-operasjoner. `cmake/mermaid-source.json` låser biblioteket
og arkivhashen. Integrasjonspinnen er `61a85b670dc1755b52c0fdf82c39497d0fe39512`
fra forkens `phase/boxui-046-implementation`. Den foreløpige, overlappende
`phase/boxui-045-core` inngår ikke i leveransen.

Godkjent designgrunnlag er forkcommit `54aee4e685a4454f2233060341336446d58054a9`.
Kontraktfilens daværende SHA-256 var
`9eb0b629287349e4e2f849d0d3ab9f61b1e52d8fce64de1917e4c2b26d8a1a35`.
Produsentens SDP-08-03 angir realisert API og presiseringer; de seks draft1-skjemaene
ble beholdt. XFMD får ingen SDP-mappe eller autoritet til å vedta SDL-begreper.

## Faktiske eiere og kallretning

| Eier | Filer / ansvar |
| --- | --- |
| contracts/boxui | BoxUiModel, BoxUiFrame, IBoxUiInterpreter, IBoxUiLayout, BoxUiAbi; FOX-/Rust-/JSON-frie offentlige verdier |
| contracts/boxui/private | BoxUiCodec: skjult wire-serialisering og resultateierskap; nlohmann/json 3.12.0, MIT, vendret uendret |
| interpreter/boxui | BoxUiBlockBuilder velger gjerde; BoxUiInterpreter bruker parse-ABI og tolker childSources gjennom IDiagramInterpreter |
| renderer/boxui | BoxUiLayout bruker prepare-ABI med normalisert modell, snapshots og ferdige child SVG-er; BoxUiPlacement plasserer én visual-run |
| application/boxui | BoxUiPreparation forbereder typed diagram-barn og BoxUI på arbeider; BoxUiSession eier identitet, state og ledger; SyntheticActivity er lokal testdeltaker |
| application/adapters | FoxBoxUiOverlay eier native felt/knapper; FoxBoxUiInput håndterer Enter/Escape og press/release; eksisterende DiagramPainter/librsvg tegner SVG |
| application/composition | DiagramServices registrerer parseren og prepareringskjeden; mermaid/src/boxui.rs eier separate C-innganger og panic-/buffergrensen |

Biblioteket eksponerer `parse_boxui_bytes`, `decode_prepare_json` og
`prepare_boxui_cancellable`. XFMD gjenimplementerer ikke parsing eller plasseringsmotor.
SharedTextMetrics leverer bredde, høyde og baseline i SVG-piksler. Innebygde
flowchart-, sequence- og state-diagrammer tolkes før layout; renderer får aldri
Mermaid-kilde som skjult input. Child-feil begrenses til sin rute.

## Publisering og interaksjon

Eksisterende ParserWorker gir én aktiv og én utskiftbar ventende forberedelse;
både arbeidsticket og DocumentToken sjekkes. PreviewCoordinator/RenderHost eier
publisering, så en ekstra BoxUiCoordinator er unødvendig. BoxUiSession sjekker
source/epoch/binding/state/kontekst og aktuell enabled/type før dispatch.

View → BoxUI prototype aktiverer en lokal, syntetisk deltaker uttrykkelig.
Kildeendring stopper økten. Den lagres ikke mellom omstarter. Hver blokk får egen
Activity A1; suspend/resume beholder fremdrift, mens kontekstendring gir avvist
resume. Ingen tilstandsmaskin evalueres fra det innebygde stateDiagram.

Native felt beholder draft/caret/fokus ved kompatibel resize/theme; Enter sender,
Escape gjenoppretter akseptert verdi, blur/save sender ingenting. Endret kilde,
binding eller kontekst forkaster inkompatible drafts. Endret Value-revisjon
overskriver ikke et skittent felt: innsending avviser konflikt, Escape tar siste verdi.
Native knapper bruker det samme kontrollkartet og FOX-fokus/tastatur; PDF bruker
bibliotekets SVG-knapper. Dette konkretiserer draftens SVG/semantiske knapper.

Kommandoer fullføres synkront av den lokale deltakeren; ingen ubundet bakgrunnskø
opprettes. Ledger beholder opptil 4096 ID-er, returnerer identiske duplikater og
avviser konflikt/full ledger. Det er ingen restartbestandig exactly-once-garanti.

## Geometri, tema og PDF

SVG px → 0,75 renderer-punkt → dokumentplassering → viewport/zoom/scroll er én
felles transform. Native klipp følger kontroll, blokk og synlig previewflate.
Host tilbyr minst 320 × 640 px for målte widgets; plassmangel blir lokal diagnose.
Farger følger eksisterende lesepalett i lys/mørk modus uten ny tolkning.

PDF fanger en immutable akseptert state på GUI-tråden og forbereder statisk SVG
på eksportarbeideren. Drafts og native events inngår ikke. Simulert state merkes
`simulated snapshot`. Vanlig Markdown og diagrammer beholder eksisterende eksportvei.

## Grenser og bevis

256 KiB kilde, 256 noder, dybde 16, åtte child-diagrammer à 64 KiB, 4096 Unicode-
skalarer per tekstinput, 8 MiB frame/prepare og 64 MiB aktive visualressurser per
Markdown-dokument. Tidsbudsjett 2000 ms (10000 ved sanitizer) er kooperativt;
en pågående callback eller diagramoperasjon kan ikke avbrytes hardt.

Fase 046 og 047 har konkrete core/session/GUI-bevis. Sluttkontroll, skjermbilder,
PDF- og regresjonsresultater registreres i [sprinten](../../sprints/Sprint-003--Interactive-BoxUI/README.md).
Status er Implemented, ikke generell Verified. Fysisk IME, AT-SPI, ekstern transport,
produksjonsbindinger og SDL-kodegenerering er ikke bekreftet av disse testene.
