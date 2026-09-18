# Mermaid-modeller og målte etiketter — revisjon 1

Status: Planned, P38–P41. Krav UR-039/040, SR-021/022.

## Kontrakt

Nye familier bruker `SemanticDiagram`, ikke Flowchart-modellen. `DiagramFamily`
identifiserer familien; hver `SemanticRecord` har en eksplisitt `SemanticTag`
og ordnede verdifelt med definert skjema. Tagger er domenespesifikke: State,
StateRegion, StateTransition, ClassType, ClassAttribute, ClassOperation,
ClassRelation, Requirement, RequirementAttribute, RequirementRelation,
Entity, EntityAttribute og EntityRelation. Frie egenskapsnavn eller bibliotekets
Graph-serialisering er ikke kontrakten. Feltantall, referanser, enumerasjoner og
numeriske grenser valideres før layout. En familie kan bare inneholde sine tagger.

C++ bevarer disse verdirekordene uten bibliotekstyper. Rust-kontrakten eier
skjemaet og valideringen. Parserprofilen produserer dem fra standard Mermaid;
renderer-adapteren oversetter dem til bibliotekets native diagramtype uten å
parse kilde igjen. Originalkilden finnes bare i dokumentblokken for redigering
og synlig feilmelding. Modellen inneholder ingen kjørbare direktiver.

Hierarki/region og relasjonstype er egne felt. En label er aldri eneste
representasjon av kardinalitet, krav-ID eller eierskap. Klassemedlemmer skiller
attributt og operasjon. En state-guard er bevart tekst, ikke en evaluert betingelse.

## Tekstmåling

Nye diagramtyper genererer også tekst under layout, for eksempel aksetall.
En synkron C-ABI-callback lar bibliotekets målesøm bruke XFMDs ITextMetrics
for slike strenger. Context er lånt bare mens layoutkallet kjører, aldri delt
mellom tråder; C++ fanger exceptions, Rust fanger panic ved yttergrensen.
Ingen FOX-typer krysser grensen. Parametre angir tekst, skriftstørrelse og
ombrytningsbredde. Resultatet er målte linjer i bibliotekets TextBlock.
Eksisterende Flowchart/Sequence-målekatalog beholdes for regresjonsstabilitet.

## Ressurser og feil

64 KiB kilde per diagram, begrenset antall rekorder og referanser, kooperativ
layoutfrist og 8 MiB scene gjelder fortsatt. Dokumentgrensen økes til 64
blokker, samtidig innføres 64 MiB samlet scenebudsjett. Ukjent syntaks og
ikke-bevart semantikk avvises lokalt med kildeblokken. Ingen diagramfamilie
fallback-konverteres til Flowchart. Bibliotekets SVG brukes i preview og PDF.

## Verifikasjon

Hver familie får akseptansefixture, negativ syntakstest, modell/wire-roundtrip,
SVG, lys/mørk preview og vektor-PDF. Samme Markdown-galleri brukes i build/xfmd.
Støttematrisen oppdateres med faktisk testet subset; bibliotekets reklametekst
er aldri en kompatibilitetstest. Utestede konstruksjoner forblir eksplisitte hull.
