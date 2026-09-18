# P34: Beskyttet etikettilhørighet

Krav: SR-024. Eier: FUNC-024 / bibliotekforkens etikettsteg. DiagramPainter
viser fortsatt bare ferdig SVG; ingen ny rutelogikk i XFMD eller `.deps`.

Etiketten knyttes til nærmeste ortogonale segment med overlappende projeksjon.
Det åpne rektangelet mellom tekstboksen (inkludert tekstmargin) og segmentet
er beskyttet mot alle andre kanter. Identiteten er kantindeks, ikke kilde/mål.
Plassering, hindringsregistrering og sluttvalidering bruker samme geometri.

Forkens `routed::attachment` eier sonen og hindringsutvidelsen. `labels::place`
prioriterer fri tilhørighet; inntil fire faste etikettordener kan prøves.
`labels::obstacles` utvider teksthindringen mot eiersegmentet, fratrukket
nøyaktig libavoids globale hindringsmargin. Dermed blokkeres mellomrommet uten
å blokkere eiersegmentet. `labels::validate` kontrollerer faktiske sluttruter;
`quality::measure` rapporterer `attachment_intrusions`. Eksisterende grense på
tre rutetransaksjoner og siste etikettpass beholdes. Feil gir NoSpace og vanlig
synlig fallback; ingen skjult motorendring. Manglende gyldig portretning
etter hindringsruting klassifiseres som NoSpace og kan bruke eksisterende, ene
omplasseringsretry. Portkontrollen slakkes ikke.

Sammenfallende ruter på sonegrensen og minste parallellavstand er separate
begrensninger. Denne endringen lover ikke å løse alle parallelle overlapp.
Nøyaktig hindringsbuffer er viktig: et ekstra toleransegap lot libavoid legge
en fremmed forbindelse i åpningen; regresjonen avviser dette.

## Mulig portsøk senere

Libavoid mottar plasserte noder og kilde-/målforbindelser med kandidater eller
låste porter, og finner rutepunktene. Første pass velger porter; senere pass i
adapteren låser valgte koordinater i kantspesifikke pin-klasser. En annen kant
kan ikke velge den klassen, men ulike pin-objekter kan ligge fysisk samme sted.
Et portsøk må derfor føre fysisk belegg og sjekke klaring eksplisitt.

Et mulig separat steg prøver et begrenset kandidatsett, kjører libavoid på nytt
og godtar bare streng samlet forbedring etter tilhørighets-/kollisjonskontroll.
Bruk besøkte konfigurasjoner, maksimal forsøksmengde og tidsbudsjett. Kortere
geometrisk portavstand alene beviser ikke kortere rute eller bedre totalresultat.
Stabilitet betyr et lokalt resultat, ikke globalt optimum. Ikke implementert her.
