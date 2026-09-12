---
id: FUNC-002
kind: Functionality
audience: System
role: Service
owner: application
status: Implemented
scope: FirstRelease
requirements: UR-001, UR-003, UR-009, SR-005, SR-006, SR-007, SR-011, SR-013
uses: none
---

# Functionality-002: Lokal lagring og inputpolicy

## 1. Hensikt og avgrensning

Gjenbrukbar lokal fil-I/O og inputkontroll. Tjenesten kjenner ikke widgets, historikk eller Markdown-layout. Validering her gjelder transport/encoding og filpolicy, ikke om ufullstendig Markdown er grammatisk pent.

## 2. Krav og akseptanse

Krav: UR-001, UR-003, UR-009, SR-005, SR-006, SR-007, SR-011, SR-013. Definisjoner og normativ akseptanse finnes i
[kravspesifikasjonen](../../../xfmd_requirements.md). Kapittel 7 konkretiserer beviset.

## 3. Kontrakter og eierskap

LocalFileStore::read og writeAtomic returnerer eide data eller kaster Error. InputPolicy::validate kontrollerer størrelse, endelse og UTF-8; supportedPath/plainText deles med sidebar og dokumentøkt.

## 4. Atferd, tilstand og feil

Les bare regulære filer med løpende 8 MiB-grense. Skriv temp i målmappen; bevar eier, modusbits og xattrs/ACL. Hardlinks avvises. Kontroll av ekstern identitet gjentas før rename. Ny fil publiseres uten overskriving via link/unlink. Directory-sync-feil returnerer durable=false etter commit. Stat/hash er ikke atomisk CAS.

## 5. Plumbing

Tabellen beskriver implementerte kall. Navngitte hendelser er injiserte callbacks, ikke en global event bus.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `DocumentCoordinator::requestOpen` | `LocalFileStore::read` | `src/application/io/LocalFileStore.cpp` | Path → bytes/identity | Bounded read og stat-sjekk | Implemented |
| 2 | `LocalFileStore::read / writeAtomic` | `InputPolicy::validate` | `src/application/io/InputPolicy.cpp` | Bytes/path → validering | Error ved ugyldig input | Implemented |
| 3 | `DocumentCoordinator::save` | `LocalFileStore::writeAtomic` | `src/application/io/LocalFileStore.cpp` | Bytes/expected → nytt mål | Temp ryddes ved feil | Implemented |
| 4 | `LocalFileStore::writeAtomic` | `copyAttributes` | `src/application/io/LocalFileStore.cpp` | Åpne descriptors → bevart metadata | Feil før commit | Implemented |

## 6. Gjenbruk og avhengigheter

Ingen andre functionality-kontrakter konsumeres; delte datatyper følger arkitekturen.

Dokumentkoordinatoren er første konsument; framtidig eksport kan bruke lagring hvis samme metadata-/konfliktpolicy passer. Ikke opprett alternative save-metoder inne i features.

## 7. Verifikasjon

Relevante akseptanse-ID-er: AT-001, AT-003, AT-009, AT-015, AT-016, AT-017, AT-021, AT-023.

`DocumentTest` injiserer feil før rename, tester xattr/mode, hardlink, ny-fil-identitet, ekstern endring, invalid UTF-8 og 8 MiB.

Evidence: [Fase P2](../../../docs/evidence/P2.md). Samlet kravdekning og eventuelle gjenstående begrensninger kontrolleres i P7; Implemented er ikke automatisk Verified.

## 8. Status, risiko og endringskonsekvenser

Implemented i P2. Oppdater kontrakter, kallkart, konsumenter og tester i samme endring.
Rene porter og tydelig rolleeierskap er obligatorisk. Eventuelle senere avvik står i fasens bevisrapport.
