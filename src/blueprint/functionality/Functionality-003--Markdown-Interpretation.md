---
id: FUNC-003
kind: Functionality
audience: System
role: Service
owner: interpreter
status: Proposed
scope: FirstRelease
requirements: UR-002, SR-001, SR-003, SR-004, SR-005, SR-009, SR-011, SR-013
uses: none
---

# Functionality-003: Markdown-tolkning og semantisk modell

## 1. Hensikt og avgrensning

Konverter kildetekst til en stabil, eid semantisk modell. cmark er første adapter, ikke del av den offentlige kontrakten. Interpreter kjenner ikke fonter, FOX eller navigasjonshistorikk.

## 2. Krav og akseptanse

Krav: UR-002, SR-001, SR-003, SR-004, SR-005, SR-009, SR-011, SR-013. Definisjoner og normativ akseptanse finnes i
[kravspesifikasjonen](../../../xfmd_requirements.md). Kapittel 7 konkretiserer beviset.

## 3. Kontrakter og eierskap

`IInterpreter::parse(const SourceSnapshot&, const ParseOptions&) -> ParseResult`, implementert av `CmarkInterpreter::parse`. Returnerer SemanticDocument med samme dokument/revisjon, text runs, blokker, lenkemål, kildeområder og mappingkvalitet. For `.txt` produseres ren tekstmodell. ParseOptions låser profil/ressursgrenser. Ingen lagring eller ressurslasting.

## 4. Atferd, tilstand og feil

cmark-callbacks bygges om til modell med eid minne. Entiteter/escapes normaliseres i dette laget samtidig som kildeankre bevares. HTML er inert tekst, bilder alt-tekst/plassholder. Ufullstendig syntaks er normalt tekst, ikke fatal feil. Runtime-/minnefeil returnerer ingen halv modell. Ikke behold callback-pekere etter parse.

## 5. Plumbing

Alle symboler og kildefiler i tabellen er **planlagte**, ikke implementert kode.
Bibliotekskall verifiseres mot valgt dependency-versjon før implementering.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `PreviewCoordinator::refresh` | `IInterpreter::parse / CmarkInterpreter::parse` | `src/interpreter/CmarkInterpreter.cpp` | Snapshot + profil → ParseResult | Runtimefeil avbryter modellpublisering. | Planned |
| 2 | `CmarkInterpreter::parse` | `ModelBuilder::beginDocument` | `src/interpreter/ModelBuilder.cpp` | Dokument/revisjon → builder | Ny builder per parse, ingen delt tilstand. | Planned |
| 3 | `cmark AST traversal` | `ModelBuilder::appendNode` | `src/interpreter/ModelBuilder.cpp` | Parserhendelse → semantikk | Bibliotektyper stopper i adapteren. Kildeområder kommer fra AST. | Planned |
| 4 | `ModelBuilder::appendNode` | `SourceMapBuilder::record` | `src/interpreter/SourceMapBuilder.cpp` | Kildebevis → range/kvalitet | Ukjent posisjon merkes, gjettes ikke exact. | Planned |
| 5 | `CmarkInterpreter::parse` | `ModelBuilder::finish` | `src/interpreter/ModelBuilder.cpp` | Builder → eid SemanticDocument | Valider ranges/revisjon før retur. | Planned |

## 6. Gjenbruk og avhengigheter

Ingen andre functionality-kontrakter konsumeres; delte datatyper følger arkitekturen.

Preview er første konsument; headless inspeksjon/eksport kan senere bruke samme port. Renderer mottar bare ferdig modell. Ingen deling av cmark callbacks med application.

## 7. Verifikasjon

AT-002, AT-011, AT-013, AT-014, AT-015, AT-019, AT-021, AT-023: CommonMark-fixtures, gjentatt tekst, entiteter, nestede lister, tomme blokker og fake interpreter-kontrakt. Planlagt `tests/interpreter/InterpreterContractTest.cpp`.

Bevis: ingen applikasjonstest kjørt; testfiler ovenfor er planlagte. Ved implementering
oppgis kommando, fixture, miljø, commit og faktisk utfall. Strukturkontroll alene
oppfyller ikke atferdskravene.

## 8. Status, risiko og endringskonsekvenser

Proposed, revisjon 0.1, 2026-09-12. P0 må bevise kildeområder: cmark gir ikke alle ranges direkte. Alternativ interpreter/avgrenset adapterutvidelse vurderes før Ready dersom mapping ikke møter UR-008.

Ved endret offentlig kontrakt: oppdater konsumentene i registeret, dette kallkartet,
berørte krav og kontrakttester i samme endring. Før status Ready skal relevante
P0-spørsmål være avgjort; før Verified skal kapittel 7 inneholde testbevis.

P0-beslutning: cmark 0.31.1 velges for AST-kildeposisjoner; se [P0](../../../docs/evidence/P0.md).
