---
id: FUNC-001
kind: Functionality
audience: System
role: Workflow
owner: application
status: Proposed
scope: FirstRelease
requirements: UR-001, UR-003, UR-009, SR-002, SR-006, SR-007, SR-008, SR-013
uses: FUNC-002
---

# Functionality-001: Dokumentøkt og transaksjoner

## 1. Hensikt og avgrensning

Gi alle åpne-, lagre- og lukkeinnganger samme dokumentlivsløp. Load/save er tjenesteoperasjoner, ikke egne features. DocumentSession eier tekst/revisjon; DocumentCoordinator eier beslutningsforløpet. Ingen parsing, tegning eller historikkalgoritme her.

## 2. Krav og akseptanse

Krav: UR-001, UR-003, UR-009, SR-002, SR-006, SR-007, SR-008, SR-013. Definisjoner og normativ akseptanse finnes i
[kravspesifikasjonen](../../../xfmd_requirements.md). Kapittel 7 konkretiserer beviset.

## 3. Kontrakter og eierskap

`DocumentCoordinator::requestOpen(OpenRequest) -> OpenResult`, `save(SaveRequest) -> SaveResult`, `requestClose() -> CloseResult`; `DocumentSession::snapshot() -> SourceSnapshot`, `applyEdit(Edit) -> Revision`, `replace(LoadedDocument)`, `markSaved(SavedSnapshot)`. OpenRequest bærer opprinnelse og eventuell historikkforespørsel. Åpneresultat skiller Opened, Cancelled og Failed. Sesjonen eier baseline og filidentitet; koordinatoren låner filstore.

## 4. Atferd, tilstand og feil

Åpning sjekker dirty og brukerens valg. Kandidat leses før replace; feil/avbrudd beholder økten. Save-feil stopper etterfølgende bytte. Forkast betyr ikke at gammel buffer slettes før nytt dokument faktisk er lastet. Etter commit sendes DocumentOpened til preview/historikk-abonnenter; feil i preview ruller ikke tilbake en vellykket filåpning. Ulagret tomt dokument krever målsti. Dirty er innhold sammenlignet med lagret baseline, så undo kan gjøre dokumentet rent.

## 5. Plumbing

Alle symboler og kildefiler i tabellen er **planlagte**, ikke implementert kode.
Bibliotekskall verifiseres mot valgt dependency-versjon før implementering.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `CommandRouter::open / SidebarWidget::onOpen / NavigationCoordinator::openTarget` | `DocumentCoordinator::requestOpen` | `src/application/document/DocumentCoordinator.cpp` | OpenRequest → beslutning | Dirty-dialog; Cancelled muterer ikke økt. | Planned |
| 2 | `DocumentCoordinator::requestOpen` | `DocumentCoordinator::resolveUnsaved` | `src/application/document/DocumentCoordinator.cpp` | Lagre/Forkast/Avbryt | Save-feil returnerer Failed uten bytte. | Planned |
| 3 | `DocumentCoordinator::requestOpen` | `LocalFileStore::read` | `src/application/io/LocalFileStore.cpp` | Sti → LoadedDocument | I/O/formatfeil returneres før replace. | Planned |
| 4 | `DocumentCoordinator::requestOpen` | `DocumentSession::replace` | `src/application/document/DocumentSession.cpp` | Kandidat → ny ID/revisjon | Commit; sender DocumentOpened etter konsistent tilstand. | Planned |
| 5 | `DocumentCoordinator::save` | `LocalFileStore::writeAtomic` | `src/application/io/LocalFileStore.cpp` | Snapshot + forventet identitet → SavedSnapshot | Konflikt/I/O beholder dirty. | Planned |
| 6 | `DocumentCoordinator::save` | `DocumentSession::markSaved` | `src/application/document/DocumentSession.cpp` | Lagrede bytes → baseline | Senere edits forblir dirty. | Planned |

## 6. Gjenbruk og avhengigheter

[FUNC-002](../functionality/Functionality-002--Local-File-Storage.md)

Konsumenter: preview, tekstredigering, workspace og navigasjon. Alle må bruke requestOpen; ingen direkte Session::replace utenfor koordinatorens commit. Snapshot er gjenbrukbar data, ikke tilgang til muterbar buffer.

## 7. Verifikasjon

AT-001, AT-003, AT-009, AT-012, AT-016, AT-017, AT-018, AT-023: test avbrutt bytte, feilet lagring, undo til baseline, save av bestemt revisjon og én DocumentOpened per commit. Planlagt `tests/application/DocumentSessionTest.cpp` og `DocumentCoordinatorTest.cpp`.

Bevis: ingen applikasjonstest kjørt; testfiler ovenfor er planlagte. Ved implementering
oppgis kommando, fixture, miljø, commit og faktisk utfall. Strukturkontroll alene
oppfyller ikke atferdskravene.

## 8. Status, risiko og endringskonsekvenser

Proposed, revisjon 0.1, 2026-09-12. Dialogadapter, filmetadata og editorprojeksjon avklares i P1/P2. Ingen implementerte symboler eller utførte applikasjonstester.

Ved endret offentlig kontrakt: oppdater konsumentene i registeret, dette kallkartet,
berørte krav og kontrakttester i samme endring. Før status Ready skal relevante
P0-spørsmål være avgjort; før Verified skal kapittel 7 inneholde testbevis.
