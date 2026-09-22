# Fase 049 — navigasjon og adressert dokumentvisning

M1: krav, blueprint, to Markdown-paneler, registrert SDL-tool og XFMD1-endpoint.
M2: native og IPC-verifikasjon, fasebygg, push og sprint-PR.
G6-M3 i SDL-planen er konsument. Resultater registreres etter prøvene.

## P049-M2 — faktisk verifikasjon

Release-fasebygg fra 820d86f med CMake/Ninja og systemets FOX 1.6.57,
Pango og Cairo. Alle 63 CTest-prøver bestod (145,69 s), inkludert Rust,
PDF og native GUI. To små oppfølgingsrettelser følger i M2: felles lesefarger
og navigatorens ASCII-headingankre. De tre berørte native prøvene ble kjørt
på nytt og bestod (6,38 s). Blueprint-validator: 37 objekter/69 krav;
symbolkontroll: 239 dokumenterte kall.

DocumentViewsGuiTest bruker ekte X11 press/release på Markdown-lenken og et
reelt registrert Go SDL-verktøy. Hovedpanelet åpner generert arkitektur,
navigatoren bevares, fokusbytte endrer ikke mål. Ugyldig prosjekt/viewpoint
beholder dokumentet. Headingankeret VP02 prøves også.
DocumentEndpointTest kjører to uavhengige FOX-prosesser: riktig vindu/panel,
stale/ugyldig/for stor forespørsel, beholdt dokument ved manglende fil og lukket
mål. [Visuelt kontrollert X11-opptak](navigation.png).

Første IPC-profil er Linux XFMD1/SOCK_SEQPACKET, maks 32 KiB, samme UID,
16 ventende klienter og 256 klient/panel-sekvensspor. Idle peers har 2 s grense.
Ingen global --active-oppslag; avsendervindu velges ved klikk eller --window.
Modellgenerering har én aktiv jobb, prosessgruppekansellering og 65 s grense.
Pakker beholdes i privat runtime-katalog til vinduet lukker. Broker-leases
legges til i neste fase, slik at dokumenttjenesten også kan dele pakker.
