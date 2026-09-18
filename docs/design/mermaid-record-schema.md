# SemanticDiagram records — modellversjon 3

Eier: contracts. Format: family, record count, deretter tag og en ordnet liste
UTF-8-felt per record. Ingen felt er Mermaid-kilde eller serialisert Graph.
Tall transporteres som validerte strenger; layout får bare validerte verdier.
Wire-grensen er 16 felt/record og 4096 bytes/felt. Leseren avviser ukjente
familier/tags, feil feltantall, ugyldige tall og referanser før native mapping.

State/Class/Requirement/ER (familie 2–5) er spesifisert ved de navngitte
konstantene i `src/contracts/diagram/rust/src/semantic.rs`.

| Familie | Tag | Felt i rekkefølge |
| --- | --- | --- |
| C4 (6) | 50 | view-header |
| C4 | 51 | id, label, type, technology, description, parent-boundary |
| C4 | 52 | id, label, boundary-type, parent-boundary |
| C4 | 53 | source, target, relation-kind, label, technology |
| Architecture (7) | 60/61 | group/service id, label, icon, parent-group |
| Architecture | 62 | junction id, parent-group |
| Architecture | 63 | source, target, source-side, target-side, arrow-mask |
| Block (8) | 70 | column-count |
| Block | 71/72 | cell id, label, span, shape / space id, span |
| Block | 73 | source, target, label, arrow-mask |
| Packet (9) | 80 | start-bit, inclusive-end-bit, label |
| Timeline/Gantt/Journey/charts | 90 | title (bare familier som støtter den) |
| Timeline/Gantt/Journey (10–12) | 91 | section id, label |
| Timeline | 92 | event id, period, section id, event labels… |
| Gantt | 100 | task id, label, ISO start, duration-days, after-id, section id, status |
| Journey | 110 | task id, label, score, section id, comma-separated actors |
| Pie (13) | 120/121 | label, value / showData |
| Mindmap (14), Treemap (21) | 130 | id, label, parent id, shape, optional leaf-weight |
| GitGraph (15) | 140 | branch name, branch-point commit |
| GitGraph | 141 | commit id, branch, first parent, second parent, kind, reserved-empty tag |
| Sankey (16) | 150 | source label, target label, weight |
| Quadrant (17) | 160/161/162 | point label, x, y / axis, low label, high label / quadrant index, label |
| ZenUML (18) | 170/171 | participant id, label / source, target, async-message label |
| Kanban (19) | 180/181 | column id, label / card id, label, column id |
| Radar (20) | 190/191/192 | axis id, label / curve id, label, values… / option, value |
| XYChart (22) | 200/201/202 | categories… / Y label, optional min, optional max / series kind, values… |

Ordning er meningsbærende for hendelser, oppgaver, kort, akser og commits.
Hierarkiforeldre og commit-avhengigheter deklareres før bruk der profilen krever
et tre/DAG. Diagramidentitet kommer aldri fra bare source/target-par.
Sankey-nodeidentiteter genereres stabilt av etikettenes førstegangsforekomst.
Gantt `after` er en tidsavhengighet, GitGraph-foreldre er commit-historikk,
ZenUML arrow er asynkron; disse er ikke generiske flowchart-kanter i kontrakten.

Legg til nye tags/familier i kontrakt, C++-enum, validerer, parser, adapter og
roundtrip-tester samlet. Endret feltbetydning krever ny modellversjon.
