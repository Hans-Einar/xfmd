---
id: FUNC-011
kind: Functionality
audience: User
role: Service
owner: application
status: Proposed
scope: FirstRelease
requirements: UR-003, UR-004, UR-009, SR-002, SR-006, SR-008, SR-013
uses: FUNC-001, FUNC-007
---

# Functionality-011: Tekstredigering, undo og søk

## 1. Hensikt og avgrensning

Tilby vanlig tekstredigering med én konsistent vei til dokumentrevisjoner. FOX-editoren er projeksjon, ikke en konkurrerende source of truth. Avgrenset enkel editor, ikke erstatning for xfw som avansert tekstverktøy.

## 2. Krav og akseptanse

Krav: UR-003, UR-004, UR-009, SR-002, SR-006, SR-008, SR-013. Definisjoner og normativ akseptanse finnes i
[kravspesifikasjonen](../../../xfmd_requirements.md). Kapittel 7 konkretiserer beviset.

## 3. Kontrakter og eierskap

`EditController::applyEdit(Edit)`, `undo()`, `redo()`, `find(SearchRequest)`; `EditorWidget::applyProjection(SourceSnapshot)`. Edit har byte-range, replacement og opprinnelse. EditController eier undo/redo-operasjonene, DocumentSession eier tekst/baseline. Velg én undo-mekanisme; ikke parallelle FOX- og egne stacks.

## 4. Atferd, tilstand og feil

Brukeredit valideres mot aktuell revisjon og oversettes til øktendring. Programmatisk projection gir ikke ny undo-post eller rekursiv edit. Undo/redo bruker samme apply-vei og varsler preview; vanlig søk endrer bare selection/cursor. Inputpolicy hindrer NUL/ugyldig UTF-8 fra paste, uten å avvise vanlig ufullstendig Markdown. Nytt dokument resetter undo; view mode gjør det ikke.

## 5. Plumbing

Alle symboler og kildefiler i tabellen er **planlagte**, ikke implementert kode.
Bibliotekskall verifiseres mot valgt dependency-versjon før implementering.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `FOX insert/delete/replace event` | `EditorWidget::onChanged` | `src/application/ui/EditorWidget.cpp` | FOX-endring → bytebasert Edit | Ignorer projection-origin; valider inndata. | Planned |
| 2 | `EditorWidget::onChanged / undo / redo` | `EditController::applyEdit` | `src/application/document/EditController.cpp` | Edit + forventet revisjon | Mismatch krever re-sync, ikke blind mutasjon. | Planned |
| 3 | `EditController::applyEdit` | `DocumentSession::applyEdit` | `src/application/document/DocumentSession.cpp` | Byte-range + replacement → Revision | Oppdater baseline-sammenligning/dirty. | Planned |
| 4 | `EditController::applyEdit` | `EditorWidget::applyProjection` | `src/application/ui/EditorWidget.cpp` | Bekreftet tekst → editor | Behold markør/selection; undertrykk event-loop. | Planned |
| 5 | `EditController::applyEdit` | `PreviewCoordinator::schedule` | `src/application/preview/PreviewCoordinator.cpp` | Ny Revision | Én planlegging per faktisk edit. | Planned |
| 6 | `CommandRouter::find` | `EditController::find` | `src/application/document/EditController.cpp` | SearchRequest → treff/selection | Ingen tekst-/dirty-endring. | Planned |

## 6. Gjenbruk og avhengigheter

[FUNC-001](../functionality/Functionality-001--Document-Session.md), [FUNC-007](../functionality/Functionality-007--Preview-Pipeline.md)

Live preview konsumerer edits; dokumentøkt/lagring brukes uendret. Nye editorhandlinger skal beskrives som Edit-operasjoner, ikke mutere FXText og Session separat.

## 7. Verifikasjon

AT-003, AT-004, AT-009, AT-012, AT-016, AT-018, AT-023: Unicode-range, CRLF/BOM, paste, undo til lagret baseline, redo etter ny edit, søk uten dirty og projection uten duplikat. Planlagt `tests/application/EditControllerTest.cpp` og ekte FXText-integrasjon.

Bevis: ingen applikasjonstest kjørt; testfiler ovenfor er planlagte. Ved implementering
oppgis kommando, fixture, miljø, commit og faktisk utfall. Strukturkontroll alene
oppfyller ikke atferdskravene.

## 8. Status, risiko og endringskonsekvenser

Proposed, revisjon 0.1, 2026-09-12. P0/P1 avklarer byteposisjoner, CRLF/BOM og hvilken undo-stack som brukes. Ingen garanti om tapsfri FOX-redigering før roundtrip-testene passerer.

Ved endret offentlig kontrakt: oppdater konsumentene i registeret, dette kallkartet,
berørte krav og kontrakttester i samme endring. Før status Ready skal relevante
P0-spørsmål være avgjort; før Verified skal kapittel 7 inneholde testbevis.
