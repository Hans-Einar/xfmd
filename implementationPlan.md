# Implementeringsplan: xfmd

Status: **P0–P13 implementert og lokalt verifisert**, 2026-09-13.
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

Status: **Implementert P9–P13**, 2026-09-13. Krav UR-015–020 / SR-015–019 og
[designrevisjonen](softwareDesign.md) beskriver neste leveranse. Runtime-implementasjonen følger milepælene nedenfor; tabellene bevarer de opprinnelige akseptansekriteriene. Designet ligger på `design/scroll-pages-pdf`.
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

## 6. Gjennomføring av P9–P13

| Fase | Milepælcommits | Integrasjon / bevis |
| --- | --- | --- |
| P9 | 628354e, 2025098, 029d82c | [PR #2](https://github.com/Hans-Einar/xfmd/pull/2), [P9](docs/evidence/P9.md) |
| P10 | 1bbf461, 52beeb0, a331fdd | [PR #3](https://github.com/Hans-Einar/xfmd/pull/3), [P10](docs/evidence/P10.md) |
| P11 | a162d3d, 73614d3, 2648611; benchmark c2107dc | [PR #4](https://github.com/Hans-Einar/xfmd/pull/4), [P11](docs/evidence/P11.md) |
| P12 | 293fe08, 7f3b220, 0a126e5 | [PR #5](https://github.com/Hans-Einar/xfmd/pull/5), [P12](docs/evidence/P12.md) |
| P13 | 1f92f20, f7a2259, P13-M3 | [P13](docs/evidence/P13.md); siste commit identifiseres i fase-PR |

Designgrunnlaget ble integrert i [PR #1](https://github.com/Hans-Einar/xfmd/pull/1).
Brancher beholdes. Fysisk touchpad/fler-monitor-hotplug er ikke erklært verifisert;
automatiserte tester og isolert Window Maker er dokumentert per fase.


## P14 — native tabeller, lenkemarkør og muselevetid

Branch: `phase/p14-tables-input`, egen PR og milestone-commits.

- M1: reproduser og rett ubalansert pointer-grab, bruk fonttegnet ↗ for HTTP(S).
  Native blandede knappetrykk, stale preview og videre meny-/knappebruk testes.
- M2: pin cmark-gfm 0.29.0.gfm.13 med bare table-utvidelsen. Eide tabellrader/celler
  i kontrakten, egen TableLayout, samme inline-shaping, ankre og PDF-tegning.
- M3: tabell-/PDF-/inputregresjon, visuell kontroll, baseline-ytelse, installasjon,
  oppdatert dokumentasjon og CI før merge.

Ingen browser engine eller FXTable-widget inne i dokumentflaten. TableLayout
beregner kolonner, bryter celleinnhold, tegner header/borders og holder en rad
samlet ved sideskift. En rad høyere enn utskrivbar side avvises med forklaring;
header gjentas ikke automatisk på neste side i denne første tabellutvidelsen.
Maks 64 kolonner og 50 000 celler; eksisterende glyph-/run-/sidegrenser gjelder.


P14-status: M1 (`4ac475a`) og M2 (`6fd8469`) er implementert; M3 samler endelig
verifikasjon, TextProjection-minneretting og installasjon. Se
[P14-evidens](docs/evidence/P14.md). Egen PR/CI kontrolleres før merge.

## P15 — sidepanel med dokumentindeks og referanser

Branch `phase/p15-document-index`, egen PR, commit ved hver milestone.

- M1: krav/blueprints, semantisk indeksering og bounded referanse-worker med tester.
- M2: Files/Index-faner, delt kapittel-/referansetre, enkeltklikk og kildeankre.
  Gjenbruk filtransaksjoner, preview-modell og scrolladapter; isolert FOX-test.
- M3: review av stale/dirty/livstid, regresjon og visuell evidens, dokumentasjon,
  installasjon, CI og merge. Kort målrettet verifikasjon på batteri.

P15-status: M1 (`33634d6`) og M2 (`2d187cd`) implementert. M3 dokumenterer review,
regresjon, GUI-evidens og installasjon. Alle 39 Release-tester passerte; etter
Save As-/levetidsjusteringer passerte berørte tester på nytt, inklusive seks
ASan/UBSan/LSan-tester. GitHub kjører hele matrisen før merge.

## P16 — nettleserpreferanse og Xfe-studie

Branch `phase/p16-browser-xfe-study`, egen PR.
- M1: utvid eksisterende Preferences og felles HTTP(S)-aktivering, vedvarende
  programvalg, målrettede tester for argv/Cancel/feil og native dialog.
- M2: kildebasert Xfe-analyse mot upstream og vår fork, komponentkart og anbefalt
  integrasjonsrekkefølge; dokumentasjon, review, installasjon og CI før merge.
Xfe-integrasjonen er en studie i denne fasen, ikke et uavklart toolkit-bytte.

## P17–P19: integrasjon av FOX UI/UX-forslagene

Bestilt 2026-09-14. Grunnlag: fox_ui_improvements.md og fox_ux_improvements.md.
Light/Dark er ett vedvarende valg som deles av toolbar og Preferences. Egen
phase-branch/PR per fase; commit per milestone. Designprototypen inngår som
historisk sammenligningsgrunnlag, ikke produktkode.

| Phase / branch | Milestone-commit | Resultat og gate |
| --- | --- | --- |
| P17 / phase/p17-ui-foundation | M0: krav og integrasjonsplan | UR-025/026/027 og AT-045/046/047; eiere FUNC-010/014 |
| P17 | M1: preferanser og profiler | Light/Dark, Comfortable/Compact, Flat/Classic og kontrollfont. En profilkilde med validerte overrides; persistens/fallback-tester |
| P17 | M2: UI-komponenter | UiContext, ikonressurser, UiButton/ButtonPainter, factory og layoutkomponenter. Native input, state, ressurslevetid og mål testes |
| P18 / phase/p18-themed-workspace | M1: toolbar og tema | Ikonhandlinger, grupper, checked modus/sidebar og Light/Dark; felles CommandRouter og vedvarende valg |
| P18 | M2: arbeidsflate | Files-header/filter/Refresh, tydelige Index/References, lokal preview-format/zoom og konsistente avstander. Behold lazy/dirty/history/scroll |
| P19 / phase/p19-appearance-preferences | M1: Preferences | Appearance/Scrolling/Document/Programs, live utseendeutkast, OK/Cancel/feil og felles dialogknapper; reload av profilfil |
| P19 | M2: samlet kvalitet | Unit/native GUI, Light/Dark og compact, tastatur/state, rollback, smal layout, eksisterende regresjoner, sanitizer og korte ytelsesmål |
| P19 | M3: dokumentasjon og installasjon | Ekte produktskjermbilder, release notes og evidens; CI/PR, atomisk installasjon uten å lukke brukerens dokument |

Avgrensning: alle de foreslåtte første UI-/UX-stegene integreres. Automatisk
kapittelfølging, sammenleggbar historikk og dynamisk toolbar-overflow var merket
«senere, bare hvis nyttig» og inngår ikke. Smal layout håndteres med en eksplisitt
kompakt toolbar/fallback; ingen kontroll får overlappe. Ingen Xfe-import, global
FOX-override, dokumentfontendring, renderer-bytte eller plugin-SO-er.

Tema-preview skal ikke committe preferanser eller endre dokument/undo. Ved
Cancel/gammel lagringsfeil gjenopprettes aktivt utseende. Light/Dark på toolbar
committer bare temafeltet og beholder density, font, knappestil og øvrige valg.
Alle nye produksjonsklasser hører til application/ui/style eller controls;
interpreter/renderer-kontraktene er uendret.


### Gjennomføring P17–P19

| Fase | Milestone-commits | PR |
| --- | --- | --- |
| P17 | M0 1e158b3, M1 2caac7c, M2 69f77c1 | [#10](https://github.com/Hans-Einar/xfmd/pull/10) |
| P18 | M1 41a9748, M2 23a2cd4, testmiljø 22bfcd6 | [#11](https://github.com/Hans-Einar/xfmd/pull/11) |
| P19 | M1 beb3b26, M2 9f110b6, M3 dokumentasjon/installasjon | [#12](https://github.com/Hans-Einar/xfmd/pull/12) |

Implementert etter første leveranses avgrensning. [Produksjonsdesign](docs/design/fox-ui-layer.md)
oppgir faktiske klasser; [evidens](docs/evidence/P17-P19.md) dokumenterer tester,
review, produktskjermbilder og en kort oppstartsmåling. Framtidspunktene over
(automatisk kapittelfølging, sammenleggbar historikk, dynamisk overflow) er ikke
merket implementert.


## P20 — trehandlinger og robust Split

Branch phase/p20-navigation-split med egen PR.
- M1: xdg-open for eksterne filtyper og delt Enter/Space/leaf-Right-policy.
- M2: reproduser Editor → Split med faktisk geometri, bevar deling og tegn
  Editor/Split/Preview-ikoner i samme katalog som eksisterende toolbar.
- M3: målrettede native/regresjonstester, dokumentasjon, installasjon og CI før merge.


P20 M1 `7e4d159` og M2 `7eb8346` er implementert. M3 samler
[testbevis](docs/evidence/P20.md), produktskjermbilde og installasjon.
Seks native Release-tester og fem sanitizer-tester passerte lokalt; full CI er merge-gate.

## P21 — Live lesefarger og temaspesifikk persistens

Branch phase/p21-reading-colors. M1: UR-030/031, eiere og forskningsgrunnlag.
M2: profiler, semantiske dekorasjoner, repaint og fire native sliders.
M3: lagring/rollback, raster/GUI/PDF-regresjoner, visuell kontroll og integrasjon.

P21 M1 `b6f934a`, M2 `9b259a9` er implementert. M3 samler
[testbevis, visuell kontroll og installasjon](docs/evidence/P21.md).
47 Release-tester og 45 sanitizer-tester består lokalt; fase-PR krever grønn CI.

## P22 — kompakt topplinje, delte kontroller og hover

Branch phase/p22-compact-workspace. M1: UR-032–034 og eierskap.
M2: felles lav layout, editorpalett/format og oppløst hover-mål.
M3: native geometri/input/regresjon, skjermbilder, installasjon og CI.

P22 M1 `b89149d`, M2 `35400a9`; M3 inkluderer Fit width ved hele tegnpiksler,
[testbevis og installasjon](docs/evidence/P22.md). Lokalt består 48 Release-tester
og 46 sanitizer-tester. CI er merge-gate.

## P23 — rikt Markdown-innhold og kopiering

Branch phase/p23-rich-preview. M1: UR-035–037, FUNC-021/022 og kontraktdesign.
M2: logisk lesetekst, glyph-basert merking, native clipboard, matematikkprofil,
lokale bilder, felles ressursforberedelse og PDF-tegning.
M3: regresjoner, visuell kontroll, dokumentasjon og installasjon.

P23 er implementert: M1 `e190f49`, M2 `094adf3`, M3 `fc38d0b` og
[evidens/installasjon](docs/evidence/P23.md). 50 Release- og 48 sanitizer-tester
består lokalt, med tilleggskontroll av flersidet kopiering. CI er merge-gate.

## P24 — Recent files

Branch: `phase/p24-recent-files`. M1: UR-038, FUNC-010 og plumbing.
M2: egen listeadapter, persistens og dokumentkobling. M3: AT-058 med
GUI-/restart-/feilveitest, regresjon og installasjon. Mermaid er et separat
diagramformat og inngår ikke i denne avgrensede filhistorikkendringen.

P24 M1–M3 er implementert. [Testbevis](docs/evidence/P24.md) dekker AT-058,
Release, sanitizers og Light/Dark.

## P25–P29 — Mermaid via mermaid-rs-renderer

Status: **Implemented**, med testbevis i P26–P29. Se
[design, filkart og ABI](docs/design/mermaid-integration.md), FTR-010 og FUNC-023–025.
Alle planlagte symboler nedenfor skal forbli Planned til de finnes. Hver fase får
egen branch og commits per milestone; ingen fase behandles som sprint.

| Fase / branch | Milestones | Utgangskriterium |
| --- | --- | --- |
| P25 `phase/p25-mermaid-design` | M1: krav, blueprints, kildeundersøkelse og kontraktsutkast; M2: plumbingreview, eierskap og tekniske gates; M3: dokumentert konsistenskontroll og designgjennomgang | Dokumenterte beslutninger, risiko og gates; kun design, Proposed. |
| P26 `phase/p26-mermaid-foundation` | M1: låst toolchain/pin/Cargo.lock, minimal C++/Rust-link og lisensoversikt; M2: ren Flowchart-modell, parserprofil og ABI-eierskap; M3: måleseam og ren layout roundtrip med begge brukerfixtures | AT-062/064-prober; ingen renderer→parser-avhengighet, ingen tapte konstruksjoner, målt labelgeometri og akseptabel verste graf. Først da Ready for integrasjon. |
| P27 `phase/p27-mermaid-preview` | M1: cmark-gjerder, kildekart og blokklokal fallback; M2: request keys, worker-lokal font/cache og felles prepare-kjede; M3: native scene/placement og støttet Flowchart-profil | AT-059/063, baseline-regresjoner, stabilt preview ved edits/resize og ingen Rust-layout på GUI-tråden. |
| P28 `phase/p28-mermaid-reading-export` | M1: semantiske paintroller og slider-repaint; M2: DrawRuns, leserekkefølge, markering og ankre; M3: A4 og felles PDF-geometri | AT-060/061, begge temaer, Unicode, vektor-PDF og faktisk tekstuttrekk. |
| P29 `phase/p29-mermaid-verification` | M1: full profil-/failure-/fuzzmatrise og dependency-check; M2: native skjermbilder, ytelse og offline/installasjonsprøve; M3: bevisrapport, grønn CI og installasjon | AT-059–064 samlet; ingen Verified uten identifisert testbevis. |

P26-gaten er konkret: mål-seam finnes ikke som offentlig upstream-port i undersøkt
pin. Vis kontrollert node-/edge-/gruppe-måling med Pango-data, ingen stille syntax-
tap og håndterbar kjøretid for 128 noder/512 kanter. Ved behov for stor upstream-fork
eller prosessisolasjon revideres designet før P27; ikke skjul reparsing i renderer.
Rust har en versjonsbundet kooperativ tosekunders deadline i tillegg til
checkpoint før/etter kall; det er ikke hard tidsavbrudd.

De planlagte testnavnene er konsolidert til Rust-profil-/FFI-tester,
DiagramLayoutTest, DiagramPreparationTest, DiagramWorkerTest, DiagramReadingTest,
MermaidGuiTest og MermaidPdfTest. MermaidDependencies kontrollerer låst graf;
vanlige lag-/blueprint-/regresjonskontroller beholdes.

Utvidelse utover Flowchart 1 (bl.a. sequence/class/ER) krever egne modellvarianter,
kravprofil og fixture-dekning; det er ikke en skjult del av P27.

P25 M1 `60a63aa`, M2 `f0cdb41`; M3 samler
[designgjennomgang og kontroller](docs/evidence/P25.md). P26 er implementert og lokal gate består; M1 `1e13664`, M2 `68b9c01`.
M3 bevis samles i [P26](docs/evidence/P26.md). P27–P29 er implementert; verifikasjon og integrasjon står nedenfor.

P27 M1 `012361c`, M2 `7aedb48`; M3 samler native preview og regresjon.
Alle 55 lokale tester består; se [P27](docs/evidence/P27.md).

P28 M1 `6d85642`, M2 dokumenterer native clipboard; M3 verifiserer
A4 og tekst-/vektor-PDF. Se [P28](docs/evidence/P28.md).

P29 M1 `a19bb17` styrker ABI/feil og dependency-kontroll. M2/M3 samler
sluttverifikasjon, offline/installasjon og CI i [P29](docs/evidence/P29.md).


P29 M2 `9b0156b` kontrollerer offline og native installasjon. Rettelsen
`168e6a1` skiller produksjons- og instrumenteringsbudsjett. M3 samler
faktiske plumbing-symboler, flersidet PDF og testbevis: Release 59/59 og
Debug ASan/UBSan/LSan 57/57, med berørte tester kjørt igjen etter utvidelser.
P26–P28 er integrert via PR 19–21; P29 går gjennom samme CI før merge/installasjon.

## P30: Mermaid-layout uten overdreven fristkontroll

Branch: `phase/p30-mermaid-layout-performance`. UR-039/SR-022, FUNC-024.

- M1: reproduser brukerens sporbarhetsdiagram (13 noder / 17 kanter), lokaliser kostnaden og legg til regresjonsfixture.
- M2: reduser klokkeoverhead i indre geometri uten endret geometri eller lengre tidsbudsjett; behold frist- og TLS-kontroll.
- M3: verifiser native preview, PDF, Release/sanitizere og CI før merge og atomisk installasjon.

P30 M1 `6217bcb`, M2 `dc17db4`; M3 samler [testbevis](docs/evidence/P30.md).


## P31 — Bibliotekets SVG som presentasjonsformat

Branch `phase/p31-mermaid-svg`. M1: revider UR-041/FUNC-024/025 og payload.
M2: ferdig SVG, librsvg/Cairo og én visual-run; behold gammel ruter/pin.
M3: CTest, visuell Light/Dark og vektor-PDF. Se
[beslutningen](docs/design/mermaid-svg-routing.md).

## P32 — Libavoid fra låst bibliotekfork

Branch `phase/p32-libavoid-integration`. M1: forkens måleseam og sammenligningsbevis;
M2: konkret pin, backendvalg, diagnostikk og bygg/lisens; M3: regresjoner,
SVG/PNG, tidsmåling, CI/integrasjonskontroll. Bibliotekarbeidet ligger på
forkens `phase/libavoid-routing`, med separate milestone-commits.

P31/P32 er gjennomført på separate brancher og PR 24/25. P31 består av
`ae00de1`, `d789d54`, `9a7156e` og livsløpsrettelsen `35e70b7`. P32 integrerer
fork-pin `6ff5a54ce39d912e399493ec4639a7b8ff8bdc7f` via `59f885f`/`0ec9b2b`;
P31-rettelsen er tatt inn før sluttesten. Begge faser har grønne CI-jobber
med og uten sanitizer. Se [samlet bevis, commits og begrensninger](docs/evidence/P32.md).

## P36 — typed dekning og første sekvensprofil

Branch `phase/p36-sequence-foundation`. M1: undersøk pin/upstream og dokumenter
matrise, semantiske grenser og roadmap. M2: SequenceModel, versjonert wire,
streng Sequence 1-profil, separat layoutdispatch og SVG. M3: APT-fixture,
negative syntakstester, mixed Markdown, preview/PDF og dokumentert evidens.
Videre sekvensutvidelser og typeprioritet følger [matrisen](mermaid_coverage.md).

P36: M1 `2f94be1`, M2 `39a6a14`; M3 samler [sluttbevis](docs/evidence/P36.md).
Sequence 2 og de prioriterte familiene er senere implementert i P37–P41 nedenfor.

## Sammenhengende dekning P37–P41

Brukerbestilt 2026-09-18. Hver phase har egen branch og milestone-commits.
`mermaid_evicence.md` i roten er den løpende, praktiske lesefilen; bare støttede
profiler legges inn. Bygg `build/xfmd` etter hvert inkrement.

- P37 `phase/p37-sequence-events`: ordnet event-layout, async og nested fragmenter.
- P38 `phase/p38-semantic-diagrams`: state, class, requirement og ER med typed kontrakter.
- P39 `phase/p39-architecture-diagrams`: C4 context/container/component, architecture og block.
- P40 `phase/p40-planning-diagrams`: packet, timeline, Gantt og journey.
- P41 `phase/p41-mermaid-coverage`: øvrige annonserte diagramtyper, samlet evidens/PDF og matrise.

For hver fase: M1 krav/blueprint/kontrakt, M2 implementasjon, M3 positiv/negativ
profilkontroll, faktisk SVG/preview/PDF og oppdatering av lesefilen. Ingen
Mermaid-kilde eller bibliotekeid Graph flyttes inn i layoutkontrakten.

P37–P41 er implementert: M2 `70ab7b0`, `ab5b61d`, `3e46b04`, `7e32a86` og
`071a876`. P41 samler sluttkontrollen i [P41](docs/evidence/P41.md),
med 29 praktiske eksempler for 23 eksplisitte delprofiler. Hver fase ligger på
egen branch. Brukeren har godkjent galleriet; fasene er integrert i `main`,
alle PR-er er lukket og versjonen er installert.
[gjeldende integrasjonsstatus](docs/evidence/P41-integration.md) beskriver
faktisk PR-/CI-/installasjonsstatus. Ukjente konstruksjoner gir
synlig kildefallback; bred type-dekning er ikke full syntakskompatibilitet.
