# Øvrige Mermaid-profiler — revisjon 1

P41 Implemented; UR-039/040, SR-021/022, FUNC-023/024/025. Alle profiler er
avgrensede; ukjente utsagn avvises. Ingen JS, nettverk eller ekstern tjeneste.
De er illustrasjoner og utforskning, ikke typekontroll eller aksepterte SDL-regler.

| Type | Første profil | Eksplisitte grenser |
| --- | --- | --- |
| Pie | title, showData, sitert kategori og positiv verdi | Ingen init/styling; maks 12 kategorier |
| Mindmap | Innrykket tre, plain/rectangle/circle/rounded noder | To mellomrom per nivå, én rot, maks åtte nivåer; ingen icons/classes |
| GitGraph | commit id, branch, checkout, merge | LR-standard, eksplisitte commit-ID-er; ingen cherry-pick/order/tag/type |
| Sankey | source,target,positiv verdi | Asyklisk, ingen quoted CSV eller konfigurasjon |
| Quadrant | title, akseender, fire quadrant-navn, punkt [x,y] | Koordinater 0–1; ingen punktstyling |
| ZenUML | Deltakere/alias og A->B: asynkron melding | Ingen synkrone kall, creation, replies eller kontrollblokker; ikke sequence-piler tolket på nytt |
| Kanban | Kolonner og innrykkede ID[tekst]-kort | Ingen metadata, tickets, assignments eller styles |
| Radar | title, axis ID[label], curve ID[label]{verdier}, min/max, graticule | 3–12 akser, posisjonelle komplette verdier; ingen named values |
| Treemap | Innrykkede grupper og sitert blad: positiv verdi | Foreldre har ingen egenverdi; maks åtte nivåer |
| XYChart | title, kategorisk x-axis, y-axis label/range, bar/line | 1–12 kategorier, komplette endelige verdier, vertikal orientering |

Egne records bevarer hierarki, commit-foreldre, vekt, punktkoordinater, kategorier,
kurver og asynkron meldingsform. Native Graph er kun adapterens representasjon,
ikke felles flowchart-modell. ZenUML bruker faktisk sequence-layout med åpne
asynkrone pilspisser. Samme kildegrense, frist og SVG/PDF-port gjelder.

Nye Mermaid-typer uten DiagramKind i pinnen er fremtidig dekning, ikke stille
konvertering. Oppdatert støttematrise skiller disse fra de 23 typene i denne pinnen.

Kilder: Mermaid sine offisielle syntax-sider for pie, mindmap, gitgraph, sankey,
quadrantChart, zenuml, kanban, radar, treemap og xychart. ZenUML `A->B: text`
er asynkron syntaks; vanlige sequenceDiagram-pilformer avvises i den profilen.
GitGraph `branch` bytter aktiv branch slik standarden beskriver.
