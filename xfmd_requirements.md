# Kravspesifikasjon: xfmd

Status: **Proposed**, revisjon 0.2, 2026-09-12. Revisjonen konkretiserer opprinnelig
intensjon og brukerens arkitekturføringer. Tallgrenser og detaljpolicyer merket
«foreslått» er forslag til gjennomgang, ikke målte egenskaper.

## 1. Formål og avgrensning

xfmd skal være en rask, lokal Markdown-viser med enkel redigering, bygget på FOX
og naturlig å bruke sammen med Xfe-verktøyene `xfw` og `xfi`. Brukeren skal kunne
lese, navigere, redigere og sammenligne kilde med rendret dokument i ett vindu.
Første leveranse er en selvstendig Linux-applikasjon uten nødvendige endringer i
`xfw`, `xfi` eller Xfe. «Companion» betyr komplementær bruk og filåpning, ikke at
andre programmer allerede har et integrasjons-API.

FOX er applikasjonens toolkit. Interpreter og renderer skal kunne erstattes
uavhengig gjennom avtalte kontrakter. Se [arkitekturen](softwareArchitecture.md).

## 2. Begreper og kravstatus

- **Use case (UC):** brukerens situasjon og forløp; kan krysse flere features.
- **User requirement (UR):** observerbart resultat eller brukerregel.
- **System requirement (SR):** intern kontrakt, kvalitet eller begrensning.
- **Feature (FTR):** avgrenset, sammenhengende evne med egen akseptanse.
- **Functionality (FUNC):** tjeneste/atferd med ett eierlag og eksplisitte kall.

Alle krav er foreslått for første leveranse med mindre «senere» er angitt. ID-er
er stabile; slettede krav beholdes som `Retired` med begrunnelse. Et dokument eller
en stub oppfyller ikke i seg selv et funksjonelt krav.

## 3. Use cases

| ID | Forløp og forventet resultat | Alternative forløp | Krav |
| --- | --- | --- | --- |
| UC-001 | Start `xfmd fil.md` eller åpne via filbehandler; les rendret dokument. Uten argument vises tom arbeidsflate med Åpne. | Manglende/uleselig fil eller ugyldig encoding gir feil uten tap av gjeldende buffer. | UR-001, UR-002, UR-009 |
| UC-002 | Følg lokal dokumentlenke; bruk Alt+Venstre/Høyre til tilbake/frem med bevart leseposisjon. | Avbryt dirty-dialog, brutt lenke eller feilet lasting endrer ikke historikken. | UR-005, UR-009 |
| UC-003 | Vis/skjul sidepanel med F10; velg mappe og dobbeltklikk `.md`/`.txt`. | Enkeltklikk velger bare. Uleselig mappe gir feil uten dokumentbytte. | UR-006, UR-009 |
| UC-004 | Velg side-ved-side, rediger, se preview etter pause, angre/gjør om, søk og lagre. | Parse-/lagringsfeil beholder edits; lukking/dokumentbytte spør om ulagret tekst. | UR-003, UR-004, UR-007, UR-009 |
| UC-005 | Scroll i editor eller preview og se tilsvarende kildeavsnitt i motsatt flate. | Resize, skjult panel eller gammel mapping gir re-layout eller midlertidig deaktivert sync. | UR-008 |
| UC-006 | Senere: bruk separat `xfw` som editor og xfmd som preview. | Frakobling beholder siste lokale visning og gir status. | UR-010 |

## 4. Brukerkrav

| ID | Krav | Akseptanse / planlagt bevis |
| --- | --- | --- |
| UR-001 | Åpne lokale `.md`/`.txt` via CLI, dialog og sidepanel i samme aktive økt. `.txt` vises som ren tekst. | AT-001: mellomrom/Unicode i sti, tom/manglende fil; ingen uønsket ekstra prosess. |
| UR-002 | Vis H1–H6, avsnitt, fet/kursiv, lister, sitater, kode og lenketekst. Brødtekst/overskrifter er proporsjonale; kode er monospace; overskriftsnivåer er tydelige. | AT-002: fixtures og visuell kontroll av blandede fonter, nesting og linjebryting. |
| UR-003 | Tilby tekstredigering, vanlig utklippstavle, angre Ctrl+Z, gjør om Ctrl+Y, søk Ctrl+F og lagring Ctrl+S. | AT-003: lagre–åpne gir samme tekst; undo/redo oppdaterer dirty og preview. |
| UR-004 | Oppdater preview når det har gått 300 ms uten ny redigering. Behold fokus og editorens markør. | AT-004: simulert klokke bekrefter debounce; GUI-sjekk bekrefter fokus og nyeste revisjon. |
| UR-005 | Lokale lenker og tilbake/frem gir dokumentnavigasjon med sesjonshistorikk og gjenopprettet leseposisjon. | AT-005: A→B→C, tilbake til B, ny lenke D sletter frem-grenen; avbrutt/feilet åpning endrer ikke køen. |
| UR-006 | Sidepanelet viser mapper og `.md`/`.txt`, kan skjules og åpner på dobbeltklikk. | AT-006: blandede filtyper, enkelt-/dobbeltklikk og F10. |
| UR-007 | Tilby preview alene (standard), editor alene og editor venstre/preview høyre i justerbar splitter. | AT-007: modusbytte bevarer dokument, dirty, undo og fokus. |
| UR-008 | Synkroniser scrolling begge veier etter kildeanker, også med wrapping og varierende teksthøyde, uten pendling. | AT-008: overskrift, lang liste og kodeblokk holder tilsvarende avsnitt synlig; resize og tom fil testes. |
| UR-009 | Vis dirty-status og handlingsrettede feil. Ved bytte/lukking tilby Lagre, Forkast eller Avbryt. Avbrudd/feil bevarer gjeldende dokument. | AT-009: skrivefeil, ekstern endring, avbrudd og feilet nytt dokument gir ingen stille tap. |
| UR-010 | Senere: motta dokument- og kildeankeroppdateringer fra `xfw` uten å endre interpreter/renderer. | AT-010: framtidig IPC-integrasjonstest; ikke første leveranse. |

## 5. Systemkrav

| ID | Krav | Akseptanse / planlagt bevis |
| --- | --- | --- |
| SR-001 | `application`, `interpreter`, `renderer` er egne kildekataloger. Bare application integrerer FOX; delte kontrakter har ingen FOX-/MD4C-typer. | AT-011: byggeavhengigheter og headless kontrakttester; alternative implementasjoner krever bare wiring. |
| SR-002 | Én tydelig eier per dokumenttilstand og functionality; ingen direkte mutasjon mellom søskenwidgets eller feature-interner. | AT-012: eierskaps- og kallgjennomgang mot blueprints. |
| SR-003 | Interpreter produserer uforanderlig semantisk modell med revisjon og kildeankre; renderer konsumerer uten parserkall. | AT-013: fixture og ekte/fake implementasjoner bruker samme kontraktstest. |
| SR-004 | Lås CommonMark-baseline og eksplisitte MD4C-flagg i P0. Ingen stilltiende aktivering av voksende «GitHub dialect». | AT-014: versjon/flaggliste og corpus med Unicode, entiteter, HTML som inert tekst og ufullstendig syntaks. |
| SR-005 | Ingen nettverkslasting, skripteksekvering eller browser engine. Lenker sendes aldri til shell for evaluering. | AT-015: eksterne ressurser, `javascript:`, `data:` og HTML gir ingen slik sideeffekt. |
| SR-006 | Valider UTF-8 og input før dokumentbytte. Bevar linjeslutt og BOM ved lagring; ingen automatisk normalisering. | AT-016: UTF-8, LF/CRLF, BOM, ugyldige byte og NUL-policy testes byte-for-byte. |
| SR-007 | Lagring skriver midlertidig søskenfil og erstatter mål bare ved suksess, bevarer vanlige modusbits og kontrollerer ekstern endring. | AT-017: feil før rename, full disk og endret mål bevarer fil og dirty-buffer. |
| SR-008 | Preview/layout har dokument-ID, revisjon og layoutgenerasjon. Foreldet resultat publiseres eller brukes aldri til scrolling. | AT-018: dokumentbytte, edit og resize mens arbeid er ventende. |
| SR-009 | Mapping bruker eksplisitte UTF-8-byteoffsets/kildeankre, ikke likhet mellom byte, tegn, visuelle rader og linjenumre. | AT-019: Unicode, gjentatt tekst, entiteter, lister, tomme blokker og wrapping; mappingkvalitet rapporteres. |
| SR-010 | FOX-objekter/GUI-mutasjon eies av GUI-tråden. Første implementasjon bruker FOX-event loop med målte arbeidsgrenser. | AT-020: timer-/objektlevetid og blokkering måles; worker-design krever eksplisitt revisjon. |
| SR-011 | Foreslått: filgrense 8 MiB; benchmark-corpus til 1 MiB. Referansemaskin: p95 parse+layout ≤100 ms, første visning ≤500 ms for 1 MiB, RSS ≤100 MiB. | AT-021: 30 varme kjøringer og egen kaldstart; oppgi maskin/bygg/input. Overskridelse krever tiltak eller kravrevisjon. |
| SR-012 | Features/functionality har krav-ID, eier, kontrakter, plumbing, feilvei, gjenbruk og verifikasjon. | AT-022: validator og manuell semantisk sporbarhetskontroll. |
| SR-013 | Roller deles i fokuserte filer; nye tjenester begrunnes i krav og gjenbruk eller nødvendig ansvarsgrense. | AT-023: filkart og review; ingen skjult funksjonalitet i vindusklasse/generisk hjelpefil. |
| SR-014 | Senere: IPC versjonerer protokoll, avgrenser meldingsstørrelse, kontrollerer lokal peer og dokument/revisjon. | AT-024: framtidige tester av feil peer, partial reads, gammel revisjon og frakobling. |

## 6. Detaljpolicy for første leveranse

Dette er eksplisitte forslag slik at implementasjonen slipper å gjette:

- **Markdown er tolerant tekst.** «Validere `.md`» betyr encoding, ressursgrenser
  og støttet profil. Ufullstendig Markdown under skriving blokkerer ikke lagring.
  Egen lint-/kvalitetsfeature krever et brukerkrav om diagnoser og forslag.
- `.md`/`.txt` matches uten hensyn til ASCII store/små bokstaver. Andre endelser
  avvises med forklaring. NUL avvises som ikke-støttet editorinput; tom fil er
  gyldig. Blandede linjesluttsekvenser må bevares via editoradapteren; hvis ikke,
  avvis redigering eksplisitt fremfor å normalisere stille.
- CommonMark-kjerne prioriteres. Tabeller, task lists, strikethrough og fotnoter
  er senere utvidelser. Full CommonMark-konformitet påstås først etter tester.
- HTML vises inert. Bilder vises med alt-tekst/plassholder; heller ikke lokale
  bilder dekodes i første leveranse. CSS, JavaScript, plugins og IDE er utenfor scope.
- Relative lenker løses mot dokumentets mappe. `..` er tillatt: dette er en lokal
  viewer, ikke en prosjekt-sandbox. Vanlige relative/absolutte lokale stier er
  tillatt; nett-URI-er og andre schemes åpnes ikke automatisk. Fragmentlenker
  markeres som ikke støttet i første versjon; ingen stille feilnavigasjon.
- Historikk er liste med cursor, ikke FIFO. Foreslått grense 100 poster, kun i
  minnet. Lik gjeldende destinasjon gir ikke duplikat. Filer leses på nytt ved
  tilbake/frem; historikken er ikke en kopi av gamle dokumentversjoner.
- Eksternt endret fil avbryter vanlig lagring. Tilby ny lasting etter dirty-sjekk
  eller lagring under annet navn, uten automatisk merge. Symlinker lagres til
  eksplisitt oppløst mål. Flere hardlinks avvises for erstatningslagring med tilbud
  om annet navn. ACL/xattr-policy avklares i P0; metadata tapes ikke stille.

## 7. Sporbarhet og gjennomgang

[Blueprint-registeret](src/blueprint/README.md) kobler hvert UR/SR til designobjekter
og AT-ID-er. Load/save, splitter og filtrering er functionality, ikke automatisk
features. Før implementering gjennomgås fontstrategi, presis mapping, ytelsesbudsjett,
Markdown-profil og filpolicy. IPC, fragmentlenker og lokale bilder krever senere
kravrevisjon. Ingen SDL-kompilator eller avhengighet til SDP innføres nå.
