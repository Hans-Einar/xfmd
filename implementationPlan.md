# Implementeringsplan: xfmd

Status: **Proposed**, 2026-09-12. Denne runden leverer design og dokumentasjonsmetode,
ikke applikasjonsimplementasjon. Ingen av stegene nedenfor er utført. Rekkefølgen
styres av kontrakter, risiko og akseptanse; ikke av sprintlengde.

## 1. Gjennomgang av designgrunnlaget

Les [krav](xfmd_requirements.md), [arkitektur](softwareArchitecture.md),
[arbeidsmåte](docs/working-method.md) og [objektregister](src/blueprint/README.md).
Før kodefasen gjennomgår vi særlig:

- Skillet mellom features, functionality, målgruppe og eierlag.
- Renderer som rent presentasjonslag og FOX-host som application-adapter.
- Nødvendigheten av `src/contracts/` som felles, liten kontraktkatalog.
- Fire features / elleve functionality-objekter; om noen er for brede/smale.
- Foreslått format-, lenke-, fil- og ytelsespolicy, som ikke er tidligere avtalte detaljer.

Dette er den etterspurte designgjennomgangen, ikke en ny godkjenningsrunde for
hver fil. Senere autorisert implementeringsarbeid følger metoden autonomt innen
avklart scope. Alle objekter starter Proposed; strukturelt gyldig er ikke Ready.

## 2. Plan og avhengigheter

| Steg | Leveranse og objekter | Avhenger av | Ferdigkriterium |
| --- | --- | --- | --- |
| P0 | Teknisk bevis: fonter/FOX-host, interpreter-kildekart, editor-roundtrip og benchmark. FUNC-003/004/005/009/011. | Designgjennomgang | Reproduserbare bevis og beslutninger for portene nedenfor. |
| P1 | C++17/CMake-grunnlag, kontrakttyper, composition root, fokuserte mapper, testoppsett og dependency-grenser. SR-001/002/003/012/013. | P0 | Ren configure/build/test og byttbarhet demonstrert med fake porter. |
| P2 | DocumentSession/Coordinator, LocalFileStore/InputPolicy, grunneditor, CLI/dialog/workspace. FUNC-001/002/010/011. | P1 | AT-001/003/006/007/009/016/017; feil gir ikke datatap. |
| P3 | Første ende-til-ende Markdown-visning. FUNC-003/004/005/007, FTR-001. | P2 + P0-bevis | AT-002/011/013/014/015; native fonter og synlige feil. |
| P4 | Debounce og live preview med korrekt revisjon/levetid. FUNC-006/007/011, FTR-002. | P3 | AT-004/018/020 og fokus-/undo-scenarioer bestått. |
| P5 | Kildeankre, begge scrollretninger og resize-restore. FUNC-009, FTR-004. | P4 | AT-008/019; samme avsnitt synlig, ingen echo-loop. |
| P6 | Lokal lenkenavigasjon, back/forward og ankerrestore. FUNC-008, FTR-003. | P5 | AT-005/009/015/018; alle dokumentinnganger registreres én gang. |
| P7 | Samlet QA, ressursgrenser, ytelse, dokumentasjon og pakking. Alle FirstRelease-objekter. | P6 | Hele AT-001–009 og AT-011–023 med bevis; godkjente scope-avvik listet. |
| Senere | xfw-IPC, ev. lokale bilder, fragmentlenker og Markdown-utvidelser. | Ny kravrevisjon | Egen blueprint før kode; AT-010/024 hører ikke til første leveranse. |

P2 kan åpne kilde/editor før preview eksisterer, men er ikke en full viewer-leveranse.
Historikkens rene algoritmetester kan utvikles tidligere; P6 er ende-til-ende-gaten.
Ingen av stegene er en automatisk forespørsel om nye subagenter.

## 3. P0: beslutningsporter med konkrete bevis

### P0-A: FOX-presentasjon

Bygg et lite forsøk med proporsjonal brødtekst, H1/H2 i ulike størrelser, fet/kursiv,
monospace, wrapping, clipping og lenketreff. Verifiser fontmål mot faktisk tegning
og at FXScrollArea-host kan holdes uten Markdown-logikk. Dokumenter valgt FOX-
versjon, kompilatorkommando, skjermbilde og hva som eventuelt ikke støttes.
FXText-only-preview er ikke godkjent erstatning for UR-002 uten kravendring.

### P0-B: interpreter og kildekart

Kjør MD4C-kandidat på fixtures med gjentatt tekst, Unicode, entiteter, escapes,
nestede lister, tomme blokker og kode. Dokumenter hvordan hver node får kildeområde
og mappingkvalitet. Ingen søk etter første match som generell løsning. Hvis
adapteren ikke kan gi tilstrekkelig presisjon, velg alternativ interpreter eller
avgrenset utvidelse og dokumenter kostnad/vedlikehold før resten bygges på den.
Lås dependency-version/commit, CommonMark-baseline og eksplisitte flagg.

### P0-C: dokument og editor

Bevis tapsfri LF/CRLF/BOM/Unicode gjennom FOX-editor, undo og lagring. Avklar
blandet linjeslutt, NUL-policy, byteoffsetkonvertering, fontfallback, ACL/xattr,
symlink/hardlink og feil etter rename. Velg én undo-stack. Resultat er testbevis
og eventuelle presise kravjusteringer, ikke antakelser gjemt i adapteren.

### P0-D: ressursbudsjett og dependencies

Mål et representativt 1 MiB-corpus med 30 varme parse+layout-kjøringer og kaldstart
på navngitt maskin. Registrer p95, RSS, compiler/build type og inputhash.
Foreslåtte grenser står i SR-011. Hvis en GUI-tråd blokkeres for lenge, velg tiltak
før live-preview-kontrakten fryses. Benchmark-prototype er ikke produksjonskode.

Velg dependency-distribusjon og prosjektlisens med eieren før kode gjenbrukes eller
release pakkes. Det offentlige repoet har foreløpig ingen valgt lisens; ikke legg
inn andres kildekode eller antatt lisens bare fordi repoet er offentlig.

## 4. P1: bygge- og teststruktur

Planlagt C++17, CMake og CTest. Lag separate targets for contracts (header-only),
interpreter, renderer og application. Bare application lenker FOX; bare
interpreter-adapteren lenker MD4C. Testverktøy foreslås Catch2, men konkret versjon
og installasjonsmåte låses i P1. Unngå skjult nettverksnedlasting under configure.
Innfør `.clang-format` med to mellomrom og CI med låste dependencies.

Følgende kommandoer er **planlagte og virker ikke før P1**:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON
cmake --build build
ctest --test-dir build --output-on-failure
./build/xfmd tests/fixtures/markdown/basics.md
```

Testkart: `tests/contracts/`, `tests/interpreter/`, `tests/renderer/`,
`tests/application/`, `tests/gui/`, `tests/acceptance/`, `tests/fixtures/`.
Filnavn `<Subject>Test.cpp`; scenarioer identifiserer relevante AT-/UR-/SR-ID-er.
GUI-tester skal ha eksplisitt display-oppsett; headless tester krever ikke FOX.
Ingen prosentvis coverage-grense er valgt. Alle krav må ha relevant bevis;
linjedekning alene er ikke akseptanse.

## 5. Gjennomføring per objekt

1. Avklar tilknyttede krav og P0-beslutninger; sett objekt Ready med begrunnelse.
2. Implementer kontrakter og roller i arkitekturens filkart; ingen store samleklasser.
3. Oppdater kapittel 5 når symboler lander, fra Planned til Implemented med riktige filer.
4. Kjør kontrakt-/unit-/integrasjonstester som passer endringen; legg til regressjon
   for faktisk feil, ikke tester som bare speiler en implementasjonslinje.
5. Knytt AT-bevis til commit/miljø. Kontroller kravmatrise og konsumenter ved API-endring.
6. Sett Verified først etter samlet akseptanse. Hold gamle bevis som historikk ved revisjon.

P3 må inkludere både fake og ekte fontadapter. P5 må teste forsinket event-ekko,
ikke bare synkrone callbacks. P6 må injisere åpnefeil og dirty-cancel. P7 må
bekrefte at dokumenter ikke utløser nettverkskall eller shell-evaluering.

## 6. Kontroll som finnes nå

```sh
python3 tools/validate_blueprints.py
```

Kontrollen verifiserer dokumentstruktur, ID-er, lokale fillenker, kravdekning,
avhengighetsreferanser og plumbing-status. Den erstatter ikke semantisk review,
symbolanalyse eller tester av framtidig applikasjonskode. Se
[arbeidsmåten](docs/working-method.md) for forslag til videre prosessforbedringer.
