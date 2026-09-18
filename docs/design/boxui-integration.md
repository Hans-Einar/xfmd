# BoxUI i Markdown — separat XFMD-prosjekt

Status: Implementasjon autorisert av brukeren; felles kontraktgrunnlag godkjent. Opprettet 2026-09-18.
Brukeren har godkjent oppstart og hele integrasjonsløpet i én økt.
Arbeidet følger fasebrancher, milestone-commits og ett planlagt bygg per fase.

## Formål og felles grunnlag

XFMD skal være første interaktive vert for BoxUI-widgeter i `.md`-filer.
Dette er en native FOX-integrasjon, ikke en HTML-side, CSS-motor eller nettleser.
Forkens SDP-prosjekt eier den delte kontrakten `BX-HOST/0.1-draft1`, under
`SDP/06--Container-Design/06-02--XFMD-Host-Contract.md` på
`Hans-Einar/mermaid-rs-renderer`, branch `feature/boxui-extension`.
Lokalt finnes den i søster-worktree `../mermaid-rs-renderer-boxui/`.
Eksakt godkjent kontraktcommit og SHA-256 skal festes her ved parallellstart;
kontrakten er godkjent som implementasjonsgrunnlag av brukeren.

Les forkens SDP-12-01 for åpne spørsmål, SDP-08-02 for gate og SDP-09-03 for
akseptansekatalog. XFMD beholder egne krav/blueprints/sprinter; ingen SDP-migrasjon.

## Ansvar og planlagte kildefiler

| Eier | Planlagt rolle / plassering | Gjenbruk |
| --- | --- | --- |
| contracts | `boxui/BoxUiModel.h`, `BoxUiFrame.h`, `IBoxUiInterpreter.h`, `IBoxUiLayout.h`, `BoxUiAbi.h` | FOX-/Rust-frie verdier, separate parse/layout-porter |
| interpreter | `boxui/BoxUiBlockBuilder.cpp`, `BoxUiInterpreter.cpp` | Cmark ModelBuilder; nytt BoxUI-blokkvalg og parserinjeksjon, vanlig Mermaid beholdes |
| renderer | `boxui/BoxUiLayout.cpp`, `BoxUiPlacement.cpp` | tekstmål, dokumentplassering/transform, SVG-run og hit-geometri |
| application | `boxui/BoxUiCoordinator.cpp` | PreviewCoordinator/worker: framepublisering, bindingsrevisjon og currentness |
| application | `boxui/BoxUiSession.cpp`, `BoxUiCommandLedger.cpp` | per-blokk identitet, kontrollert livstid, deduplisering og køgrenser |
| application | `adapters/FoxBoxUiOverlay.cpp`, `FoxBoxUiInput.cpp` | native FXTextField, fokus, clipboard, clipping; widgetinput før tekstvalg/lenker |
| application | `boxui/SyntheticActivity.cpp` | eksplisitt lokal simuleringsmodus og typed port; ingen domeneautoritet i renderer |
| application/composition | `BoxUiServices.cpp` | registrer parser, layout, child-diagramadapter og host; eksport bruker frosset snapshot |

Alle navn er Planned. Rust-bro kan bygges ved eksisterende Rust-integrasjon, men
BoxUI får egne C-ABI-innganger og wireversjon. Ikke legg kildeparsing i renderer,
FOX i interpreter, domene-/scenario-policy i widgets eller mer koordinering i
XfmdWindow. Funksjonalitetseiere skal få egne blueprints før kode begynner.

## Interaksjonsregler og akseptanse

Felles kontrakt velger `boxui`-gjerde med `boxui 0.1` + strict JSON; mermaid-gjerde
med samme header er alias. Separate typed modeller bevarer widgets/bindinger.
SVG/control map/native overlays publiseres samlet. Native tekstfelt beholder
utkast/caret/fokus ved kompatibel resize/theme, men ikke ved byttet binding/kontekst.
Enter sender typed intent; blur/save sender ingenting. Gammel ramme eller gjentatt
klikk skal ikke treffe ny kontroll. Unbound/simulated/native vises eksplisitt.

PDF fryser aksepterte verdier, utelater usendte utkast, bruker komplett statisk SVG
og utløser ingen hendelser. Preview-skalering, Wrap/A4/scroll og clipping må bruke
samme transform for bilde, overlay og hit-test. Vanlig Markdown-merking og lenker
beholdes utenfor widgets. Ingen auto-start av scenario, nettverk eller kildekode.

## Arbeidspakker etter review

X1: tolkning/modell mot falsk parser og kontraktfixtures.
X2: plassering, native input og overlay mot tydelig merkede mock-rammer.
X3: sessions/bindinger/draft/command-ledger og syntetisk Activity med uavhengig oracle.
X4: konkret fork-pin, faktisk grafikk, mixed Markdown, GUI/PDF og regressjoner.
R1–R4 i forken kan utvikles parallelt. X4 krever reell R4-leveranse og begge commits.
Ingen påstand om runtime-dekning før virkelige tester; mock-resultater er bare
kontrakt-/hostkontroll. JSON-skjema er ikke test av visuell kvalitet eller IME.

## Stoppunkt

G-PARALLEL-REVIEW er passert: brukeren har godkjent grunnlaget uten ny iterasjon. Før X1 må shared revision låses, berørte
functionality-blueprints konkretiseres og fasebrancher opprettes. Senere faser
får egne brancher/milestone-commits; én samlet PR for sprinten og bygg ved faseslutt.

## Identifisert reviewgrunnlag

Forkens designcommit: `54aee4e685a4454f2233060341336446d58054a9`.
Kontraktfilens SHA-256: `9eb0b629287349e4e2f849d0d3ab9f61b1e52d8fce64de1917e4c2b26d8a1a35`.
Dette identifiserer det godkjente kontraktgrunnlaget; runtime får egen konkret fork-pin.
Kontraktfil, seks JSON-skjemaer og fixtures hentes fra samme commit ved X1.

Innebygde diagrammer tolkes av interpreter før arbeid i renderer. Normalisert
BoxUiModel har childRef; application-kjeden forbereder eksisterende typed
DiagramModel-barn til SVG/error før BoxUI-layout komponerer dem. Ingen Mermaid-
kildetolkning flyttes til renderer. Foreslått C++ JSON-kodek er nlohmann/json
3.12.0, MIT, låst til upstream `55f93686c01528224f448c19128836e7df245f72`;
avhengighet/lisens/bootstrap inngår i X1. Den finnes ikke i dagens XFMD-bygg.
