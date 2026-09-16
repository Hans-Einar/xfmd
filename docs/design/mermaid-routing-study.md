# Rutestudie: begrenset side- og portsøk for Mermaid

**Senere beslutning:** Bruk en etablert libavoid-backend i eksisterende fork,
ikke studiens 3×3-søk som hovedløsning. Se [P31/P32](mermaid-svg-routing.md).
Forken finnes nå i `Hans-Einar/mermaid-rs-renderer`; analysen nedenfor beholdes
som historisk grunnlag.

Status: **Forslag og kildekodeanalyse**, 2026-09-16. Ingen ny ruter er implementert,
og ingen fork er opprettet. Dokumentet skiller brukerens prinsipp, eksisterende
mekanismer og anbefalte presiseringer. Beslutning om fork tas etter gjennomgang.

Grunnlag: [krav](../../xfmd_requirements.md), [arkitektur](../../softwareArchitecture.md),
[Mermaid-design](mermaid-integration.md), [FUNC-024](../../src/blueprint/functionality/Functionality-024--Diagram-Layout.md)
og brukerens [sporbarhetsgraf](mermaid/traceability.mmd), med 13 noder og 17 kanter.
Undersøkt bibliotek: `mermaid-rs-renderer` 0.3.1, eksakt upstream-commit
`3726ccbffe0e8032361eb9668694b24f77858060`. Dette er ikke en analyse av en senere
master-versjon. Kildehenvisningene nederst peker på upatchet upstream ved denne pinnen.
XFMDs lokale patch legger til tekstmåling og fristkontroll; den er ikke en egen ruter.

## 1. Konklusjon og avgrensning

Prinsippet er gjennomførbart: velg blant få sidepar, beregn faktiske ruter,
finjuster porter på lovende sider, og bruk et begrenset antall forbedringspass.
Det krever ikke rekursiv utprøving av alle rutekombinasjoner i diagrammet.
Det gir heller ingen garanti for globalt korteste eller kryssingsfri tegning.

En viktig korrigering: **biblioteket optimaliserer allerede lengde og svinger**.
Det har A*, alternative sidepar, portfinjustering, belastningskart og omruting.
Problemet er blant annet at flere ulike kostnadsfunksjoner og regler konkurrerer.
I noen trinn prioriteres kryssinger eller svinger foran lengde, og lange omveier
kan dermed vinne. Etterbehandling kan dessuten endre en tidligere god rute.
Det er ikke bevist hvilket trinn som skaper hver omvei i brukerens konkrete graf.
Dette må måles med rutelogging, ikke utledes bare fra skjermbildet.

Studien gjelder **kantruting med faste nodebokser**. Dårlig nodeplassering kan
ikke alltid repareres med bedre ruting. Etiketter må også få plass: kortere
kanter alene garanterer ikke at lange tekster blir lesbare.

Brukeren har nedprioritert merking av diagramtekst. Videre presentasjonsmål er
bibliotekets ferdige SVG, vist gjennom en SVG-leser til Cairo, uten egne
XFMD/Pango-tekstobjekter per etikett. Gjeldende UR-041 og native scene er fortsatt
implementert baseline; krav og blueprints må revideres før en SVG-omlegging.
Dette dokumentet markerer ikke omleggingen som implementert.

## 2. Brukerens foreslåtte algoritme

### 2.1 Begreper og hovedsøk

En **node** er en grafisk enhet. En **port** er et tilkoblingspunkt på grensen.
Vi bruker port om brukerens «entry/exit node» for å skille det fra grafnoden.
En rektangulær node har fire hovedporter: midt på topp, høyre, bunn og venstre.

For hver kant fra A til B:

1. Finn det geometrisk nærmeste hovedportparet mellom A og B.
2. Beregn en hindringsfri rute mellom disse portene.
3. Prøv de to tilstøtende sidene på avsenderen mot samme mottakerside.
4. Prøv også mottakerens to tilstøtende sider, inkludert kombinasjonene med
   avsenderens nabosider. Sammenlign faktisk rutelengde og antall svinger.
5. Først etter sidesøket vurderes underporter på de valgte sidene.
6. Velg konkrete ledige porter og ruten som gir best samlet resultat.
7. Registrer ruten, slik at senere kanter tar hensyn til plassbruk og minsteavstand.

«Høyre og venstre» tolkes her som nabosidene med og mot klokken rundt formen,
ikke alltid skjermens høyre og venstre. En symmetrisk formalisering av forslaget
er 3 avsendersider × 3 mottakersider = **maksimalt ni hovedportpar**. Den inkluderer
også opprinnelig avsenderside mot mottakerens nabosider. Dette er en presisering
av meldingen, ikke en påstand om at brukeren spesifiserte denne løkkerekkefølgen.

Den motsatte siden på hver node utelates i første omgang. Alle 4 × 4 = 16
hovedportpar er for øvrig også et lite, konstant søk; det dyre er hver rutesøkning,
portfinjusteringen og gjentakelser over mange kanter, ikke tallet 16 alene.

### 2.2 Underporter

Når midtporten ved t=1/2 er brukt, opprettes kandidater ved t=1/4 og t=3/4.
Når en slik kandidat brukes, deles de tilgjengelige intervallene igjen.
Portene har stabil identitet, posisjon og reservasjon; kandidater er ikke reserverte
før en rute er valgt. Hovedporten brukes fortsatt som virtuelt referansepunkt i
sidesøket, selv om den fysiske plassen allerede er opptatt.

Presisering: del mellom nærmeste eksisterende port/grense, ikke blindt helt til
hjørnet hver gang. Ellers kan nye punkter kollidere med porter som allerede finnes.
Hold av hjørnemargin og en minste portavstand. Stopp deling når intervallet er for
lite. Halvering beskriver generering av punkter; den skal ikke bli rekursiv
utprøving av hele diagrammet. Inn- og utgående kanter deler fysisk portkapasitet.

### 2.3 Det som må legges til for en robust implementasjon

- Euklidsk avstand kan finne første sidepar. For rettvinklede ruter er Manhattan-
  avstand en bedre nedre grense for rutelengde. Ingen av dem tar alene hensyn til hindringer.
- Ruten må starte utover fra avsenderens kant og ankomme mottakeren uten å gå
  gjennom boksen. Inkluder de korte portstubbene i både lengde og svingtelling.
- Test faktisk hindringsfri rute, ikke bare avstanden mellom endepunktene.
- Minimumsavstand til parallelle ruter må skilles fra tillatte vinkelrette
  kryssinger. Gjør vi alle tidligere ruter til ugjennomtrengelige hindringer,
  kan vanlige grafer bli umulige å tegne.
- Portområder trenger kontrollerte unntak for inn-/utløp; pilspisser har også bredde.
- En ledig midtport kan gjenbrukes ved omruting av sin egen kant. Fjern midlertidig
  kantens gamle reservasjoner og belastning før vurdering; gjenopprett ved avslag.
- Diamanter, sirkler og sylindre må få formtilpassede porter og normalretninger.
  Fire bokssidemidtpunkter er en første prototype, ikke full Mermaid-støtte.

## 3. Hvordan biblioteket gjør dette i dag

### 3.1 Nodeplassering og kantroller

`Current`, `Dagre` og `Auto` velger nodeplassering; de deler den etterfølgende
rute- og etikettpipelinen. `classify_edge_roles` bruker blant annet rangering og
sterkt sammenhengende komponenter. `is_back_edge` settes når mottakerrang er
mindre enn eller lik avsenderrang. Det betyr ikke nødvendigvis en faktisk syklus.
Back edges kan bevisst legges i ytre korridorer. [S1, S2]

### 3.2 Sidevalg: både geometri og faktisk ruting finnes

`edge_sides` velger en foretrukket horisontal eller vertikal kombinasjon ut fra
sentrene, overlapp og diagramretning. `edge_sides_balanced` vurderer sidebelastning,
Manhattan-avstand og særregler for blant annet huber og ytre ruter. [S3]

Deretter lager `collect_routed_side_candidates` en avgrenset liste av primærpar,
balansert par, horisontalt/vertikalt par og eventuelle forgrenings-/back-edge-par.
`choose_routed_flowchart_sides` ruter kandidatene via
`routed_side_candidate_score` → `route_edge_with_avoidance` og sammenligner dem.
Det er altså allerede mer enn et enkelt valg basert på senteravstand. [S4]

Et viktig utdrag av sidekostnaden er:

```text
100000 × hindringstreff + 20000 × etikettreff
+ 1600 × kryssinger + 70 × overlapp
+ 42 × svinger + 0.09 × lengde
+ belastning, retning, primæravvik og rollebonuser
```

Alt annet likt tilsvarer én sving omtrent 467 lengdeenheter, og én kryssing
omtrent 17 778. Dette er en avledning av vektene **i dette trinnet**, ikke en
universell tillatt omvei. Andre vurderinger og grenser gjelder senere.
Vektene gir en konkret grunn til å undersøke hvorfor lange ruter kan vinne.

### 3.3 Portfordeling og finjustering

`build_routed_edges` grupperer porter per side eller akse, sorterer dem etter
motpartens ideelle posisjon og fordeler dem med kantmargin, separasjon og mulig
rutenettsnapping. Portplassering er dermed ikke begrenset til ett punkt per side.

`port_offset_candidates` foreslår i rekkefølge gjeldende punkt, geometrisk ideelt
punkt, midtpunkt og ideelt punkt ± et steg. Listen avkortes. Ved grense fire og
fire forskjellige tidligere kandidater kommer minusvarianten ikke med; det er
ikke et symmetrisk halveringssystem. `refine_flowchart_ports_with_route_candidates`
prøver kombinasjoner av sider og offsets, med kollisjonsscore. Back edges og
utvalgte forgreningsporter kan låses til sitt tidligere sidepar. [S4]

For grafen med 17 kanter gir `flowchart_routing_performance_profile` nivået
`Exact`: høyst fem sidekandidater, fire offsetkandidater per ende, ingen generell
avkorting av antall raffinerte kanter, to globale pass og to congestion-pass
(17/13 > 1,1). «Exact» er bibliotekets navn; fem sidepar er ikke et uttømmende
16-parsøk. Teoretisk kan portfinjusteringen prøve opptil 5 × 4 × 4 = 80
kandidater per kant, i tillegg til baseline og andre steg; faktiske antall avhenger
av deduplisering og regler. Større grafer bruker strammere profiler.

### 3.4 Rutesøk og kandidatvalg

`route_edge_with_avoidance` lager flere typer rutekandidater, inkludert korridorer,
etikettstyrte omveier og eventuelt ytre fallback. Den kan bruke
`route_edge_with_grid`, som er A* over `(x, y, retning)`. Stegkostnaden inkluderer
lengde, skifte av retning og belastning fra eksisterende ruter. Manhattan-avstand
brukes i estimatet. Standardinnstillinger inkluderer rutenett 16, svingvekt 0,6,
occupancy-vekt 1,2 og `max_steps=160000`; grid-søket bruker minst 10000 som grense.
Dette er en begrensning per søk, ikke hele diagrammets totale arbeid. [S1, S3]

`route_candidate_better` bruker ikke den samme enkle totalscoren som sidevalget.
Hindringstreff og egen etikett kommer tidlig; deretter blant annet kryssinger,
andres etiketter, overlapp og avstand til ønsket etikettpunkt. Lengde og svinger
kommer sent. `prefer_shorter_ties` bytter prioritet mellom dem. Kryssingsvalget
har en eksplisitt regel som begrenser uforholdsmessige omveier, så det er feil å
si at biblioteket aldri vurderer problemet. Regelen gjelder ikke automatisk
alle de andre sammenlignerne. [S3]

### 3.5 Avstand mellom ruter og etterbehandling

`EdgeOccupancy::add_path_with_weight` fordeler belastning til rutenettcellen og
åtte naboceller, med vekter 3/2/1. A* kan straffe slike områder. Dette er en myk
belastningsmodell, ikke et bevis på eksakt geometrisk minsteavstand. [S3]

`optimize_flowchart_routes_globally` omruter mot andre segmenter.
`negotiate_flowchart_route_congestion` bruker også historisk belastning.
`global_route_score_is_better` sammenligner hardfeil, endpoint-reentries,
andre nodetreff, etikettreff, kryssinger, overlapp, svinger og til slutt lengde.
Dermed kan færre svinger vinne over kortere rute i dette steget. [S4]

Deretter følger opprydding og reparasjon: kryssinger, node-/subgraph-hindringer,
endepunkter og akseoscillasjoner. En endpoint-reparasjon kan prøve alle 16 sidepar,
men bare når baseline har relevante feil. Til slutt kjører separat
`resolve_all_label_positions`, som igjen søker og flytter etiketter. [S4–S6]

## 4. Anbefalt konkretisering av forslaget

Dette avsnittet er **designforslag**, ikke eksisterende kode eller ferdig kalibrerte tall.

### 4.1 Én sammenhengende kostnadspolicy

Avvis ulovlige nodegjennomganger, ugyldige portretninger og brudd på den valgte
parallelle minimumsavstanden. For gyldige kandidater brukes samme policy i
sidevalg, underportvalg, omruting og sluttkontroll:

```text
cost(route) = L + bendCost × B + crossingCost × X + congestionCost × C
```

`L` er total rutelengde, `B` antall 90-graders svinger, `X` tillatte kryssinger
og `C` en definert ekstra nærhetsbelastning utenfor den harde klaringen.
Vektene uttrykkes i lengdeenheter og kalibreres mot eksempler. Færrest svinger
skal ikke være et leksikografisk kriterium foran all lengde: én spart sving
må ha en begrenset pris. Bruk stabile ID-er som siste tie-break.

Etikettkollisjoner er fortsatt uakseptable i sluttresultatet, men plassering av
lange etiketter bør ikke usynlig tvinge ruten gjennom et dårlig forhåndsvalgt
punkt. Reserver plass, prøv et avgrenset sett etikettposisjoner og vurder rutens
sluttkostnad igjen. Rapportér uløselig plassmangel eksplisitt; ikke skjul etiketter.

### 4.2 Avgrenset søk og pseudokode

Forslag til startverdier: ni sidepar, behold K=2, høyst Q=3 konkrete porter per
ende og valgt side, høyst to forbedringspass. Disse tallene må måles.

```text
for edge in stableRoutingOrder:
    temporarilyRemoveOwnReservations(edge)
    pairs = closestMainPairAndAdjacentPairs(edge)       # <= 9
    mainRoutes = routeWithBudget(pairs, obstacles, occupancy)
    shortlist = bestKFeasibleSidePairs(mainRoutes, K=2)
    candidates = refineAvailablePorts(shortlist, Q=3)   # <= 18 rutesøk
    if noFeasibleResult or excessiveDetour:
        testRemainingMainPairs()                       # <= 7 ekstra
        refineBestNewPairs(K=2, Q=3)                    # <= 18 ekstra
    acceptBestValidCandidateOrRestorePreviousRoute()
    commitPortsAndOccupancy()

repeat at most 2 improvement passes:
    revisitOnlyConflictingOrLongDetourEdges()
    acceptOnlyPolicyImprovementWithoutBreakingValidity()
```

Hovedportsøk bruker virtuelle sentre; en slik rute kan ikke publiseres dersom
porten er opptatt. Den må erstattes av en gyldig konkret underport-rute.
En hindring kan gjøre en lang rute nødvendig. Omveisgrensen utløser et ekstra
søk; den må ikke forkaste eneste gyldige rute bare fordi den er lang.

Portgeneratoren vedlikeholder ledige intervaller iterativt, med dybde-/antallsgrense
og minsteavstand. Den returnerer høyst Q kandidater nær ønsket posisjon, inklusive
eventuell gyldig nåværende port. Ikke bygg alle kombinasjoner av underporter først.
Porter reservert av andre kanter flyttes ikke rekursivt; forbedringspasset kan
frigjøre og reforhandle én kant av gangen.

Med disse grensene er det høyst 52 rutesøk per kant per pass, inklusive fallback,
og høyst 156 over førstegangsruting og to forbedringspass. Dette er et tak for
port-/rutesøket i pseudokoden, ikke for etikettarbeid eller hele biblioteket.
Hvis hvert A*-søk har høyst G ekspanderte tilstander og fire naboer, er søkearbeidet
omtrent `O(156 × E × G log G)`, i tillegg til hindrings-/belastningsspørringer og
oppbygging av graf/rutenett. G og total diagramtid må også begrenses. En konstant
kandidatgrense alene er ingen ytelsesgaranti.

En rask test av direkte, L- og enkle Z-ruter kan gå foran A*. En nedre grense
på Manhattan-lengde kan avskjære kandidater som allerede er dyrere enn beste
akseptable kostnad, så lenge alle kostnadsledd er ikke-negative. Dette er
branch-and-bound lokalt per kant, ikke rekursiv global enumerasjon.

### 4.3 Stabilitet og vanskelige tilfeller

Behandle trange eller eksplisitt begrensede porter først, med deterministisk
rekkefølge. Test også om kantdeklarasjonsrekkefølge påvirker resultatet for mye.
En grådig rekkefølge kan låse senere kanter ute; et lite rip-up/reroute-pass
med reservasjonstilbakerulling er derfor nyttig.

Hold nodeposisjoner fast i første prototype for å måle ruteren isolert.
Test begge retninger, samme-side-kanter, self-loops, parallelle og motsattrettede
kanter, subgraphs og porter nær hjørner. Dersom ingen rute med nødvendig klaring
finnes, må politikken velge mellom større nodeavstand, eksplisitt degradert klaring
eller rapportert feil. Ingen av disse valgene skal skje stille.

## 5. Plumbing: eksisterende innganger og planlagte endringspunkter

Alle filstier i denne tabellen er relative til upstream `src/`.
Planlagte navn er forslag, ikke eksisterende symboler.

| Status | Kaller → kalt symbol | Fil | Data og hensikt |
| --- | --- | --- | --- |
| Eksisterende | `build_routed_edges` → `choose_routed_flowchart_sides` | `layout/flowchart/edge_pipeline.rs` | Nodebokser/roller → sidepar gjennom faktisk rutekostnad. |
| Eksisterende | `choose_routed_flowchart_sides` → `routed_side_candidate_score` → `route_edge_with_avoidance` | `layout/flowchart/edge_pipeline.rs`, `layout/routing.rs` | Kandidatpar → rute og konkurrerende score. |
| Eksisterende | `build_routed_edges` → `refine_flowchart_ports_with_route_candidates` | `layout/flowchart/edge_pipeline.rs` | Sidepar og offsets → finjusterte porter. |
| Eksisterende | `route_edge_with_avoidance` → `route_edge_with_grid` | `layout/routing.rs` | Hindringer/belastning → begrenset A*. Grid er én av rutemulighetene. |
| Eksisterende | `build_routed_edges` → globale pass → `post_route::apply_edge_path_cleanup` | `layout/flowchart/edge_pipeline.rs`, `layout/flowchart/post_route.rs` | Ruter → omruting og etterbehandling. |
| Eksisterende | `compute_layout_with_metrics` → `resolve_all_label_positions` | `layout/mod.rs`, `layout/label_placement.rs` | Ferdig layout → siste etikettplassering. |
| Planlagt | Side-/portvalg → `BoundedPortSearch` | ny `layout/flowchart/port_search.rs` | Systematiske nabosidepar, shortlist og endelig portreservasjon. |
| Planlagt | Alle kandidatvalg → `RouteCostPolicy` | ny `layout/flowchart/route_cost.rs` | Én dokumentert gyldighets- og kostnadspolicy. |
| Planlagt | Portvalg/omruting → `PortReservationTable` | ny `layout/flowchart/port_reservations.rs` | Stabile porter, ledige intervaller og rollback. |
| Planlagt | Rutesøk/sluttvalidering → `RouteClearanceIndex` | ny `layout/flowchart/route_clearance.rs` | Eksakt parallell klaring og separat kryssingspolicy. |

Gjenbruk eksisterende A*, hindringsgeometri og SVG-renderer så langt de passer.
Ingen rutelogikk legges i XFMDs FOX-vinduer. Ikke la eksisterende sluttpass
overskrive den nye kostnadspolicyen ubemerket: logg og valider geometri før/etter
hvert pass, og tilpass eller velg bort uforenlige pass for den nye strategien.

## 6. Sammenligning og vurdering

| Tema | Brukerens prinsipp | Biblioteket i dag | Foreslått endring |
| --- | --- | --- | --- |
| Hovedporter | Nærmeste par, så nabosider | Heuristisk kandidatsett og faktiske prøveruter | Systematisk, symmetrisk 3×3, eventuell 16-par-fallback |
| Underporter | Halvering av ledige sideintervaller | Gruppefordeling og avkortet offsetliste | Stabile reservasjoner og begrenset intervallsøk |
| Lengde/svinger | Sentrale mål for hele ruten | Finnes, men med ulik prioritet i ulike steg | Felles mål med endelig kostnadskontroll |
| Parallelle ruter | Minsteavstand | Myk occupancy + reparasjon/omruting | Eksakt klaring der mulig, egen policy for kryssinger |
| Arbeidsmengde | Lite kandidatsøk | Allerede flere begrensede profiler og pass | Én samlet målt arbeidsgrense, ikke bare flere heuristikker |
| Etiketter | Ikke detaljert i forslaget | Egen kompleks sisteplassering | Må inngå i akseptanse og endelig validering |

Forslaget er verdt en prototype, men biblioteket har allerede mye av strukturen.
Den største gevinsten kan være en konsistent policy og bedre kandidatprioritering,
ikke en helt ny A*-motor. En blind økning av antall kandidater kan gjøre både
kostnaden og samspillet med etterbehandling verre.

## 7. Fork, utvikling og mulig upstream-PR

Hvis vi beslutter implementasjon, opprettes en **egen fork av mermaid-rs-renderer**.
Algoritmeendringen skal ikke vokse som en skjult patch under XFMDs `.deps/`.

1. Registrer upstream-pinnen over som reproduserbar baseline. Sammenlign deretter
   med daværende upstream HEAD før valg av fork-base; ikke anta at funnene gjelder uendret.
2. Legg først til fixture og instrumentering: hvilke side-/portpar ble prøvd,
   delkostnader, forkastelsesgrunn og geometri etter hvert pass.
3. Innfør en valgfri routing-strategi, separat fra `Current/Dagre/Auto` som velger
   nodeplassering. Behold gammel strategi for sammenligning og kompatibilitet.
4. Implementer policy og side-/underportsøk i små moduler. Gjenbruk eksisterende
   rutemekanikk; dokumenter avvik for self-loops, former og subgraphs.
5. Lever regresjoner og målinger i bibliotekets egne tester. Hold XFMDs tekstmåle-
   seam, fristkontroll og adapterendringer separate fra algoritme-PR-en.
6. Test upstreams standard SVG direkte før integrasjon i XFMD. Ingen påstand om
   forbedring baseres bare på at XML er gyldig eller at testen ikke krasjer.
7. Pin XFMD til en konkret fork-commit med ny arkivhash og lisens-/buildkontroll.
   Revider krav, FUNC-024 og relevant presentasjonsplumbing for SVG-retningen.
8. Foreslå en avgrenset upstream-PR med før/etter-SVG, målinger, konfigurasjon og
   begrunnelser. Aksept eller senere merge hos upstream er ikke garantert.

Fork/PR er betinget av videre beslutning; ingen ekstern publisering utføres som
del av denne dokumentasjonsoppgaven.

## 8. Eksperiment og akseptansekriterier

Sammenlign samme nodebokser og tekstmål med gammel og ny routing. Gjør deretter
en separat ende-til-ende-sammenligning med Current/Dagre, og behold Graphviz-
prøven som visuell referanse. Forskjellige nodeposisjoner må ikke feiltolkes som
ren ruteforbedring.

Mål for hver kant og hele grafen:

- total lengde og svinger, inklusive portstubber; lengde/nedre Manhattan-grense
  som diagnostikk, ikke bevis på optimalitet;
- kryssinger, parallelle klaringsbrudd, gjennomgang av noder, portkollisjoner,
  etikett–node-/etikett–etikett-overlapp og uklare etikett–kant-tilknytninger;
- kandidater, A*-ekspansjoner, rutetid og etikettid separat, samt total tid og minne;
- hvilke etterbehandlingspass som økte lengde/svinger, med konkret begrunnelse;
- determinisme ved gjentakelse og følsomhet for kantrekkefølge.

Første testsett: sporbarhetsgrafen, `service-map.mmd`, `layer-delivery.mmd`,
fire sider × fire sider rundt én hindring, blokkert nærmeste par, tett fan-in/out,
smal korridor, toveis/parallelle kanter, syklus/self-loop, nested subgraph,
diamant/sirkel og lange etiketter. Ta med en umulig klaringssituasjon og avbrudd.

Akseptanse før integrasjon: ingen skjulte hardfeil eller teksttap; synlig bedre
sporbarhetsgraf uten urimelige omveier; målbar lengde-/svingforbedring uten å kjøpe
uakseptable kryssinger; deterministiske grenser og kontrollert feilvei; ingen
vesentlig regresjon i resten av testsettet. Tallfest forbedringsmål etter baseline-
målingen, før justering av vekter. Produksjonsbudsjett og brukerens SVG-prioritering
må være eksplisitte i den reviderte planen. Ingen ny ruter er foreløpig verifisert.

## Kilder ved låst upstream-versjon

- [S1: Konfigurasjon](https://github.com/1jehuang/mermaid-rs-renderer/blob/3726ccbffe0e8032361eb9668694b24f77858060/src/config.rs#L849).
- [S2: Kantroller](https://github.com/1jehuang/mermaid-rs-renderer/blob/3726ccbffe0e8032361eb9668694b24f77858060/src/layout/flowchart/roles.rs#L16).
- [S3: Ruting og kostnad](https://github.com/1jehuang/mermaid-rs-renderer/blob/3726ccbffe0e8032361eb9668694b24f77858060/src/layout/routing.rs#L565).
- [S4: Sidevalg, porter og globale pass](https://github.com/1jehuang/mermaid-rs-renderer/blob/3726ccbffe0e8032361eb9668694b24f77858060/src/layout/flowchart/edge_pipeline.rs#L793).
- [S5: Etikettplassering](https://github.com/1jehuang/mermaid-rs-renderer/blob/3726ccbffe0e8032361eb9668694b24f77858060/src/layout/label_placement.rs#L256).
- [S6: Siste etikettpass](https://github.com/1jehuang/mermaid-rs-renderer/blob/3726ccbffe0e8032361eb9668694b24f77858060/src/layout/mod.rs#L224).
