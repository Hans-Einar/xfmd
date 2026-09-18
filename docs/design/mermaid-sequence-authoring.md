# Forfatterveiledning — Sequence 2

Bruk et `mermaid`-kodegjerde med `sequenceDiagram`. Deklarer alle deltakere først
med `participant ID as Navn` eller `actor ID as Navn`; ID består av ASCII-bokstaver,
sifre og understrek. Alias og vanlig etiketttekst kan bruke Unicode.

[APT-eksemplet](mermaid/apt-import.mmd) skiller import fra aktivering, og viser
aksept, ugyldig input og revisjonskonflikt. [Det nestede eksemplet](mermaid/sequence-nested.mmd)
viser asynkrone piler, aktivitet, notater og samtidige forløp. Eksemplene er
illustrasjoner av kandidatbegreper, ikke vedtatte SDL-regler.

| Konstruksjon | Støttet betydning |
| --- | --- |
| `A->>B: Request`, `B-->>A: Reply` | Heltrukken/stiplet lukket pilspiss |
| `A-)B: Command`, `B--)A: Reply` | Heltrukken/stiplet åpen asynkron pilspiss |
| `activate B`, `deactivate B` | Balanserte aktivitetsmarkører, også i fragmenter |
| `Note left of A: …`, `Note right of B: …`, `Note over A,B: …` | Notat på riktig sted i hendelsesrekkefølgen |
| `alt …` / `else …` / `end` | Alternativer, også nestet |
| `opt …`, `loop …`, `par …` / `and …` / `end` | Nestede fragmenter med bevart omfang |

Aktiveringer må være balansert innen hver alternativ-/parallellgren; en gren
kan ikke endre aktivitetstilstanden til en annen. Maks åtte fragmentnivåer.
Hver gren må inneholde en hendelse. Deltakere betyr ikke automatisk tråder,
og en asynkron pil bestemmer ikke scheduling.

Andre piler, inline `+`/`-`, implisitte deltakere, box, autonumber,
create/destroy, HTML, entities, styling og init-direktiver gir synlig feil
med hele kildeblokken. Én syntaktisk setning per linje; kommentarer begynner
med `%%`. Tekst brytes ved ordgrenser. Maks 16 deltakere, 128 hendelser og
64 KiB kilde. Del lange scenarier for lesbar A4/PDF.

[Støttematrisen](../../mermaid_coverage.md) skiller bibliotekstøtte fra XFMD-støtte.
