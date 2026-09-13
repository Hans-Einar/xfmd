# Arbeidsmåte: krav, features og functionality

Status: Anvendt i P0–P7. Metoden er et praktisk eksperiment for agentarbeid. Den bruker
stabile designobjekter og eksplisitte endringsregler, uten sprintseremonier eller
en foreløpig SDL-kompilator. Det er ikke en implementasjon av SDP, og andre
repositoryer er ikke brukt som autoritative kilder.

## 1. Hva som er et designobjekt

En **feature** gir et sammenhengende resultat med egen akseptanse og livsløp.
En **functionality** er en avgrenset tjeneste, mekanisme eller arbeidsflyt med ett
eierlag og eksplisitt offentlig kontrakt. En C++-metode er en implementasjonsdetalj;
vi oppretter ikke et dokument per metode. Ett use case kan bruke flere features
og direkte functionality. Ikke lag en tom feature bare for å fylle hierarkiet.

To uavhengige egenskaper hindrer at «synlig for brukeren» blir eneste kriterium:

| Felt | Verdier | Bruk |
| --- | --- | --- |
| Kind | Feature, Functionality | Sammenhengende evne eller avgrenset tjeneste. |
| Audience | User, System, Integration | Hvem som observerer/bruker resultatet. |
| Role (functionality) | Workflow, Service, Adapter, Mechanism | Orkestrering, lokal tjeneste, teknologikobling eller intern algoritme. |
| Owner | application, interpreter, renderer | Ett lag som eier objektets kontrakt/atferd. |
| Scope | FirstRelease, Future | Planlagt leveranse, uavhengig av designstatus. |

En systemfeature er tillatt når den har en samlet, kravfestet evne og egen
akseptanse. Det er ikke et påskudd for å kalle hver klasse en feature. Kontrakter
har egne filer, men er ikke funksjonelle eiere.

## 2. Eksempler og grensevalg

- Markdown-presentasjon er feature; tolkning, layout og FOX-tegning er functionality.
- Live preview er feature; debounce og dokumentrevisjoner er delt functionality.
- Lenker + tilbake/frem + posisjonsgjenoppretting er navigasjonsfeature.
- Synkronisert scrolling er feature; ankerkonvertering er gjenbrukbar functionality.
- Load/save er dokument-/lagringstjenester, ikke separate features.
- Splitter, view mode og sidepanel er workspace-functionality.
- Encoding-/inputvalidering er delt tjeneste. En framtidig lint-feature må gi
  brukeren konkrete diagnoser og ha egne krav; tolerant Markdown er ikke en
  streng grammatikksjekk som skal blokkere lagring.

Fire features og elleve functionality-objekter er implementert første baseline. Tallet er ikke
et mål. Slå sammen objekter med samme ansvar; splitt bare når eierskap, kontrakt,
endringsårsak eller selvstendig akseptanse begrunner det.

## 3. ID-er, filer og sporbarhet

Bruk `UR-001`, `SR-001`, `FTR-001`, `FUNC-001` og `AT-001`. Neste ledige nummer
reserveres i registeret; omnummerer aldri eksisterende objekter. Filer heter
`Feature-001--Markdown-Presentation.md` og `Functionality-001--Document-Session.md`.
Titler/slugs kan endres med oppdaterte lenker; ID-en beholder betydning.

Sporbarhet går **UC → UR/SR → FTR/FUNC → kontrakt/kall → AT → faktisk testbevis**.
UR/SR kan gå direkte til functionality. Hvert objekt har maskinlesbar metadata og
åtte faste kapitler; kapittel 5 heter alltid `Plumbing`. Se [malene](../src/blueprint/templates/README.md).
Registeret gir motsatt vei fra krav til objekter. Alle funksjonaliteter må ha minst
ett krav. Cross-cutting krav arves ikke usynlig: oppgi dem eksplisitt der de gjelder.

## 4. Obligatorisk arbeid før og under en endring

1. Les krav/arkitektur og søk i registeret etter eksisterende eiere og tjenester.
2. Identifiser krav, scope, feilvei og observerbar akseptanse; oppdater krav ved ny atferd.
3. Oppdater berørte blueprints før kode. Tegn faktisk kallretning i kapittel 5.
4. Beskriv konsekvenser for data, revisjon, eierskap, feil og eksisterende konsumenter.
5. Implementer i oppgitte kildefiler. Avvik rettes i blueprint i samme commit/PR.
6. Kjør relevante tester og validator; oppgi kommando, input, utfall og commit.
7. Kontroller at navngitte symboler finnes og at dokumenterte kall faktisk skjer.

En brukerbestilt avgrenset implementering trenger ikke ny godkjenning for hvert
steg. Ikke bruk metoden som grunn til å stoppe rutinearbeid. Vesentlige endringer
i produktomfang eller kontrakters betydning skal derimot beskrives og avklares.
Designgrunnlaget ble gjennomgått før implementasjon; videre endringer følger samme sporbarhet.

## 5. Plumbing som gjenbrukskart

Hver rad viser kildehendelse/kaller, kalt offentlig symbol, kildefil, data/resultat
og feil/sideeffekt. Marker `Planned` eller `Implemented` eksplisitt. FTR-tabeller
viser ende-til-ende-forløp; FUNC-tabeller viser tjenestens interne vei og offentlige
innganger. Pek til eierens blueprint fremfor å kopiere hele kontrakten.

Eksempel: en framtidig eksportfeature som trenger parsing bruker
`IInterpreter::parse`; den kaller ikke en privat metode på PreviewCoordinator.
Ny gjenbruk begrunnes med konsument og krav. Flytt delt semantikk til riktig
functionality-eier, fjern duplikatet og oppdater begge kallkart. Unngå generell
service bus, runtime plugin-system og «Utils» som skjuler avhengigheter.

## 6. Status og bevis

`Proposed → Ready → Implemented → Verified`; `Retired` er avsluttet objekt.

- Proposed: designforslag; ubesvarte spørsmål kan stå eksplisitt.
- Ready: krav, kontrakter, feilvei og testplan er avklart; gate-bevis er lenket.
- Implemented: symbolene finnes, men samlet akseptanse er ikke nødvendigvis verifisert.
- Verified: alle relevante AT-er har testbevis mot identifisert commit/miljø.

Status og leveransescope er separate. Future-krav kan stå utsatt i registeret uten
spekulative stubs. Ved ny atferd settes berørt objekt tilbake til Proposed/Ready;
eldre bevis beholdes som historikk, ikke som bevis for nye krav. Testfeil blokkerer
Verified, ikke nødvendigvis uavhengig arbeid.

## 7. Minimumskontroll og videre forbedring

`python3 tools/validate_blueprints.py` sjekker metadata, ID-er, lokale fillenker,
kapitler, kravdekning, avhengigheter og planlagt/implementert plumbing-markering.
Dette er strukturkontroll; den beviser ikke korrekt kode, kravkvalitet eller at
metoder faktisk kaller hverandre. Semantisk review og relevante tester er obligatorisk.

`python3 tools/check_blueprint_symbols.py` kontrollerer at 83 dokumenterte
Implemented-kall har navngitte callee-symboler i oppgitte filer. Kontrollen leser
tekst, ikke AST, og beviser ikke at caller faktisk kaller callee. P7 avdekket gamle
caller-navn fra tidligere faser; disse ble rettet ved manuell gjennomgang.

Videre forbedringer etter første implementasjon:

- Mål hvor ofte plumbing driver fra kode før vi lager en AST-basert symbolkontroll.
- Legg bare strukturerte kontraktfelt til malen når de fjerner en konkret tvetydighet.
- Innfør korte beslutningslogger ved reelle veivalg, ikke et dokument per småvalg.
- Vurder senere generering fra et lite schema når begrepene har vært prøvd i praksis.

Målet er minst mulig **duplisert** functionality og dokumentasjon, ikke færrest
mulig nødvendige tjenester. Metoden skal gjøre neste agent i stand til å finne
riktig vei gjennom systemet uten å måtte gjette eller gjenoppfinne den.

## Revisjon 1.1

[Designrevisjonen](../softwareDesign.md) utvider P0–P8-baseline med to features og
seks functionality-objekter (7 og 19 totalt). Nye objekter har scope Future og
status Proposed. Berørte eksisterende objekter beholder FirstRelease som opprinnelig
scope, men får Proposed-status for endret kontrakt. Deres Implemented plumbing og
eldre bevis gjelder baseline; nye Planned-rader og akseptanse må verifiseres separat.
Faseplanen P9–P13 beskriver neste leveranse; Future betyr ikke ubestemt utsettelse.
