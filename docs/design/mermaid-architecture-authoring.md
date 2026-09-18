# Arkitekturprofiler — C4, Architecture og Block 1

Status: Implemented, P39. Krav UR-039/040, SR-021/022; FUNC-023/024/025.

C4 context/container er første arkitekturtillegg fordi systemgrenser og ansvar
passer SDL/SDP-dokumentasjon. Component viser interne roller. C4s Container-navn
vedtar ikke at et kandidat-SDL Container-begrep har samme definisjon.
C4-syntaksen er eksperimentell i Mermaid. Architecture-beta viser ressurser,
services og grafiske tilkoblingssider; den uttrykker ikke automatisk alle logiske
porter/Channels. Block-beta passer eksplisitte horisontale lag og grid.

Kontrakten utvides med egne C4-elementer/grenser/relasjoner, architecture-
services/grupper/junctions/portretninger og block-celler/spans/kanter. Disse
bevares i SemanticDiagram uten bibliotekets Graph eller original kilde.
Parserprofiler konsumerer hvert utsagn. Layoutadapteren bygger native C4Data,
Architecture-graf med arch_edge_ports, eller BlockDiagram med eksplisitt grid.
SVG og PDF fortsetter gjennom samme presentasjonsport.

## C4 1

C4Context, C4Container og C4Component. Person, System, Container og Component,
med Db/Queue og _Ext-varianter; Boundary, Enterprise_Boundary, System_Boundary
og Container_Boundary med eksplisitte `{}`. Rel/BiRel og retningsvarianter,
navn, teknologi og beskrivelse. Identiteter deklareres eksplisitt. Styling,
callbacks, tag/link/sprite-parametre, named arguments, Dynamic og Deployment
ligger utenfor første profil. Ingen eksterne ressurser hentes.

## Architecture 1

architecture-beta; group/service med `[navn]`, valgfritt innebygd ikon
(cloud, database, disk, internet, server), service/junction `in` gruppe.
Kanter bevarer L/R/T/B ved begge ender og --/-->/<--/<-->.
Nested grupper, `{group}`-kanter, align og eksterne ikonpakker avvises.
Disse begrensningene følger faktisk pin og adapter, ikke nyere JS-dokumentasjon.

## Block 1

block-beta; columns N (1–16), én deklarasjon per linje, rektangulære celler
med navn og valgfritt :span, space og space:span, samt enkle forbindelser.
Nested block og flere spesialformer/direktiver krever senere profil.
Grid/spans er designinformasjon, ikke en flowchart-plasseringsantagelse.

## Verifikasjon

Separate context/container/component-eksempler, ressurs-/portdiagram og
horisontale lag; modell-roundtrip, avvist syntaks, Unicode/tekst, lys/mørk,
resizing/zoom og lesbar vektor-PDF. Konstruksjoner merkes implementert først
etter faktisk gjennomløp. Galleriet og støttematrisen oppdateres samlet.

Kilder: https://mermaid.js.org/syntax/c4.html,
https://mermaid.js.org/syntax/architecture.html og
https://mermaid.js.org/syntax/block.html.

Native block-piler ender foreløpig bak boksene; korreksjon i forken inngår i neste
pin sammen med packet-layout. C4-relasjoner har bibliotekets enkle routing.
