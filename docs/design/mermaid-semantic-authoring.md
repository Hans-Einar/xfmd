# Forfatterprofiler — State/Class/Requirement/ER 1

Bruk ett standard Mermaid-kodegjerde. Profilene aksepterer hele utsagn, ett
per linje, med eksplisitte feil for ukjent syntaks. Vanlig Unicode-tekst er
støttet; HTML, entities, init/style/click-direktiver og semikolon er ikke del
av disse profilene. Identifikatorer bruker ASCII-bokstaver, sifre og understrek.
Maks 64 KiB kilde, 128 identiteter og 512 semantiske rekorder per diagram.

## State 1 — stateDiagram-v2

[Livsløp](mermaid/measurement-state.mmd), [regioner](mermaid/state-regions.mmd)
og [valg/fork/join](mermaid/state-choice.mmd) viser profilen.
`[*] --> State`, `State --> [*]`, `A --> B : tekst`, `state "Navn" as ID`,
`ID : beskrivelse`, `state ID {` / `}` og `--` mellom samtidige regioner er
støttet. `state ID <<choice>>`, `<<fork>>` og `<<join>>` har egne roller og former.
`direction LR/TB/BT/RL` gjelder hele diagrammet. Maks åtte composite-nivåer.

Regioner beholder scope og egne start/sluttilstander. Piltekst evaluerer ingen
guard. En sen observasjon gjør ikke automatisk en foreldet verdi aktuell;
eksemplet skiller ny akseptert revisjon fra nullstilling av source-session.
State-notater, lokal retning og styling er foreløpig ikke støttet.

## Class 1 — classDiagram

[Metamodellforslag](mermaid/sdl-class.mmd) viser Unit, Container, Functionality,
Function, Dataset, Datagram, Command og Value. Dette er forklarende kandidat-
relasjoner, ikke en vedtatt SDL-typekontroll eller et krav om OO-implementasjon.

`class ID` eller `class ID {` / `}` deklarerer typer. Medlemslinjer er attributter
eller operasjoner med parenteser; `<<annotation>>` er egen annotasjon.
`ID : medlem` støttes også. Relasjonene `-->`, `--`, `..>`, `..|>`, `--|>`,
`*--`, `o--` og motsatt pilretning bevares eksplisitt. Siterte multipliciteter
kan stå ved begge ender; valgfri relasjonstekst følger ` : `. Komposisjonens
fylte diamant står hos eieren. Namespace, generics, callbacks og styling avvises.

## Requirement 1 — requirementDiagram

[APT-sporbarhet](mermaid/apt-requirements.mmd) bruker krav, underkrav og elementer.
Støttede kravtyper: requirement, functionalRequirement, interfaceRequirement,
performanceRequirement, physicalRequirement og designConstraint. Egenskaper
`id`, `text`, `risk` (low/medium/high) og `verifymethod`
(analysis/inspection/test/demonstration) bevares som egne verdier.
`element` har `type` og `docref`. Deklarasjoner har én egenskap per linje i `{}`.

`A - satisfies -> B` og tilsvarende contains, copies, derives, verifies,
refines og traces er Mermaid-relasjoner. En tegnet verifies-relasjon er ikke
bevis for bestått test. SDL-begreper og Mermaid-vokabular er ikke automatisk like.

## ER 1 — erDiagram

[Proveniens](mermaid/provenance-er.mmd) viser logiske identiteter uavhengig av
fysisk databaseskjema. Deklarer `ENTITY {` med `type name`, valgfrie
`PK`, `FK`, `UK` (kommaseparert uten mellomrom) og valgfri sitert kommentar.
Relasjoner har format `A ||--o{ B : navn`. `||`, `o|`/`|o`, `}|`/`|{`,
`}o`/`o{` representerer henholdsvis én, null/én, én/flere og null/flere.
`--` og `..` skiller identifiserende fra ikke-identifiserende relasjon.
Kardinalitet er ikke bare en tekstetikett. Dataset og Datagram har ingen
innebygd én-til-én-antagelse. Alias/styling og databaseutføring støttes ikke.
