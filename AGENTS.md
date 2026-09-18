# Repository Guidelines

## Formål og leserekkefølge

xfmd er en lettvekts Markdown-viser/editor og companion til `xfw` og `xfi`.
Les [kravene](xfmd_requirements.md), [arkitekturen](softwareArchitecture.md),
[arbeidsmåten](docs/working-method.md) og berørte [blueprints](src/blueprint/README.md)
før endringer. Baseline P0–P8 er implementert. [Designrevisjon 1.1](softwareDesign.md)
beskriver Proposed utvidelser; Planned-symboler er ikke eksisterende kode.

## Arkitektur og plassering

- `src/application/`: FOX-applikasjon, vinduer, kommandoer, koordinering og adaptere.
- `src/interpreter/`: Markdown-tolkning; ingen FOX- eller renderer-avhengighet.
- `src/renderer/`: presentasjon, layout, hit-testing og visuell kildemapping; ingen FOX- eller interpreter-avhengighet.
- `src/contracts/`: minimale, FOX- og parser-frie grensesnitt og datatyper.
- `src/blueprint/feature/` og `src/blueprint/functionality/`: designobjekter, ikke kode.

FOX er et varig valg. Application eier FOX-integrasjonen; renderer beskriver hva
som tegnes, og FOX-adapteren utfører tegningen. Bytte av interpreter eller renderer
skal bare kreve ny implementasjon og registrering i composition root, ikke endring
i applikasjonens arbeidsflyter.

## Små filer med tydelig ansvar

Følg filkartet i arkitekturen. Én hovedrolle per header/implementasjonspar.
`XfmdWindow` bygger vinduet; den skal ikke parse, lagre eller beregne scrolling.
`DocumentSession` eier dokumenttilstand; egne koordinatorer håndterer preview,
navigasjon og scrolling. Unngå `Manager`, `Utils` og samlefiler uten presist ansvar.
Over omtrent 300 linjer: vurder oppdeling og dokumenter begrunnelsen; ikke del
mekanisk. Nye filer skal ha dokumentert eier og en naturlig plass i filkartet.

## Arbeidsflyt og sporbarhet

Arbeid i rekkefølgen **krav → blueprint → kontrakt/plumbing → kode → verifikasjon**.
Bruk [malene](src/blueprint/templates/README.md). Behold kapittel 5 som `Plumbing`,
med konkrete kall, kildefiler, data og feilvei. Merk symboler som planlagte inntil
de finnes. Oppdater blueprint og kode i samme endring.

Søk etter eksisterende functionality før du lager ny. Features gjenbruker
functionality; de kaller ikke hverandres interne implementasjon. Eksponer bare
små, begrunnede kontrakter, og oppgi faktisk eller planlagt konsument.

## Sprinter, faser og bygg

Følg [prosjektets arbeidsmåte](docs/working-method.md) og
[versjonsreglene](docs/versioning.md). Dokumenter nye leveranser under `sprints/`.
Én branch per fase, én commit per milestone og én samlet PR per sprint.
Autonom flerfaseøkt bygger ved faseslutt, ikke ved hver commit. I samarbeid
med brukeren kan hver feilretting bygges. Behold historikken ved merge; ikke
squash/rebase publiserte commits. Ikke opprett en SDP-mappe som del av dette.

## Stil, kontroll og bidrag

Skriv prosjektprosa på norsk og kodeidentifikatorer på engelsk. C++17-stil:
to mellomrom, `PascalCase` for typer/filer, `camelCase` for metoder og RAII for
eierskap. Behold etablerte CMake-/CTest-kontroller.

Kjør `python3 tools/validate_blueprints.py` og `python3 tools/check_blueprint_symbols.py`
ved designendringer. Gjeldende bygge-/testkommandoer står i [README](README.md).
Commits skal ha korte imperative titler. PR-er oppgir krav-/blueprint-ID-er,
endret plumbing, utførte kontroller og kjente begrensninger; legg ved skjermbilder
ved synlige GUI-endringer. Ikke marker noe `Verified` uten dokumentert testbevis.
