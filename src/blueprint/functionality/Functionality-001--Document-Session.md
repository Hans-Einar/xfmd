---
id: FUNC-001
kind: Functionality
audience: System
role: Workflow
owner: application
status: Implemented
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

DocumentSession eier rå UTF-8, baseline, filidentitet og token. DocumentCoordinator eksponerer requestOpen, save, requestClose og resolveUnsaved. Bool-resultat betyr fullført eller avbrutt/feilet; feilteksten går via error-callback. Session-mutatorer brukes bare gjennom dokument-/edit-eier.

## 4. Atferd, tilstand og feil

Kandidat lastes før replace. Dirty-valg skjer først; Failed/Cancelled beholder buffer. Vellykket åpning gir én opened-callback. Save oppdaterer kun den lagrede baselinen; ekstern konflikt beholder dirty. Dialogene injiseres fra Application.

## 5. Plumbing

Tabellen beskriver implementerte kall. Navngitte hendelser er injiserte callbacks, ikke en global event bus.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `NavigationCoordinator::openTarget` | `DocumentCoordinator::requestOpen` | `src/application/document/DocumentCoordinator.cpp` | Path → bool | Feil via callback | Implemented |
| 2 | `DocumentCoordinator::requestOpen` | `DocumentCoordinator::resolveUnsaved` | `src/application/document/DocumentCoordinator.cpp` | Dirty → Save/Discard/Cancel | Save-feil avbryter | Implemented |
| 3 | `DocumentCoordinator::requestOpen` | `LocalFileStore::read` | `src/application/io/LocalFileStore.cpp` | Path → kandidat | Ingen mutasjon før suksess | Implemented |
| 4 | `DocumentCoordinator::requestOpen` | `DocumentSession::replace` | `src/application/document/DocumentSession.cpp` | Kandidat → ny token | opened etter commit | Implemented |
| 5 | `DocumentCoordinator::save` | `LocalFileStore::writeAtomic` | `src/application/io/LocalFileStore.cpp` | Snapshot + expected → SavedDocument | Konflikt stopper save | Implemented |
| 6 | `DocumentCoordinator::save` | `DocumentSession::markSaved` | `src/application/document/DocumentSession.cpp` | Lagret snapshot → baseline | Beholder senere edits | Implemented |

## 6. Gjenbruk og avhengigheter

[FUNC-002](../functionality/Functionality-002--Local-File-Storage.md)

Konsumenter: preview, tekstredigering, workspace og navigasjon. Alle må bruke requestOpen; ingen direkte Session::replace utenfor koordinatorens commit. Snapshot er gjenbrukbar data, ikke tilgang til muterbar buffer.

## 7. Verifikasjon

Relevante akseptanse-ID-er: AT-001, AT-003, AT-009, AT-012, AT-016, AT-017, AT-018, AT-023.

`DocumentTest` bekrefter dirty-cancel, failed-open, filkonflikt, save, undo og metadata. `WorkspaceTest` verifiserer ekte FXText-projeksjon.

Evidence: [Fase P2](../../../docs/evidence/P2.md). Samlet kravdekning og eventuelle gjenstående begrensninger kontrolleres i P7; Implemented er ikke automatisk Verified.

## 8. Status, risiko og endringskonsekvenser

Implemented i P2. Oppdater kontrakter, kallkart, konsumenter og tester i samme endring.
Rene porter og tydelig rolleeierskap er obligatorisk. Eventuelle senere avvik står i fasens bevisrapport.
