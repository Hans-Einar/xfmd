# Implementeringsplan: xfmd

Status: **P0–P7 fullført**, 2026-09-13. Brukeren godkjente
designgrunnlaget og autoriserte første leveranse i én sesjon. En phase er en
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
