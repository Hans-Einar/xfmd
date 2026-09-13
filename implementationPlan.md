# Implementeringsplan: xfmd

Status: **P0–P8 fullført; P9–P13 planlagt, ikke startet**, 2026-09-13.
Brukeren godkjente designgrunnlaget og autoriserte første leveranse i én sesjon.
Neste implementeringsløp er konkretisert nedenfor; tekniske kontrakter med
uavklarte P9-prøver beholder Proposed-status. En phase er en
avhengighetsstyrt leveranse, ikke en tidsbokset sprint. Hver phase har egen branch,
commits per milepæl og merge-commit til main; branchene beholdes på GitHub.

## 1. Gjennomført plan

| Phase / branch | Milepæl 1 | Milepæl 2 | Bevis |
| --- | --- | --- | --- |
| P0 `phase/p0-technical-proof` | Native FOX-probe og parser-/kildekartvalg (`bf0ab1d`) | Roundtrip, fonter og tekniske beslutninger (`e2a25d6`) | [P0](docs/evidence/P0.md) |
| P1 `phase/p1-build-contracts` | C++17/CMake og rene porter (`2311b52`) | Kontrakttester og lagkontroll (`3e9ebd2`) | [P1](docs/evidence/P1.md) |
| P2 `phase/p2-document-workspace` | Dokumenttransaksjoner/editor/workspace (`c0527d1`) | Roundtrip og feilbevaring (`8ef4ce6`) | [P2](docs/evidence/P2.md) |
| P3 `phase/p3-markdown-presentation` | Semantikk, native layout og host (`4051fd3`) | Ekte font-/GUI-verifikasjon (`6077924`) | [P3](docs/evidence/P3.md) |
| P4 `phase/p4-live-preview` | Bounded worker, debounce og tokens (`773eec1`) | Levetid, fokus og gamle resultater (`65dc19c`) | [P4](docs/evidence/P4.md) |
| P5 `phase/p5-synchronized-scrolling` | Kildeankre og begge scrollretninger (`ab5f034`) | Kodelinjer, guards og resize (`4b6c177`) | [P5](docs/evidence/P5.md) |
| P6 `phase/p6-navigation-history` | Lokal navigasjon og transaksjonell historikk (`456d0fb`) | Branching, avbrudd og native treff (`c8f0e6a`) | [P6](docs/evidence/P6.md) |
| P7 `phase/p7-release-verification` | Ytelse, feilhåndtering og samlet QA (`6a824d8`) | Sluttdokumentasjon, CI og installasjonsbevis (`1a686bd`); M3 avslutter bevis | [P7](docs/evidence/P7.md) |

P0 avklarte risiko før kontraktene ble låst. P1 var grunnlag for P2; P3 gjorde
arbeidsflaten til en faktisk Markdown-viser. P4–P6 la til de tre øvrige features.
P7 samler kravbevis, ytelse, installasjon og dokumentasjon. Dette er ikke en plan
om nye sprints eller automatisk delegasjon til subagenter.

## 2. Beslutninger fra implementasjonen

- **Parser:** cmark 0.31.1, CMARK_OPT_DEFAULT, uten utvidelser. MD4C-kandidaten
  manglet generelle blokkposisjoner for kildekartet. Porter og lag ble beholdt.
- **Preview:** egen FXScrollArea-host. Renderer leverer display list gjennom rene
  kontrakter; FOX-måling og tegning bor i application.
- **Tråder:** én parser-worker med én aktiv og én siste ventende jobb; GUI-tråden
  eier FOX, layout/fontmåling og paint. Token og generasjon beskytter publisering.
- **Tester:** CTest med små eksplisitte CHECK-baserte testprogrammer fremfor ekstern
  Catch2-dependency. Ekte FOX testes under isolert Xvfb; ASan/UBSan i separat bygg.
- **Dependencies:** eksplisitt bootstrap med hash, ingen skjult nettverksnedlasting
  ved configure/build. Prosjektlisens avventer eier; cmarks notis følger installasjon.

## 3. Akseptanse og begrensninger

Første scope omfatter AT-001–009 og AT-011–023. [P7](docs/evidence/P7.md) angir
faktiske tester, målinger og dekning, inklusive hva som er manuelt gjennomgått.
Et bestått struktursjekk er ikke bevis for hele applikasjonen. Blueprint-status
står Implemented der bredere desktop-akseptanse ikke er fullt automatisert.

AT-010/024 (xfw-IPC) er Future. Lokale bilder, fragmentlenker og Markdown-utvidelser
krever egne krav og blueprints. Ingen formell release publiseres før prosjektlisens
er valgt. Installérbar kildebygging og offentlig Git-historikk leveres nå.

## 4. Videre arbeid per designobjekt

1. Les krav, arkitektur og berørte blueprints; finn eksisterende functionality.
2. Beskriv kontrakt, konsumenter, feilvei og akseptanse før kode.
3. Oppdater kapittel 5 Plumbing med riktige symboler/filer i samme endring.
4. Implementer i fokuserte roller; behold FOX-bindinger i application.
5. Kjør relevante tester, lagkontroll og blueprint-/symbolkontroll.
6. Knytt bevis til commit/miljø; sett Verified bare når relevante AT-er er dekket.

Byggkommandoer finnes i [README](README.md); stil og bidragsregler i
[CONTRIBUTING](CONTRIBUTING.md). Historiske tekniske forsøk beholdes i faserapportene.

## P8 — Arbeidsområder og kjedet filfilter

Branch: `phase/p8-work-paths`. M1: UR-012–014 og FTR-005/FUNC-012/013 design.
M2: arbeidsrot, historikk, stoppbar skanning og FOX-panel, CLI og brukerhjelp.
M3: rene/native GUI-regresjoner, sanitizer, plumbing og CI før integrasjon.

P8 M1 (`5c3b2d3`) definerte designet; M2 (`bbe9062`) implementerte panel,
arbeidsstier og filter med tester. M3 dokumenterer [verifikasjon og visuell kontroll](docs/evidence/P8.md).

## 5. Planlagt utvidelse P9–P13

Status: **Planlagt, ikke startet**, 2026-09-13. Krav UR-015–020 / SR-015–019 og
[designrevisjonen](softwareDesign.md) beskriver neste leveranse. Dette er ikke
påbegynt runtime-implementasjon. Designet ligger på `design/scroll-pages-pdf`.
Hver fase får egen branch og PR; hver milepæl avsluttes med én meningsfull commit
og målrettet review. En milepæl kan omfatte flere nært relaterte klasser.
Merge skjer når fasens bevis og relevante regresjoner er ferdige. Neste fase
baseres på integrert foregående fase, uten å gjenta allerede fullførte tester.

Vi fortsetter med **phase**, slik P0–P8 brukte. En phase samler en leveranse
med klare avhengigheter; vi innfører ikke parallelle sprintnumre eller faste
kalenderlengder. Standardløpet er P9 → P10 → P11 → P12 → P13.

| Phase | Brukbar leveranse ved avslutning | Milepæler / commits | Primær akseptanse |
| --- | --- | --- | --- |
| P9 | Avklart inputvei, font-/PDF-grunnlag og vedvarende preferences | P9-M1–M3: 3 | Tekniske forutsetninger for AT-033, AT-035–038 |
| P10 | Preferences og justerbar scrolling i alle fire flater | P10-M1–M3: 3 | AT-025, AT-029, AT-030, AT-035, AT-038; scroll-del av AT-039 |
| P11 | Window wrap og A4-preview med bevart navigasjon/sync | P11-M1–M3: 3 | AT-031, AT-036 og side-/profil-del av AT-039 |
| P12 | PDF-eksport fra gjeldende buffer, også ulagret tekst | P12-M1–M3: 3 | AT-032, AT-037 og PDF-del av AT-036 |
| P13 | Fullscreen, installert ikon og samlet verifisert leveranse | P13-M1–M3: 3 | AT-033, AT-034; samlet gjennomgang av AT-029–039 |

Det gir **5 phase-brancher, 5 PR-er og 15 planlagte milepælcommits**, i tillegg
til designcommit og merge-commits. Historiske baseline-tester følger berørte
komponenter; tabellen er ikke en uttømmende liste over regresjoner.

### Oppstart og commitpraksis

Før P9 starter, ferdigstilles og committes designpakken på
`design/scroll-pages-pdf` med foreslått tittel
`Document scrolling and paged publication design`. Denne dokumentendringen
integreres som eget design-PR før P9-branch opprettes fra oppdatert main.
Baseline for implementasjonen er dermed P8 pluss sporbare designendringer.
Ingen P9-kode eller tomme implementasjonsbrancher opprettes som del av planleggingen.

Milepæl-ID-er er permanente, for eksempel `P10-M2`. Commit-titlene i tabellene
er planlagte imperative titler, ikke eksisterende commits. Hver commit inneholder
milepælens sammenhengende kode, nødvendige tester og oppdaterte kontrakter/plumbing.
Avdekkede feil rettes før milepælen regnes som passert. Senere reviewfunn får
sporbare fix-commits; de skjules ikke for å tvinge historikken til nøyaktig 15 commits.

Etter hver milepæl oppdateres faserapporten `docs/evidence/P<N>.md` med ID,
kontrollerte endringer, kommandoer, miljø, resultater, begrensninger og reviewfunn.
Rapporten opprettes ved første reelle arbeid i fasen. Samme commits rapport kan
identifiseres med milepæl-ID; endelige hasher føres i PR-/faseoppsummeringen,
siden en commit ikke kan inneholde sin egen hash.

### Kriterier for å passere en milepæl og fase

En implementasjonsmilepæl skal bygge og bevare eksisterende brukerfunksjoner.
Nye kontrakter migreres sammen med berørte konsumenter; ingen halvferdig menyhandling
presenteres som brukbar funksjonalitet. Interne byggesteg kan testes via fakes
før hele arbeidsflyten kobles inn.

Hver milepæl avsluttes med målrettede tester og review av endret ansvar, kall,
feilhåndtering og levetid. Kjør blueprint-, symbol- og lagkontroll ved berørte
endringer. Native FOX-kontroller kjøres isolert med Xvfb når det er tilstrekkelig;
fysisk touchpad og faktisk Window Maker dokumenteres særskilt. Sanitizer brukes
ved levetids-/trådendringer. Gjentatte brede testkjøringer krever en konkret grunn.

Ved faseslutt gjennomgås hele PR-en mot akseptansen, med skjermbilder for synlige
UI-endringer og faktiske bevis. PR-en oppgir krav-/blueprint-ID-er, endret plumbing,
kontroller og åpne begrensninger. Feil som bryter fasekriteriene må løses før merge.
Integrasjon bruker merge-commit og beholder phase-branchen, slik tidligere leveranse.
Designobjekter får Implemented/Verified først når deres respektive kriterier er oppfylt.

P9 dokumenterer beslutninger før etterfølgende kontrakter settes Ready. Hvis en
teknisk prøve ikke holder, revideres den berørte løsningen og planen. Uavhengig
arbeid kan fortsette, men fasen som trenger resultatet starter ikke på en uavklart
forutsetning. En prototype teller ikke som produksjonsimplementasjon eller full AT-dekning.

### P9 — Tekniske avklaringer

Branch: `phase/p9-input-typography-proof`. Avhengighet: integrert P8 og designpakken fra `design/scroll-pages-pdf`.

| Milepæl | Leveranse | Bestått når | Planlagt commit-tittel |
| --- | --- | --- | --- |
| P9-M1 Input og fullscreen | Lokal FOX-eventprobe for fysisk touchpad, hjul, små syntetiske deltaer og batching; EWMH-probe | Faktisk oppløsning, tidsstempler, akser og fullscreen/restore i Window Maker er dokumentert; beslutning om core-eventvei | `Verify FOX scroll input and fullscreen requests` |
| P9-M2 Felles typografibevis | Minimal delt glyphlayout på skjerm og Cairo-PDF; Unicode/fallback og A4-geometri | Samme glyphposisjoner og fontidentitet, valgbar PDF-tekst og riktige mål; avhengigheter/installasjon dokumentert | `Prove shared typography for screen and PDF` |
| P9-M3 Ressurser og preferences | FXRegistry-probe for skrivefeil, schema og WorkPaths-bevaring; kort layout-/PDF-måling | Lagre/publiser-kontrakt og konkrete ressurs-/cancel-grenser dokumentert; berørte blueprints kan settes Ready | `Validate preferences storage and resource budgets` |

Review: inputeierskap, fontlevetid, trådgrenser og tekniske forutsetninger.
Mislykket gate fører til revidert kontrakt, ikke skjult alternativ implementasjon.
Ingen global input-grab, X-restart eller lang batterikrevende test nødvendig.

### P10 — Preferences og felles scrolling

Branch: `phase/p10-scroll-preferences`. Tekniske forutsetninger: P9-M1/M3. Branch opprettes etter integrert P9.
Eiere: FUNC-014/015 og adapterkonsumentene FUNC-005/010/011; FTR-006.

| Milepæl | Leveranse | Bestått når | Planlagt commit-tittel |
| --- | --- | --- | --- |
| P10-M1 Uttrekk av scrollmekanisme | ScrollInput, ScrollProfile, ScrollDynamics og normalisering i eksisterende FoxWheelScrollBar | Speed=1/acceleration=false bevarer rest, clamp, modifikatorer og endepunkter i baseline | `Extract shared scroll motion without changing behavior` |
| P10-M2 Profil og dialog | PreferencesService/Store, FOX PreferencesDialog med isolert prøvefelt, Edit-meny | OK/Cancel, schema/feil/restart og oppdatering av fire levende scrollflater virker | `Add scroll preferences and an isolated test area` |
| P10-M3 Akselerasjon og integrasjon | Bounded rateestimat, egen retargeterbar timer, resets og typed origins | Presis reversering/topp/bunn, ingen dobbel gain i sync; fysisk input og begge akser gjennomgått | `Add scroll acceleration across all views` |

Review: ren matematikk/state mot FOX-eierskap. Unit-tester dekker kurver, timing,
profilvalidering og feil. Native test dekker tre, historikk, editor og preview;
ikke én unit-test per menyknapp. Tidligere preview-enable og wheel-bevis beholdes.

### P11 — Felles typografi og A4-preview

Branch: `phase/p11-paged-preview`. Avhengighet: P9-M2/M3 og integrert P10.
Eiere: FUNC-004/005/007/009/016/017, med profilvalg i FUNC-010/014.

| Milepæl | Leveranse | Bestått når | Planlagt commit-tittel |
| --- | --- | --- | --- |
| P11-M1 Kontrakt- og typografimigrering | LayoutUnit, FrameKey, FontCatalog/shaping-port og DisplayListPainter | Continuous visning bruker felles mål/tegning; Unicode, fallback, kildetreff og eksisterende linjeskiftregresjoner kontrollert | `Share typography and physical layout contracts` |
| P11-M2 Paginering | FlowLayout/visual lines, PageComposer/PageLayout, A4/marger, kodewrap og page anchors | Deterministiske sideskift, heading keep-rule, lange blokker, tomt dokument og ressursgrenser | `Compose A4 pages with source anchors` |
| P11-M3 Viewport og brukerflate | Window wrap/A4, fit-width/100 %, ViewTransform og anchor restore | Resize/zoom reflower bare riktig modus; stale profil avvises; sync og lenketreff virker over sidegap | `Add paged preview and preserve viewport anchors` |

Review: units, kontrakteierskap, fontsett og byteankre. Rene geometri-/pagebreaker-
tester pluss native preview-regresjon. Brukeren får A4-preview i denne fasen,
før eksportarbeidsflyten i P12. Ingen ny Markdown-dialekt.

### P12 — PDF-eksport

Branch: `phase/p12-pdf-export`. Avhengighet: P11 og P9s ressurs-/fontgate.
Eier: FUNC-018; fullfører FTR-007 sammen med P11.

| Milepæl | Leveranse | Bestått når | Planlagt commit-tittel |
| --- | --- | --- | --- |
| P12-M1 Frosset eksportjobb | ExportCoordinator og snapshot/profil/fontsett med test-output | Ulagret buffer fryses korrekt; edits/dokumentbytte påvirker ikke startet jobb; én jobb og testbar status | `Capture immutable PDF export jobs` |
| P12-M2 Output og publisering | PdfOutput, tempfil, finalisering/flush, PdfFilePublisher, cancel og File → Export PDF | Ingen delvis eksisterende målfil ved feil før commit, dirty/undo/historikk uendret, bounded shutdown | `Write and publish PDF exports transactionally` |
| P12-M3 WYSIWYG-bevis | Uavhengig PDF-leser, tekst/MediaBox/sidetall og raster ved samme DPI | Hele dokumentet samsvarer med A4-preview; ligatur/fallback, sideskift, full disk og cancel dokumentert | `Verify PDF fidelity and export failure handling` |

Review: jobb-/trådeierskap, filtransaksjon, snapshotidentitet og faktisk fontdeling.
Ingen UI-screenshot-eksport eller uavhengig layoutmotor for PDF.

### P13 — Fullscreen, identitet og samlet leveranse

Branch: `phase/p13-fullscreen-identity`. Planlagt integrasjon etter P12.
Fullscreen/ikon er funksjonelt uavhengig av eksport og kan utvikles tidligere
etter P9, men integreres på én tydelig baseline. Eiere: FUNC-010/019.

| Milepæl | Leveranse | Bestått når | Planlagt commit-tittel |
| --- | --- | --- | --- |
| P13-M1 Fullscreen | FoxWindowMode, View-meny/F11, Escape og WM-state-observering | Bekreftet fullscreen/restore i Window Maker, modal Escape, skjermendring og bevarte splitter/view mode | `Add fullscreen with workspace restoration` |
| P13-M2 Ikon og packaging | SVG-master, små PNG-varianter, IconResources og vindusikon | Visuell kontroll 16–128 px og ren installasjon; samme desktop-/MIME-identitet | `Install the XFMD application icon` |
| P13-M3 Samlet review og dokumentasjon | AT-029–039 med faktiske bevis, relevante baseline-regresjoner, README/release notes | Ingen nye ubekreftede Implemented/Verified-påstander, ressursgrenser og gjenværende begrensninger dokumentert | `Document and verify the integrated feature set` |

Unit-tester prioriteres for mekanismer med egen logikk. Native UI-/PDF-bevis
brukes der enhetstester ikke kan verifisere reell integrasjon. Korte målinger
brukes først; lange stresstester er ikke et generelt milepælkrav.
