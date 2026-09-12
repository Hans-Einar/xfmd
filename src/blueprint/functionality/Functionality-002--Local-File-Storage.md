---
id: FUNC-002
kind: Functionality
audience: System
role: Service
owner: application
status: Proposed
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

`LocalFileStore::read(Path) -> Result<LoadedDocument>`, `writeAtomic(SourceSnapshot, Path, ExpectedFileIdentity) -> Result<SavedSnapshot>`; `InputPolicy::validate(ByteView, Path) -> Result<FormatInfo>`. LoadedDocument har eid tekst, BOM/linjesluttmetadata, oppløst målsti og filidentitet. Resultatet skiller UnsupportedInput, TooLarge, Conflict og IoError.

## 4. Atferd, tilstand og feil

Les med størrelsesgrense under lesing, ikke bare stat før lesing. Bevar rå byte for tapsfri lagring; tolking kan bruke et eksplisitt mapped utsnitt uten BOM. Kontroller UTF-8, NUL og endelse. Skriv tempfil i målmappen, bevar støttede metadata, flush og rename; rydd temp ved feil. Kontroller ekstern endring før erstatning. Etter rename men feilet katalog-sync rapporteres durability-uklarhet og ny faktisk filidentitet, ikke påstått rollback.

## 5. Plumbing

Alle symboler og kildefiler i tabellen er **planlagte**, ikke implementert kode.
Bibliotekskall verifiseres mot valgt dependency-versjon før implementering.

| Steg | Hendelse / kaller | Kalt symbol | Kilde eller kontraktfil | Data / resultat | Feil / sideeffekt | Status |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | `DocumentCoordinator::requestOpen` | `LocalFileStore::read` | `src/application/io/LocalFileStore.cpp` | Path → avgrenset bytebuffer | Mangler/rettighet/størrelse gir typet feil. | Planned |
| 2 | `LocalFileStore::read` | `InputPolicy::validate` | `src/application/io/InputPolicy.cpp` | Bytes + endelse → FormatInfo | Ugyldig UTF-8/NUL avvises før øktbytte. | Planned |
| 3 | `DocumentCoordinator::save` | `LocalFileStore::writeAtomic` | `src/application/io/LocalFileStore.cpp` | Snapshot + filidentitet | Konflikt eller unsupported metadata avbryter. | Planned |
| 4 | `LocalFileStore::writeAtomic` | `LocalFileStore::writeTemporary` | `src/application/io/LocalFileStore.cpp` | Bytes + metadata → tempfil | Diskfeil rydder temp; mål uendret. | Planned |
| 5 | `LocalFileStore::writeAtomic` | `LocalFileStore::commitTemporary` | `src/application/io/LocalFileStore.cpp` | Flush + kontroll + rename → SavedSnapshot | Rename er commit-punkt; usikker durability rapporteres separat. | Planned |

## 6. Gjenbruk og avhengigheter

Ingen andre functionality-kontrakter konsumeres; delte datatyper følger arkitekturen.

Dokumentkoordinatoren er første konsument; framtidig eksport kan bruke lagring hvis samme metadata-/konfliktpolicy passer. Ikke opprett alternative save-metoder inne i features.

## 7. Verifikasjon

AT-001, AT-003, AT-009, AT-015, AT-016, AT-017, AT-021, AT-023: midlertidige mapper, full-disk/permission-feil via feilinjeksjon, CRLF/BOM byte-roundtrip, ekstern endring, symlink/hardlink og grense under lesing. Planlagt `tests/application/LocalFileStoreTest.cpp`.

Bevis: ingen applikasjonstest kjørt; testfiler ovenfor er planlagte. Ved implementering
oppgis kommando, fixture, miljø, commit og faktisk utfall. Strukturkontroll alene
oppfyller ikke atferdskravene.

## 8. Status, risiko og endringskonsekvenser

Proposed, revisjon 0.1, 2026-09-12. P0 fastsetter ACL/xattr-policy, linjesluttbevaring og grense. Før/etter-rename-feil må skilles. Stat/hash gir ikke atomisk compare-and-swap mot eksterne prosesser.

Ved endret offentlig kontrakt: oppdater konsumentene i registeret, dette kallkartet,
berørte krav og kontrakttester i samme endring. Før status Ready skal relevante
P0-spørsmål være avgjort; før Verified skal kapittel 7 inneholde testbevis.
