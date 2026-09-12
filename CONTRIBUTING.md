# Repository Guidelines

## Struktur og leserekkefølge

Les krav, softwareArchitecture.md, docs/working-method.md og berørte blueprints
før endringer. `src/application/` eier FOX, dokumentarbeidsflyter og adaptere.
`src/interpreter/` tolker Markdown; `src/renderer/` eier presentasjon og layout.
Begge bruker bare rene porter fra `src/contracts/`. Bare application binder FOX;
bare interpreter binder cmark. Registrer utskiftbare implementasjoner i Application.

Én hovedrolle per header/implementasjonspar. Vinduer bygger UI; egne koordinatorer
håndterer dokumenter, preview, scrolling og navigasjon. Ikke legg parsing eller
lagring i XfmdWindow. Vurder oppdeling rundt 300 linjer; dokumenter begrunnede
unntak. Unngå generiske Manager/Utils-filer.

## Arbeidsflyt og plumbing

Følg **krav → blueprint → kontrakt/plumbing → kode → verifikasjon**.
Features bor i `src/blueprint/feature/`; delte tjenester i `functionality/`.
Bruk faste maler og stabile ID-er. Kapittel 5 heter alltid Plumbing og viser
konkrete kall, symboler, kildefiler, data og feilveier. Merk framtidige symboler
Planned. Oppdater kode og berørte kallkart sammen.

Søk etter eksisterende functionality før ny kode. Features skal bruke dokumenterte
tjenester, ikke hverandres interne implementasjon. Eksponer bare små kontrakter
med begrunnet konsument. En metode trenger ikke eget designobjekt.

## Bygg, stil og tester

Kjør bootstrap én gang og bygg/test med kommandoene i README. CTest dekker rene
kontrakter, dokument-I/O, parsing, layout, koordinering og FOX under isolert Xvfb.
Tester heter `<Subject>Test.cpp`; fixtures ligger i `tests/fixtures/`. Assets finnes
i `packaging/`. Ingen prosentvis coverage-grense er valgt: dokumenter relevant
AT-/kravbevis og feilscenarier, og bruk separat sanitizer-bygg ved levetidsendringer.

C++17, to mellomrom, PascalCase typer/filer, camelCase metoder og RAII-eierskap.
Bruk `.clang-format`; prosjektprosa er norsk, kodeidentifikatorer engelske.
Kjør `python3 tools/validate_blueprints.py` og `python3 tools/check_layers.py`.
Strukturkontroll erstatter ikke semantisk review av kall og eierskap.

## Commits og gjennomgang

Historikken bruker korte imperative milepæltitler. Hver avhengighetsstyrte phase
har egen `phase/pN-...`-branch og commits per milepæl; integrasjon bruker merge-commit.
PR-er beskriver krav-/blueprint-ID-er, endret plumbing, testresultater og begrensninger.
Legg ved skjermbilder ved GUI-endringer. Verified krever faktisk dokumentert bevis.
