# BoxUI 0.1 i XFMD

Bruk et `boxui`-gjerde med første linje `boxui 0.1`, etterfulgt av strict JSON.
Et `mermaid`-gjerde med samme første linje er alias. Se
[praktisk eksempel](../../boxui_evidence.md). JSON-kommentarer, duplikatnøkler,
ukjente egenskaper og ukjente widgetversjoner avvises.

Dokumentet har profile `boxui/0.1`, unik documentId i Markdown-filen, bindings
og en root av typen row eller column. Widget-ID-er er unike i blokken og følger
`[A-Za-z][A-Za-z0-9_-]{0,63}`. Alle widgets bruker version 1.

| Kind | Innhold |
| --- | --- |
| row / column | children; size.min/max/grow styrer hovedaksen |
| text | text |
| value | label, valueBinding |
| button | label, commandBinding med argumenttype none |
| input | label, valueBinding og commandBinding med type string |
| diagram | label, family, source; flowchart, sequenceDiagram eller stateDiagram-v2 |

Bindings deklarerer id, role (value/command) og type. Values kan være string,
number eller boolean. Et dokument deklarerer bare bindingene; det får ingen
rett til å kjøre kode, hente URL-er eller kalle en tjeneste. Uten eksplisitt vert
vises missing/unbound og kontroller er deaktivert.

Den lokale prototypen binder det dokumenterte Activity-eksemplet. Menyen
View → BoxUI prototype er uttrykkelig opt-in og lagres ikke mellom omstarter.
Tegnet stateDiagram er dokumentasjon, ikke en kjørbar tilstandsmaskin.
En akseptert command er ikke et bevis på eksterne domeneeffekter.

PDF bruker et frosset snapshot med aksepterte verdier, uten utkast eller hendelser.
Tekst inne i SVG er foreløpig ikke merkbar som Markdown-tekst; native input har
vanlig tekstmerking og clipboard. Begrensninger og faktisk testbevis registreres
i sprintens siste fase, ikke utledes fra dette forfattereksemplet.
