# Fase 050 — broker-leases

Branch `sprint/004/phase/050-document-leases`, fra fase 049 d6f75b8.
P050-M1: valgfri brokerregistrering og lease-livstid ved bytte/lukking.
P050-M2: ekte SDL-daemon/XFMD, crash/restart, aktive bilder og fasebygg.
UR-044/SR-027, FUNC-031. Ingen endring i parser/rendering.

## P050-M2 — verifisert

Release-bygg fra 5fa35e3. De tre berørte CTest-prøvene bestod (6,38 s):
native navigasjon, DocumentViewsGuiTest og toprosesses endpointprøve.
Samlet baseline på 63 CTest-prøver var grønn i fase 049.

SDLs `verify_native_broker.py` kjørte ekte Go-daemon, mmdr og dette FOX-bygget:
valgt SVG åpnes med lease; SIGKILL/restart av daemon beholder filene; ugyldig
SDL beholder dokumentet; gyldig kildebytte åpner nytt og frigjør gammel pakke;
WM_DELETE_WINDOW går gjennom vanlig FOX-lukking og frigjør aktiv lease.
Brokerens requestsekvenser bevares ved restart. Opptaket viser nytt dokument
etter reload: [native brokerprøve](broker.png).

Ingen automatisk utløpstid for synlige filer. Manglende release kvitteres ikke
som frigjøring; ved krasjet leser beholdes lease til eksplisitt release.
XFMD forsøker uleverte releases igjen mens vinduet lever; køen begrenser nye
leveringer ved 256 ventende frigjøringer. Det er bedre å avvise et nytt dokument
enn å fjerne bilder som en leser fortsatt kan bruke.
