# Forfatterveiledning — Sequence 1

Bruk et `mermaid`-kodegjerde med `sequenceDiagram`. Deklarer alle deltakere først
med `participant ID as Navn` eller `actor ID as Navn`; ID består av ASCII-bokstaver,
sifre og understrek. Alias og vanlig etiketttekst kan bruke Unicode.

Se [APT-eksemplet](mermaid/apt-import.mmd): Command til ansvarlig service og
separate svar for akseptert utkast, ugyldig input og revisjonskonflikt. Import
aktiverer ikke et utkast. Eksemplet illustrerer en kandidat fra SDP-checkpoint #1;
det vedtar ingen SDL-regel.

| Konstruksjon | Sequence 1 |
| --- | --- |
| `A->>B: Request`, `B-->>A: Reply` | Solid/stiplet lukket pilspiss, kildeorden beholdes |
| `activate B`, `deactivate B` | Balanserte aktivitetsmarkører utenfor fragmenter; ingen implisitt tråd |
| `Note left of A: …`, `Note right of B: …`, `Note over A,B: …` | Utenfor fragmenter; sett en melding mellom notat og påfølgende fragment |
| `alt …` / `else …` / `end` | Ikke-nestede alternativer, minst én melding per gren |
| `opt …`, `loop …`, `par …` / `and …` / `end` | Flate fragmenter; tekst beskriver hensikt, ingen kjøring/scheduling |

Andre piler (inkludert async `-)`), nested fragmenter, aktivering inne i fragmenter,
inline `+`/`-`, implisitte deltakere, box, autonumber, create/destroy, HTML,
entities, styling og init-direktiver gir synlig forklaring og hele kildeblokken.
Ikke erstatt en asynkron pil med en synkron bare for å få diagrammet tegnet.
Notater og aktiveringer på komplekse fragmentgrenser trenger Sequence 2.
Et notat må også etterfølges av en melding før activate/deactivate, slik at
aktivitetsgrensen ikke flyttes tilbake over notatet.

Én syntaktisk setning per linje. Kommentarlinjer begynner med `%%`.
Tekst brytes ved ordgrenser; enkeltord beholdes hele. Maks 16 deltakere,
128 hendelser og 64 KiB kilde. Store diagrammer skaleres som én blokk i A4;
del lange scenarier i flere diagrammer for lesbar PDF.

Flowchart 1 er fortsatt støttet, med sin [egen profil](mermaid-integration.md).
[Støttematrisen](mermaid-coverage.md) skiller bibliotekstøtte fra XFMD-støtte.
