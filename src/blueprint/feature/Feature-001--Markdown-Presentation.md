---
id: FTR-001
kind: Feature
audience: User
role: Workflow
owner: application
status: Proposed
scope: FirstRelease
requirements: UR-001, UR-002, UR-009, SR-001, SR-003, SR-004, SR-005, SR-011, SR-012, SR-013
uses: FUNC-001, FUNC-002, FUNC-003, FUNC-004, FUNC-005, FUNC-007
---

# Feature-001: Lesbar Markdown-presentasjon

## 1. Hensikt og avgrensning

Brukeren åpner et lokalt dokument og får lesbar native Markdown med tydelig typografi. Dette er samlet brukerresultat på tvers av lag. Application eier orkestrering; interpreter eier semantikk og renderer eier presentasjon. Load/read/paint er gjenbrukt functionality, ikke underfeatures.

## 2. Krav og akseptanse

Krav: UR-001, UR-002, UR-009, SR-001, SR-003, SR-004, SR-005, SR-011, SR-012, SR-013. Definisjoner og normativ akseptanse finnes i
[kravspesifikasjonen](../../../xfmd_requirements.md). Kapittel 7 konkretiserer beviset.

## 3. Kontrakter og eierskap

Inngang er OpenRequest fra CLI/dialog/sidebar. Utgang er aktiv SourceSnapshot, tilsvarende SemanticDocument og gyldig RenderFrame eller tydelig feil. Ingen egen featureklasse innføres; FUNC-001/FUNC-007 er orkestratorer. FTR-001 eier akseptansescenarioet, ikke dupliserte API-er.

## 4. Atferd, tilstand og feil

Åpne .md, valider input, commit dokument, bygg modell/layout og publiser native frame. .txt går samme pipeline med ren tekstmodell. Feil før dokumentcommit bevarer gammel økt; senere renderfeil viser ny kilde med feilstatus. Bilder/HTML er inert etter kravpolicy. Lenketekst vises her; faktisk historikknavigasjon er FTR-003.

## 5. Plumbing

Alle symboler og kildefiler i tabellen er **planlagte**, ikke implementert kode.
Bibliotekskall verifiseres mot valgt dependency-versjon før implementering.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `CommandRouter::open` | `DocumentCoordinator::requestOpen` | `src/application/document/DocumentCoordinator.cpp` | Path → aktivt dokument | FUNC-001 håndterer dirty/feil. | Planned |
| 2 | `DocumentCoordinator::requestOpen` | `LocalFileStore::read` | `src/application/io/LocalFileStore.cpp` | Path → validert LoadedDocument | FUNC-002 avviser format/ressursfeil. | Planned |
| 3 | `DocumentOpened` | `PreviewCoordinator::refresh` | `src/application/preview/PreviewCoordinator.cpp` | Snapshot-token → pipeline | Kjører umiddelbart ved åpning. | Planned |
| 4 | `PreviewCoordinator::refresh` | `IInterpreter::parse` | `src/contracts/IInterpreter.h` | Snapshot → semantisk modell | FUNC-003 eier tolking. | Planned |
| 5 | `PreviewCoordinator::refresh` | `IRenderer::layout` | `src/contracts/IRenderer.h` | Modell + metrics → frame | FUNC-004 eier typografi/layout. | Planned |
| 6 | `PreviewCoordinator::refresh` | `FoxRenderHost::present` | `src/application/adapters/FoxRenderHost.cpp` | Riktig frame → native visning | FUNC-005 kontrollerer gyldighet. | Planned |

## 6. Gjenbruk og avhengigheter

[FUNC-001](../functionality/Functionality-001--Document-Session.md), [FUNC-002](../functionality/Functionality-002--Local-File-Storage.md), [FUNC-003](../functionality/Functionality-003--Markdown-Interpretation.md), [FUNC-004](../functionality/Functionality-004--Render-Layout.md), [FUNC-005](../functionality/Functionality-005--FOX-Presentation-Host.md), [FUNC-007](../functionality/Functionality-007--Preview-Pipeline.md)

Deler hele pipeline med FTR-002; navigasjon bruker samme åpning; sync leser samme ankergeometri. Ingen alternativ renderer inne i live-preview-feature.

## 7. Verifikasjon

AT-001, AT-002, AT-009, AT-011, AT-013, AT-014, AT-015, AT-021, AT-022, AT-023: åpne fixture med alle støttede elementer; bekreft fonter, riktig fil/revisjon, ingen eksterne sideeffekter og oppgitt ytelse. Planlagt `tests/acceptance/MarkdownPresentationTest.cpp` + manuell visuell kontroll.

Bevis: ingen applikasjonstest kjørt; testfiler ovenfor er planlagte. Ved implementering
oppgis kommando, fixture, miljø, commit og faktisk utfall. Strukturkontroll alene
oppfyller ikke atferdskravene.

## 8. Status, risiko og endringskonsekvenser

Proposed, revisjon 0.1, 2026-09-12. P0-font-/mappingbeslutninger må lukkes før Ready. Formatutvidelser krever krav og kontraktsanalyse, ikke bare nye MD4C-flagg.

Ved endret offentlig kontrakt: oppdater konsumentene i registeret, dette kallkartet,
berørte krav og kontrakttester i samme endring. Før status Ready skal relevante
P0-spørsmål være avgjort; før Verified skal kapittel 7 inneholde testbevis.
